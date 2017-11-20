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
			ORDINARY, SET, EXPORT,
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
	Catcode catcode(unsigned char) const;
	const VarTab& vtab;
	std::list<std::string> scannedTerms;
	const char *pos;
	const char *pos_before_expansion;
	struct Specials {
		unsigned redir:1, space:1, expand:1, escape:1;
	};
	Specials specials, specials_before_expansion;

	bool insideExpansion() const { return pos_before_expansion != NULL; }
	bool endOfLine() const { return *pos == '\0'; }
	unsigned char peek(int n = 0) const { return pos[n]; }
	void next(int n = 1);
	void skipSpaces();
	bool parseCommand(Command&);
	void parseBuiltin(Command&);
	void parseSetCommand(Command&);
	void parseExportCommand(Command&);
	bool parsePipes(Pipes&);
	const char *scanTerm();
	const char *scanName();
	void enterExpansion(const char *);
	void exitExpansion();
};


#endif /* PARSE_H */
