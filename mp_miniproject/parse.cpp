#include <ctype.h> /* for isspace, etc. */

#include "parse.h"

/* Parser::parse parses one line.
 *
 */
bool Parser::parse(const char *line, Parser::Command& cmd)
{
	bad = false;
	pos = line;

	parseCommand(cmd);
	if (!endOfLine()) {
		bad = true;
	}
	return !bad;
}

/* Parser::parseCommand parses a command.
 *
 * command ::= empty | command-core { '|' command-core }
 */
void Parser::parseCommand(Parser::Command& cmd)
{
	skipSpaces();
	if (endOfLine()) { // empty
		return;
	}
	for (;;) {
		cmd.push_back(CommandCore());
		parseCommandCore(cmd.back());

		skipSpaces();
		if (peek() == '|') {
			next();
		} else {
			break;
		}
	}
}

/* Parser::parseCommandCore parses a command 'core' (i.e., without pipes).
 *
 * command-core ::= command-term { command-term }
 * command-term ::= TERM | '<' TERM | '>' TERM | "2>" TERM
 */
void Parser::parseCommandCore(Parser::CommandCore& core)
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
		if (redir >= 0) {
			core.redir[redir] = term;
		} else {
			core.argv.push_back(term);
		}

		skipSpaces();
		if (endOfLine() || peek() == '|') {
			// argv ends with a NULL
			core.argv.push_back(NULL);
			break;
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
	const char *save;
	bool terminate = false;

	scannedTerms.push_back(std::string());
	std::string& term = scannedTerms.back();

	skipSpaces();
	save = pos;
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
		bad = true;
	}
	return term.c_str();
}

void Parser::skipSpaces()
{
	while (isspace(*pos)) {
		pos++;
	}
}
