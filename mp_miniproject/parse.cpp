#include <ctype.h> /* for isspace, etc. */

#include "parse.h"

/* Parser::parse parses one line.
 *
 */
bool Parser::parse(const char *line, Parser::Pipes& pipes)
{
	pos = line;

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
bool Parser::parseCommand(Parser::Command& cmd)
{
	for (;;) {
		char ch;
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
			return false;
		}
		if (redir >= 0) {
			cmd.redir[redir] = term;
		} else {
			cmd.argv.push_back(term);
		}

		skipSpaces();
		if (endOfLine() || peek() == '|') {
			// argv ends with a NULL
			cmd.argv.push_back(NULL);
			return true;
		}
	}
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
		char ch = peek();
		switch (ch) {
			case '\0':
			case ' ': case '\t': case '\n': case '\f': case '\v':
			case '<': case '>': case '|':
				terminate = true;
				break;
			case '\\':
				next(); // skip backslash
				if ((ch = peek()) == '\0') {
					break;
				}
				/* FALLTHROUGH */
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

void Parser::skipSpaces()
{
	while (isspace(peek())) {
		next();
	}
}
