/* an evil program that modifies argv[...] */

int main(int argc, char *argv[])
{
	argv[0][0] = '\0';
	return 0;
}
