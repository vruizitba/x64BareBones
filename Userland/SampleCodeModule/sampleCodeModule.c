/* sampleCodeModule.c */

void printStringWithColor(int row, int column, char *string, char attribute);
extern void getTimeRTC(unsigned char* h, unsigned char* m, unsigned char*s);
void printInt(int row, int column, unsigned int value, char attribute);

char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;

int main() {
	//All the following code may be removed 
	*v = 'X';
	*(v+1) = 0x74;

	printStringWithColor(0, 0, "Arquitectura de computadoras", 0xF2);

	printCurrentTime(23, 0, 0xF9);

	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;
}

void printStringWithColor(int row, int column, char *string, char attribute) {
	
	char *pos = 0xB8000 + ((row * 80 * 2) + (column * 2));
	while (*string) {
			*pos++ = *string++;
			*pos++ = attribute;
	}
}

void printInt(int row, int column, unsigned int value, char attribute) {
    char buffer[12];
    int i = 10;
    buffer[11] = 0;
    if (value == 0) {
        buffer[10] = '0';
        printStringWithColor(row, column, &buffer[10], attribute);
        return;
    }
    while (value > 0 && i > 0) {
        buffer[i--] = (value % 10) + '0';
        value /= 10;
    }
    printStringWithColor(row, column, &buffer[i + 1], attribute);
}

void printCurrentTime(int row, int column, char attribute) {
	unsigned char h, m, s;
	getTimeRTC(&h, &m, &s);
	printStringWithColor(row, column, "Hora:", attribute);
	printInt(row, column + 5, h, attribute);
	printStringWithColor(row, column +8, "Minuto:", attribute);
	printInt(row, column + 15, m, attribute);
	printStringWithColor(row, column + 18, "Segundo:", attribute);
	printInt(row, column + 26, s, attribute);
}