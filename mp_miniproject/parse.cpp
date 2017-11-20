#include <ctype.h> /* for isspace, etc. */
#include <string.h> /* for strcmp */

#include "parse.h"

Parser::Parser(const VarTab &vtab) : vtab(vtab)
{
}

void Parser::reportSyntaxError(std::ostream& os) const
{
	os << "Syntax error: unexpected ";
	if (isprint(*pos)) {
		if (*pos == '\'') {
			os << "<single quote>";
		} else {
			os << '\'' << *pos << '\'';
		}
	} else {
		switch (*pos) {
		case '\n':
			os << "<newline>";
			break;
		case '\t':
			os << "<tab>";
			break;
		case '\0':
			os << "<eol>";
			break;
		default:
			os << '\\' << std::oct << (int) *pos << std::dec;
			break;
		}
	}
	os << "\n";
}

/* Parser::parse parses one line.
 *
 */
bool Parser::parse(const char *line, Parser::Pipes& pipes)
{
	pos = line;
	pos_before_expansion = NULL;

	scannedTerms.clear();
	specials.redir = specials.space =
		specials.expand = specials.escape = 1;

	return parsePipes(pipes) && endOfLine();
}

/* Parser::parsePipes parses piped commands.
 * Pipes can be empty.  In this case no command is run.
 *
 * pipes ::= empty | command { '|' command }
 */
bool Parser::parsePipes(Parser::Pipes& pipes)
{
	skipSpaces();
	if (endOfLine()) { // empty
		return true;
	}
	for (;;) {
		pipes.push_back(Command());
		if (!parseCommand(pipes.back()))
			return false; // failed

		skipSpaces();
		if (catcode(peek()) == PIPE) {
			next();
		} else {
			return true;
		}
	}
}

/* Parser::parseCommand parses a command (without pipes).
 *
 * command ::= command-term { command-term }
 * command-term ::= TERM | '<' TERM | '>' TERM | "2>" TERM
 */
bool Parser::parseCommand(Parser::Command& cmd)
{
	bool first = true;
	for (;;) {
		unsigned char ch;
		int redir = -1; /* -1: no redir; 0,1,2: redir fd 0,1 or 2 */

		skipSpaces();
		ch = peek();
		if (catcode(ch) == REDIR0) {
			redir = 0;
			next();
		} else if (catcode(ch) == REDIR1) {
			redir = 1;
			next();
		} else if (ch == '2' && catcode(peek(1)) == REDIR1) {
			redir = 2;
			next(2);
		}
		const char *term = scanTerm();
		if (term == NULL) { // failed
			return Command::INVALID;
		}
		if (redir >= 0) {
			cmd.redir[redir] = term;
		} else {
			cmd.argv.push_back(term);
		}
		if (first) {
			first = false;
			cmd.type = Command::ORDINARY;
			parseBuiltin(cmd);
			if (cmd.type == Command::INVALID) {
				return false;
			}
			if (cmd.type != Command::ORDINARY) {
				// Builtin commands parses
				// arguments own their own
				return true;
			}
		}

		skipSpaces();
		if (endOfLine() || catcode(peek()) == PIPE) {
			// end of a command
			// argv ends with a NULL
			cmd.argv.push_back(NULL);
			return true;
		}
	}
}

void Parser::parseBuiltin(Parser::Command& cmd)
{
	const char *cmdname = cmd.argv[0];
	if (strcmp(cmdname, "set") == 0) {
		parseSetCommand(cmd);
	} else if (strcmp(cmdname, "export") == 0) {
		parseExportCommand(cmd);
	}
}

void Parser::parseSetCommand(Parser::Command& cmd)
{
	const char *name, *value;

	skipSpaces();
	name = scanName();
	if (name == NULL) {
		cmd.type = Command::INVALID;
		return;
	}
	// redir and space are inactive inside variable definition
	specials.redir = specials.space = 0;
	value = scanTerm();
	if (value == NULL) {
		value = ""; // "set x" sets x to empty
	}
	cmd.argv.push_back(name);
	cmd.argv.push_back(value);
	cmd.type = Command::SET;
}

void Parser::parseExportCommand(Parser::Command& cmd)
{
	const char *name;

	skipSpaces();
	name = scanName();
	if (name == NULL) {
		cmd.type = Command::INVALID;
		return;
	}
	cmd.argv.push_back(name);
	cmd.type = Command::EXPORT;
}

/* Parser::scanTerm scans a term in a command.
 *
 * A term is a sequence of normal characters.
 * Special characters ( <>|) can terminate a term.
 * A backslash (\) nullifies the specialness of whatever
 * character that follows, and itself does not go into the term.
 */
const char *Parser::scanTerm()
{
	scannedTerms.push_back(std::string());
	std::string& term = scannedTerms.back();

	skipSpaces();
	const char *save = pos;
	bool terminate = false;
	do {
		unsigned char ch = peek();
		const char *name;
		switch (catcode(ch)) {
		case EOL:
		case REDIR0: case REDIR1: case PIPE:
		case SPACE:
			terminate = true;
			break;
		case EXPAND:
			next(); // skip '$'
			name = scanName();
			if (name != NULL) {
				enterExpansion(name);
			} else { // a stray $
				term.push_back(ch);
			}
			break;
		case ESCAPE:
			next(); // skip '\'
			if ((ch = peek()) == '\0') {
				break;
			}
			/* FALL THROUGH */
		default:
			term.push_back(ch);
			next();
			break;
		}
	} while (!terminate);

	if (save == pos) { // no character consumed?
		return NULL;
	}
	return term.c_str();
}

const char *Parser::scanName()
{
	unsigned char ch = peek();
	if (!isalnum(ch) && ch != '_') {
		return NULL;
	}
	scannedTerms.push_back(std::string());
	std::string& name = scannedTerms.back();
	do {
		name.push_back(ch);
		next();
		ch = peek();
	} while (isalnum(ch) || ch == '_');
	return name.c_str();
}

void Parser::enterExpansion(const char *name)
{
	const char *value = vtab.getVar(name);
	if (value == NULL || value[0] == '\0') {
		// empty expansion; exit immediately
		return;
	}
	pos_before_expansion = pos;
	pos = value;
	specials_before_expansion = specials;
	specials.redir = specials.expand = specials.escape = 0;
}

void Parser::exitExpansion()
{
	pos = pos_before_expansion;
	pos_before_expansion = NULL;
	specials = specials_before_expansion;
}

Parser::Catcode Parser::catcode(unsigned char ch) const
{
	if (ch == '\0')
		return EOL;
	if (isspace(ch)) {
		return specials.space ? SPACE : OTHER;
	}
	switch (ch) {
	case '<':
		return specials.redir ? REDIR0 : OTHER;
	case '>':
		return specials.redir ? REDIR1 : OTHER;
	case '|':
		return specials.redir ? PIPE : OTHER;
	case '$':
		return specials.expand ? EXPAND : OTHER;
	case '\\':
		return specials.escape ? ESCAPE : OTHER;
	}
	if (isalnum(ch) || ch == '_') {
		return LETTER;
	}
	return OTHER;
}

void Parser::skipSpaces()
{
	while (isspace(peek())) {
		next();
	}
}

void Parser::next(int n)
{
	pos += n;
	if (*pos == '\0' && insideExpansion()) {
		// just finished an expansion
		exitExpansion();
	}
}
