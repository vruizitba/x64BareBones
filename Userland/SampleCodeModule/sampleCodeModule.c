/* sampleCodeModule.c */
extern unsigned char kbFlag(void);
extern void getTimeRTC(unsigned char* h, unsigned char* m, unsigned char*s);


void printStringWithColor(int row, int column, char *string, char attribute);
void printInt(int row, int column, unsigned int value, char attribute);

char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;

static inline void write(int fd, const char *buffer, int count) {
    __asm__ volatile (
        "movq $1, %%rax\n"
        "movq %0, %%rbx\n"
        "movq %1, %%rcx\n"
        "movq %2, %%rdx\n"
        "int $0x80\n"
        :
        : "r"((long)fd), "r"(buffer), "r"((long)count)
        : "rax", "rbx", "rcx", "rdx"
    );
}

int main() {
	//All the following code may be removed 
	*v = 'X';
	*(v+1) = 0x74;

	printStringWithColor(0, 0, "Arquitectura de computadoras", 0xF2);

	printCurrentTime(23, 0, 0xF9);

	waitAndPrintKey(21, 0);

	write(1, "Hola mundo", 11);


	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;

	write(1, "Hola mundo\n", 11);    // Imprime en pantalla (STDOUT)
	write(2, "Error!\n", 7);         // Imprime en rojo (STDERR)
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

void waitAndPrintKey(int row, int column) {


    const char scancodeToChar[128] = {
        0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z',
        'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0,
        // ...puedes completar el resto si lo necesitas
    };

	printStringWithColor(row, column, "Presiona una tecla...", 0x0F);

	unsigned char key = kbFlag();
	char keyStr[2] = {0};
	keyStr[0] = scancodeToChar[key];
	keyStr[1] = '\0';

	printStringWithColor(row + 1, column, "Tecla:", 0x0F);
	printStringWithColor(row + 1, column + 8, keyStr, 0x0F);
}

