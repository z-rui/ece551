#ifndef PARSE_H
#define PARSE_H

#include <string>
#include <iostream>
#include <list>
#include <vector>

#include <limits.h>

#include "var.h"
#include "debug.h"

class Parser {
public:
	struct Command {
		enum Type {
			SET, EXPORT, CD,
			ORDINARY, INVALID
		};
		Type type;
		std::vector<const char *> argv;
		const char *redir[3];
	};
	typedef std::list<Command> Pipes;
	DebugStream debug;

	Parser(const VarTab&);
	bool parse(const char *, Pipes&);
	void reportSyntaxError(std::ostream&) const;
private:
	enum Catcode {
		ESCAPE, EXPAND, SPACE,
		REDIR0, REDIR1, PIPE,
		LETTER, OTHER, EOL
	};
	struct LexState {
		const char *pos;
		unsigned redir:1, space:1, expand:1, escape:1;
	};
	LexState ls, ls_backup;

	const VarTab& vtab;
	std::list<std::string> scannedTerms;

	Catcode catcode(unsigned char) const;
	void doExpansion();
	bool insideExpansion() const { return ls_backup.pos != NULL; }
	bool endOfLine() const { return *ls.pos == '\0'; }
	unsigned char peek(int n = 0) const { return ls.pos[n]; }
	void next(int n = 1);
	void skipSpaces();

	const char *scanTerm();
	const char *scanName();
	int scanRedir();

	bool parsePipes(Pipes&);
	bool parseCommand(Command&);

	static Command::Type commandType(const char *);
	static void (Parser::*const parseBuiltins[])(Command&);
	void parseSet(Command&);
	void parseExport(Command&);
	void parseCd(Command&);
};


#endif /* PARSE_H */
