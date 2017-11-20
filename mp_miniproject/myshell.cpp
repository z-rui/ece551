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
	updatePWD();
}

int MyShell::chdir(const char *path)
{
	int rc = ::chdir(path);
	if (rc != 0) {
		perror("chdir");
	} else {
		updatePWD();
	}
	return rc;
}

void MyShell::updatePWD()
{
	char *pwd = get_current_dir_name();
	if (pwd == NULL) {
		perror("get_current_dir_name");
		exit(EXIT_FAILURE);
	}
	varTab.setVar("PWD", pwd);
	free(pwd);
}

int main()
{
	MyShell().runREPL();
}
