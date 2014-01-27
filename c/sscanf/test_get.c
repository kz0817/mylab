#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	const char *test = "1 (sa) T 5 3";
	int val0;
	int val1;
	char buf[5] = {0x1, 0x1, 0x1, 0x1, 0x1};
	char ch;
	int ret = sscanf(test, "%d (%02s) %c %d", &val0, buf, &ch, &val1);
	printf("ret: %d, val0: %d, ch: %c, val1: %d\n", ret, val0, ch, val1);
	printf("buf: %02x %02x %02x %02x %02x\n", buf[0], buf[1], buf[2], buf[3], buf[4]);

	return EXIT_SUCCESS;
}
