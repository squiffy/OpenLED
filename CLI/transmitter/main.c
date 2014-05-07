#include <stdio.h>

union Halfword {
	struct {
		unsigned char high:8;
		unsigned char low:8;
	} bytes;

	short hword_value;
};

unsigned char calculateCheck(unsigned char packet[]) {

	union Halfword sum;

	sum.hword_value = 0x0000;

	int i;
	for(i = 0;i < 10; i++)
		sum.hword_value += packet[i];

	unsigned char ret = (unsigned char)(sum.bytes.high + sum.bytes.low);

	return ret;
}

void applyColor(int red, int green, int blue) {

	/*
	 * XXX: eventually make the packet a struct.
	 */

	unsigned char packet[10];

	/*
	 * set magic bytes
	 */

	packet[0] = 0xaa;
	packet[1] = 0x55;
	packet[2] = 0x00;

	/*
	 * Set mode byte. 0x01 since we're only changing colors
	 */

	packet[3] = 0x01;

	/*
	 * Set keyNumber to 0x05 to change colors.
	 */

	packet[4] = 0x05;

	/*
	 * Set keyValue to brightness value. 0x64 for 100% brightness.
	 */

	packet[5] = 0x64;

	/*
	 * Now we set the colors! RGB format
	 */

	packet[6] = (unsigned char)red;
	packet[7] = (unsigned char)green;
	packet[8] = (unsigned char)blue;

	/*
	 * Set the checksum
	 */

	packet[9] = calculateCheck(packet);

	return;

}

int main(int argc, char *argv[]) {

	/*
	 * Four arguments. Program name and 3 color values.
	 */

	if(argc < 4) {
		printf("Wrong number of arguments.\n");
		return -1;
	}

	/*
	 * Sanitize inputs. Color values should be between 0 and 255.
	 */

	int i;
	for(i = 1;i < 4;i++) {
		if(atoi(argv[i]) < 0 || atoi(argv[i]) > 255) {
			printf("Color value(s) not in range 0-255.\n");
			return -1;
		}
	}

	int red = atoi(argv[1]);
	int green = atoi(argv[2]);
	int blue = atoi(argv[3]);

	/*
	 * Looks like we're good to go. Let's send out the data to the thing.
	 */

	applyColor(red, green, blue);

	return 0;
}
