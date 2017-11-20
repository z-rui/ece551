#include <unistd.h> /* for environ */

#include "myshell.h"

/* Constructer
 */
MyShell::MyShell() : parser(varTab)
{
	varTab.importExported(environ);
	const char *value = varTab.getVar("PATH");
	if (value != NULL) {
		pathSearcher.setPath(value);
	}
}

int main()
{
	MyShell().runREPL();
}
