#include <ctype.h> /* for isspace, etc. */
#include <string.h> /* for strcmp */

#include "parse.h"

Parser::Parser(const VarTab &vtab) : vtab(vtab)
{
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
		if (parseCommand(pipes.back()) == Command::INVALID)
			return false; // failed

		skipSpaces();
		if (peek() == '|') {
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
Parser::Command::Type Parser::parseCommand(Parser::Command& cmd)
{
	bool first = true;
	for (;;) {
		unsigned char ch;
		int redir = -1; /* -1: no redir; 0,1,2: redir fd 0,1 or 2 */

		skipSpaces();
		ch = peek();
		if (ch == '<') {
			redir = 0;
			next();
		} else if (ch == '>') {
			redir = 1;
			next();
		} else if (ch == '2' && peek(1) == '>') {
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
			Command::Type builtin = parseBuiltin(term, cmd);
			if (builtin != Command::INVALID) {
				cmd.type = builtin;
				return builtin;
			}
		}

		skipSpaces();
		if (endOfLine() || peek() == '|') { // end of a command
			// argv ends with a NULL
			cmd.argv.push_back(NULL);
			cmd.type = Command::ORDINARY;
			return Command::ORDINARY;
		}
	}
}

Parser::Command::Type Parser::parseBuiltin(const char *cmdname,
		Parser::Command& cmd)
{
	if (strcmp(cmdname, "set") == 0) {
		return parseSetCommand(cmd);
	} else if (strcmp(cmdname, "export") == 0) {
		return parseExportCommand(cmd);
	}
	return Command::INVALID;
}

Parser::Command::Type Parser::parseSetCommand(Parser::Command& cmd)
{
	const char *name, *value;

	skipSpaces();
	name = scanName();
	if (name == NULL) {
		return Command::INVALID;
	}
	// redir and space are inactive inside variable definition
	specials.redir = specials.space = 0;
	value = scanTerm();
	if (value == NULL) {
		value = ""; // "set x" sets x to empty
	}
	cmd.argv.push_back(name);
	cmd.argv.push_back(value);
	return Command::SET;
}

Parser::Command::Type Parser::parseExportCommand(Parser::Command& cmd)
{
	const char *name;

	skipSpaces();
	name = scanName();
	if (name == NULL) {
		return Command::INVALID;
	}
	cmd.argv.push_back(name);
	return Command::EXPORT;
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
		bool skip = false;
		switch (ch) {
		case '\0':
			terminate = true;
			break;
		case '<': case '>': case '|':
			terminate = specials.redir;
			break;
		case ' ': case '\t': case '\v': case '\f': case '\n':
			terminate = specials.space;
			break;
		case '$':
			if (!specials.expand) {
				break; // expansion disabled
			}
			next(); // skip '$'
			name = scanName();
			if (name != NULL) {
				enterExpansion(name);
			} else { // a stray $
				term.push_back(ch);
			}
			skip = true;
			break;
		case '\\':
			if (!specials.escape) {
				break;
			}
			next(); // skip '\'
			if ((ch = peek()) == '\0') {
				skip = true;
			}
			break;
		}
		if (!terminate && !skip) {
			term.push_back(ch);
			next();
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
