#include <ctype.h> /* for isspace, etc. */
#include <string.h> /* for strcmp */

#include "parse.h"

/* Parser::parse parses one line.
 *
 * It returns true on success and false on failure.
 *
 * If succeeded, pipes will contain several commands
 * that should be piped in order.
 * Otherwise, the content of pipes should not be used,
 * and the caller can call Parser::reportSyntaxError to
 * print an error message.
 */
bool Parser::parse(const char *line, Parser::Pipes& pipes)
{
	ls.pos = line;
	ls_backup.pos = NULL;

	savedStrings.clear();
	ls.redir = ls.space = ls.expand = ls.escape = 1;

	return parsePipes(pipes) && endOfLine();
}

/* Parser::reportSyntaxError puts an error message to
 * the given stream regarding the syntax error that happened
 * when Parser::parse was called.
 *
 * Only call this after Parser::parse returned false.
 */
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
		if (isprint(ch)) {
			os << ch;
		} else {
			os << '\\' << std::oct << (int) ch << std::dec;
		}
		os << '\'';
		break;
	}
	os << "\n";
}

/* Parser::parseBuiltins is the "jump table" for the parser
 * so that the parser can call the appropriate method
 * when parsing builtin commands.
 */
bool (Parser::*const Parser::parseBuiltins[])(Command&) = {
	[Command::SET] = &Parser::parseSet,
	[Command::EXPORT] = &Parser::parseExport,
	[Command::CD] = &Parser::parseCd,
};

/* Parser::parsePipes parses piped commands.
 * Pipes can be empty.  In this case it contains no command.
 */
bool Parser::parsePipes(Parser::Pipes& pipes)
{
	skipSpaces();
	if (endOfLine()) { // empty
		debug << "Parser: pipes ::= %empty\n";
		return true;
	}
	for (;;) {
		pipes.push_back(Command());
		if (!parseCommand(pipes.back())) {
			return false; // failed
		}
		debug << "Parser: pipes ::= "
			<< ((pipes.size() > 1) ? "pipes '|' command\n" : "command\n");

		skipSpaces();
		if (catcode(peek()) == PIPE) {
			debug << "Parser: PIPE '|'\n";
			next();
		} else {
			return true;
		}
	}
}

/* Parser::parseCommand parses a command (without pipes).
 */
