#ifndef PARSE_H
#define PARSE_H

#include <string>
#include <iostream>
#include <list>
#include <vector>

#include <limits.h>

#include "var.h"

class Parser {
public:
	struct Command {
		enum Type {
			INVALID = -1,
			ORDINARY, SET, EXPORT, CD,
		};
		Type type;
		std::vector<const char *> argv;
		const char *redir[3];
	};
	typedef std::list<Command> Pipes;

	Parser(const VarTab&);
	bool parse(const char *, Pipes&);
	void reportSyntaxError(std::ostream&) const;
private:
	enum Catcode {
		ESCAPE, EXPAND, SPACE, REDIR0, REDIR1, PIPE, LETTER, OTHER, EOL
	};
	struct LexState {
		const char *pos;
		unsigned redir:1, space:1, expand:1, escape:1;
	};
	LexState ls, ls_backup;

	const VarTab& vtab;
	std::list<std::string> scannedTerms;

	Catcode catcode(unsigned char) const;
	void enterExpansion(const char *);
	void exitExpansion();
	bool insideExpansion() const { return ls_backup.pos != NULL; }
	bool endOfLine() const { return *ls.pos == '\0'; }
	unsigned char peek(int n = 0) const { return ls.pos[n]; }
	void next(int n = 1);
	void skipSpaces();

	const char *scanTerm();
	const char *scanName();

	bool parseCommand(Command&);
	void parseBuiltin(Command&);
	void parseSetCommand(Command&);
	void parseExportCommand(Command&);
	void parseCdCommand(Command&);
	bool parsePipes(Pipes&);
};


#endif /* PARSE_H */
