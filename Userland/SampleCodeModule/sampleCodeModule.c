/* sampleCodeModule.c */

char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;

void printStringWithColor(char *string, char attribute);

int main() {
	//All the following code may be removed 
	*v = 'X';
	*(v+1) = 0x74;

	printStringWithColor("Arquitectura de computadoras", 0xF2);

	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;
}

void printStringWithColor(char *string, char attribute) {
	char *pos = 0xB8000;

	while (*string) {
		*pos++ = *string++;
		*pos++ = attribute;
	}
}