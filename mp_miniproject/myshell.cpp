#include <unistd.h> /* for environ */
#include <stdlib.h>
#include <stdio.h>

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
	value = varTab.getVar("PWD");
	if (value == NULL) {
		char *value = get_current_dir_name();
		varTab.setVar("PWD", value);
		free(value);
	}
}

int MyShell::chdir(const char *path) const
{
	int rc = ::chdir(path);
	if (rc != 0) {
		perror("chdir");
	}
	return rc;
}

int main()
{
	MyShell().runREPL();
}
