#include <ctype.h> /* for isspace, etc. */
#include <string.h> /* for strcmp */

#include "parse.h"

Parser::Parser(const VarTab &vtab) : vtab(vtab)
{
}

void (Parser::*const Parser::parseBuiltins[])(Command&) = {
	[Parser::Command::SET] = &Parser::parseSet,
	[Parser::Command::EXPORT] = &Parser::parseExport,
	[Parser::Command::CD] = &Parser::parseCd,
};

void Parser::reportSyntaxError(std::ostream& os) const
{
	unsigned char ch = peek();
	os << "Syntax error: unexpected ";
	switch (ch) {
	case '\0':
	case '\n':
		os << "<newline>";
		break;
	case '\t':
		os << "<tab>";
		break;
	default:
		os << '\'';
		if (isprint(ch) && ch != '\'') {
			os << ch;
		} else {
			os << '\\' << std::oct << (int) ch << std::dec;
		}
		os << '\'';
		break;
	}
	os << "\n";
}

/* Parser::parse parses one line.
 *
 */
bool Parser::parse(const char *line, Parser::Pipes& pipes)
{
	ls.pos = line;
	ls_backup.pos = NULL;

	scannedTerms.clear();
	ls.redir = ls.space = ls.expand = ls.escape = 1;

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
			return false;
		}
		if (redir >= 0) {
			cmd.redir[redir] = term;
		} else {
			cmd.argv.push_back(term);
		}
		if (first) {
			first = false;
			cmd.type = commandType(term);
			if (cmd.type < Command::ORDINARY) {
				(this->*parseBuiltins[cmd.type])(cmd);
				if (cmd.type == Command::INVALID) {
					return false;
				}
				// Builtin commands parses
				// arguments own their own
				break;
			}
		}

		skipSpaces();
		if (endOfLine() || catcode(peek()) == PIPE) {
			break;
		}
	}
	// end of a command
	// argv ends with a NULL
	cmd.argv.push_back(NULL);
	return true;
}

Parser::Command::Type Parser::commandType(const char *name)
{
	if (strcmp(name, "set") == 0) {
		return Command::SET;
	} else if (strcmp(name, "export") == 0) {
		return Command::EXPORT;
	} else if (strcmp(name, "cd") == 0) {
		return Command::CD;
	}
	return Command::ORDINARY;
}

/* Parser::parseSet parses the set command.
 *
 * command ::= "set" NAME DEFINITION
 */
void Parser::parseSet(Parser::Command& cmd)
{
	const char *name, *value;

	skipSpaces();
	name = scanName();
	if (name == NULL) {
		cmd.type = Command::INVALID;
		return;
	}
	skipSpaces();

	/* DEFINITION is similar to TERM (see Parser::scanTerm)
	 * because expansions still take place,
	 * so that a command like "set PATH $HOME/bin:$PATH"
	 * will behave as expected.
	 * Escaping is also in effect, in case the user
	 * wants a true dollar sign in the definition.
	 *
	 * But '<', '|', '>' and spaces lose specialness
	 * in this context.
	 */
	ls.redir = ls.space = 0;
	value = scanTerm();
	if (value == NULL) {
		value = ""; // "set x" sets x to empty
	}
	cmd.argv.push_back(name);
	cmd.argv.push_back(value);
	cmd.type = Command::SET;
}

/* Parser::parseExport parses the export command.
 *
 * command ::= "export" NAME
 */
void Parser::parseExport(Parser::Command& cmd)
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

/* Parser::parseCd parses the cd command.
 *
 * command ::= "cd" TERM
 */
void Parser::parseCd(Parser::Command& cmd)
{
	const char *path;

	path = scanTerm();
	if (path == NULL) {
		cmd.type = Command::INVALID;
		return;
	}
	cmd.argv.push_back(path);
	cmd.type = Command::CD;
}

/* Parser::scanTerm scans a TERM in a command.
 *
 * What goes into a term depends on the context.
 * (See Parser::catcode).
 *
 * - Characters of catcode EOL (\0), REDIR0 (<),
 *   REDIR1 (>), PIPE (|) and SPACE terminate a term.
 * - Characters of catcode EXPAND ($) followed by
 *   a NAME token is expanded to its definition,
 *   in the expansion context.
 * - Characters of catcode ESCAPE (\) ensures that
 *   the next character (except \0) goes into the term,
 *   regardless of its catcode.
 *
 * The characters '<', '|', '>', '$', '\' and spaces
 * may lose specialness in certain contexts.
 * In these contexts they will have catcode OTHER,
 * and thus go into the term.
 */
const char *Parser::scanTerm()
{
	scannedTerms.push_back(std::string());
	std::string& term = scannedTerms.back();

	skipSpaces();
	const char *save = ls.pos;
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
				// too bad, there's no "next character"
				break;
			}
			/* FALL THROUGH */
		default:
			term.push_back(ch);
			next();
			break;
		}
	} while (!terminate);

	if (save == ls.pos) { // no character consumed?
		return NULL;
	}
	return term.c_str();
}

/* Parser::scanName scans a NAME.
 *
 * A NAME consists of only characters of catcode LETTER.
 * (See Parser::catcode).
 * In this implmentation, they include characters
 * determined by isalnum, plus the underscore (_).
 *
 * Several places require a valid NAME, such as:
 * - the first argument of the set and export command;
 * - after a '$' in order to trigger an expansion.
 */
const char *Parser::scanName()
{
	unsigned char ch = peek();
	if (catcode(ch) != LETTER) {
		return NULL;
	}
	scannedTerms.push_back(std::string());
	std::string& name = scannedTerms.back();
	do {
		name.push_back(ch);
		next();
		ch = peek();
	} while (catcode(ch) == LETTER);
	return name.c_str();
}

void Parser::enterExpansion(const char *name)
{
	const char *value = vtab.getVar(name);
	if (value == NULL || value[0] == '\0') {
		// empty expansion; exit immediately
		return;
	}
	ls_backup = ls;
	ls.pos = value;
	/* Inside an expansion, '<', '|', '>', '$' and '\'
	 * lose their specialness.
	 * As a result, at most one level of expansion
	 * can take place.
	 *
	 * Whether spaces terminates a term depends
	 * on the context before entering the expansion.
	 */
	ls.redir = ls.expand = ls.escape = 0;
}

void Parser::exitExpansion()
{
	ls = ls_backup;
	ls_backup.pos = NULL;
}

/* Parser::catcode return the "category code" of an character.
 *
 * This mechanism simplifies the rest of the parser,
 * as it does not need to keep track of the context.
 */
Parser::Catcode Parser::catcode(unsigned char ch) const
{
	if (ch == '\0')
		return EOL;
	if (isspace(ch)) {
		return ls.space ? SPACE : OTHER;
	}
	switch (ch) {
	case '<':
		return ls.redir ? REDIR0 : OTHER;
	case '>':
		return ls.redir ? REDIR1 : OTHER;
	case '|':
		return ls.redir ? PIPE : OTHER;
	case '$':
		return ls.expand ? EXPAND : OTHER;
	case '\\':
		return ls.escape ? ESCAPE : OTHER;
	}
	if (isalnum(ch) || ch == '_') {
		return LETTER;
	}
	return OTHER;
}

void Parser::skipSpaces()
{
	while (catcode(peek()) == SPACE) {
		next();
	}
}

void Parser::next(int n)
{
	ls.pos += n;
	if (*ls.pos == '\0' && insideExpansion()) {
		// just finished an expansion
		exitExpansion();
	}
}