bool Parser::parseCommand(Parser::Command& cmd)
{
	bool first = true;
	for (;;) {
		static const char *const redir_str[] = { // for debugging
			"'<'", "'>'", "\"2>\""
		};
		skipSpaces();
		int redir = scanRedir();
		if (redir >= 0) {
			debug << "Parser: REDIR " << redir_str[redir] << '\n';
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
			cmd.type = commandType(term);
			// all builtin commands' type is smaller than
			// Command::ORDINARY, so the following test is fine
			if (cmd.type < Command::ORDINARY) {
				if (!(this->*parseBuiltins[cmd.type])(cmd)) {
					// failed to parse a builtin
					return false;
				}
				// Builtin commands parses arguments
				// own their own; no need to proceed
				break;
			}
		}
		debug << "Parser: command ::= ";
		if (!first) {
			debug << "command ";
		}
		first = false;

		if (redir >= 0) {
			debug << redir_str[redir] << ' ';
		}
		debug << "TERM\n";
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
 */
bool Parser::parseSet(Parser::Command& cmd)
{
	const char *name, *value;

	skipSpaces();
	name = scanName();
	if (name == NULL) {
		return false;
	}
	unsigned char ch = peek();
	if (ch != '\0' && catcode(ch) != SPACE) {
		// a space is required to separate the name
		// and the value
		return false;
	}
	skipSpaces();

	/* Inside the definition of a variable,
	 * '<', '|', '>' and spaces lose specialness.
	 * However, expansions still take place,
	 * so that a command like "set PATH $HOME/bin:$PATH"
	 * will behave as expected.
	 * Escaping is also in effect, in case the user
	 * wants a true dollar sign in the definition.
	 */
	ls.redir = ls.space = 0;
	value = scanTerm();
	if (value == NULL) {
		value = ""; // "set x" sets x to empty
	}
	cmd.argv.push_back(name);
	cmd.argv.push_back(value);
	debug << "Parser: command ::= \"set\" NAME TERM\n";
	return true;
}

/* Parser::parseExport parses the export command.
 */
bool Parser::parseExport(Parser::Command& cmd)
{
	const char *name;

	skipSpaces();
	name = scanName();
	if (name == NULL) {
		return false;
	}
	cmd.argv.push_back(name);
	debug << "Parser: command ::= \"export\" NAME\n";
	return true;
}

/* Parser::parseCd parses the cd command.
 */
bool Parser::parseCd(Parser::Command& cmd)
{
	const char *path;

	path = scanTerm();
	if (path == NULL) {
		return false;
	}
	cmd.argv.push_back(path);
	debug << "Parser: command ::= \"cd\" TERM\n";
	return true;
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
 * In these contexts they will have catcode OTHER.
 */
const char *Parser::scanTerm()
{
	savedStrings.push_back(std::string());
	std::string& term = savedStrings.back();

	skipSpaces();
	const char *save = ls.pos;
	for (;;) {
		switch (catcode(peek())) {
		case EOL:
		case REDIR0: case REDIR1: case PIPE:
		case SPACE:
			if (save == ls.pos) { // no character consumed?
				return NULL;
			}
			debug << "Parser: TERM \"" << term << "\"\n";
			return term.c_str();
		case EXPAND:
			doExpansion();
			break;
		case ESCAPE:
			next(); // skip '\'
			if (catcode(peek()) == EOL) {
				// too bad, there's no "next character"
				continue;
			}
			break;
		default:
			break;
		}
		term.push_back(peek());
		next();
	}
}

/* Parser::scanName scans a NAME.
 *
 * A NAME consists of only characters of catcode LETTER.
 * (See Parser::catcode).
 *
 * Several places require a valid NAME, such as:
 * - the first argument of the set and export command;
 * - after a '$' in order to trigger an expansion.
 *
 * This function stops at the first character that does not has
 * catcode LETTER.
 */
const char *Parser::scanName()
{
	unsigned char ch = peek();
	if (catcode(ch) != LETTER) {
		return NULL;
	}
	savedStrings.push_back(std::string());
	std::string& name = savedStrings.back();
	do {
		name.push_back(ch);
		next();
		ch = peek();
		if (catcode(ch) == EXPAND) {
			// "set x$x y" ?
			doExpansion();
			ch = peek();
		}
	} while (catcode(ch) == LETTER);
	debug << "Parser: NAME \"" << name << "\"\n";
	return name.c_str();
}

/* Parser::scanRedir scans '<', '>', or "2>",
 * and returns 0, 1 or 2 respectively.
 *
 * -1 is returned if it cannot scan anything.
 */
int Parser::scanRedir()
{
	unsigned ch = peek();
	if (catcode(ch) == REDIR0) {
		next();
		return 0;
	} else if (catcode(ch) == REDIR1) {
		next();
		return 1;
	} else if (ch == '2' && catcode(peek(1)) == REDIR1) {
		next(2);
		return 2;
	}
	return -1;
}

/* Parser::doExpansion is called when the current character is '$',
 * and it will try to expand the dollar.
 *
 * $var will be expanded to its value. Before all the characters
 * in the expansion are consumed, the parser is in the expansion
 * context.
 */
void Parser::doExpansion()
{
	next(); // skip '$'
	/* When scanning for the variable name after a $,
	 * do not expand another $ (i.e., $$x is not a valid expansion
	 * even if $x expands to a valid name).
	 * Otherwise multiple levels of expansion may happen.
	 */
	ls.expand = 0;
	const char *name = scanName();
	ls.expand = 1;
	if (name == NULL) {
		// '$' not followed by a NAME
		next(-1); // back up
		return;
	}
	const char *value = varTab.getVar(name);
	debug << "Parser: -> ";
	if (value == NULL || value[0] == '\0') {
		// empty expansion; exit immediately
		debug << '\n';
		return;
	}
	debug << value << '\n';
	ls_backup = ls; // save lexical state
	ls.pos = value; // let the cursor points to the expansion
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

/* Parser::catcode return the "category code" of an character.
 *
 * This is the only function that reads the context variables
 * in the lexical state.  All other functions will rely on
 * the catcode to determine whether a character is special.
 */
Parser::Catcode Parser::catcode(unsigned char ch) const
{
	if (ch == '\0') {
		return EOL;
	}
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
		// LETTER is actually letters, digits, and _
		return LETTER;
	}
	return OTHER;
}

/* Parser::skipSpaces skips spaces in the input.
 */
void Parser::skipSpaces()
{
	for (;;) {
		if (catcode(peek()) == EXPAND) {
			doExpansion();
		}
		if (catcode(peek()) == SPACE) {
			next();
		} else {
			break;
		}
	}
}

/* Parser::next shifts the reading cursor.
 *
 * By default, it shifts by one character.
 * The caller can choose to shift by other amount
 * if it will not go out of bound.
 */
void Parser::next(int n)
{
	ls.pos += n;
	if (*ls.pos == '\0' && insideExpansion()) {
		// finished an expansion. restore lex state
		ls = ls_backup;
		ls_backup.pos = NULL;
	}
}
