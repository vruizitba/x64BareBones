#include <stdint.h>
#include <naiveConsole.h>

#define SYS_WRITE 1
#define STDOUT 1
#define STDERR 2

const char scancodeToChar[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z',
    'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0,
    // ...completa si lo necesitas
};


static int pos = 0;

void process_scancode(unsigned char scancode) {
    if (scancode & 0x80) // Ignora key release
        return;
    char c = scancodeToChar[scancode];
    if (c) {
        volatile char *video = (char*)0xB8000;
        video[pos * 2] = c;
        video[pos * 2 + 1] = 0x0F;
        pos++;
        if (pos >= 80 * 25) pos = 0; // Limita a una pantalla
    }
}

void syscall_dispatcher(uint64_t syscall_number, uint64_t fd, char *buffer, uint64_t count) {
	switch (syscall_number) { //si quiero puedo agregar mas syscalls
		case SYS_WRITE:
			sys_write(fd, buffer, count);
			break; 
	}
	
}

void sys_write(uint64_t fd, char *buffer, uint64_t count) {
	uint8_t color = 0x0F; // Blanco por defecto
	if (fd == STDERR) {
		color = 0x04; //rojo
	}

	for (uint16_t i = 0; i < count; i++) {
		ncPrintCharColor(buffer[i], color);
	}

}


void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}
