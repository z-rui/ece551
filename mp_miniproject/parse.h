#ifndef PARSE_H
#define PARSE_H

#include <string>
#include <list>
#include <vector>

class Parser {
public:
	struct Command {
		std::vector<const char *> argv;
		const char *redir[3];
	};
	typedef std::list<Command> Pipes;
	bool parse(const char *, Pipes&);

private:
	std::list<std::string> scannedTerms;
	const char *pos;

	bool endOfLine() const { return *pos == '\0'; }
	char peek(int n = 0) const { return pos[n]; }
	void next(int n = 1) { pos += n; }
	void skipSpaces();
	bool parseCommand(Command&);
	bool parsePipes(Pipes&);
	const char *scanTerm();
};


#endif /* PARSE_H */
