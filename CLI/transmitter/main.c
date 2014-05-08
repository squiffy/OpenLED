#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

union Packet {
	struct {
		uint8_t magic[3];
		uint8_t mode;
		uint8_t keyNumber;
		uint8_t keyValue;
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		uint8_t checksum;
	} fields;

	uint8_t data[10];
};

union Halfword {
	struct {
		unsigned int low:8;
		unsigned int high:8;
	} bytes;

	uint16_t hword_value;
};

union byte {
	struct {
		unsigned int low:4;
		unsigned int high:4;
	} nibbles;

	uint8_t byte_value;
};

void transmitPacket(union Packet packet) {

	/*
	 * Need to scamble the packet first. I hope this works...
	 */

	int lowCounter = 0;
	int highCounter = 9;

	union byte lowByte;
	union byte highByte;
	union byte tempByte;

	bool done = false;
	while(!done) {

		lowByte.byte_value = packet.data[lowCounter];
		highByte.byte_value = packet.data[highCounter];

		tempByte.byte_value = lowByte.byte_value;
		lowByte.nibbles.low = highByte.nibbles.high;
		highByte.nibbles.high = tempByte.nibbles.low;

		packet.data[lowCounter] = lowByte.byte_value;
		packet.data[highCounter] = highByte.byte_value;

		lowCounter++;
		highCounter--;

		if(lowCounter == 5)
			done = true;

	}

	/*
	 * Print the packet out for now. At least I implement aync sockets. However,
	 * you can just shove this over netcat. Should work.
	 */

	printf("Packet: ");

	int i;
	for(i = 0;i < 10; i++) {

		printf("%02x",packet.data[i]);

	}

	printf("\n");
}

unsigned char calculateCheck(union Packet packet) {

	union Halfword sum;

	sum.hword_value = 0x0000;

	int i;
	for(i = 0;i < 10; i++)
		sum.hword_value += packet.data[i];

	unsigned char ret = (unsigned char)(sum.bytes.high + sum.bytes.low);

	return ret;
}

void applyColor(int red, int green, int blue) {

	/*
	 * XXX: eventually make the packet a struct
	 */

	union Packet packet;

	/*
	 * set magic bytes
	 */

	packet.fields.magic[0] = 0xaa;
	packet.fields.magic[1] = 0x55;
	packet.fields.magic[2] = 0x00;

	/*
	 * Set mode byte. 0x01 since we're only changing colors
	 */

	packet.fields.mode = 0x01;

	/*
	 * Set keyNumber to 0x05 to change colors.
	 */

	packet.fields.keyNumber = 0x05;

	/*
	 * Set keyValue to brightness value. 0x32 for 50% brightness.
	 */

	packet.fields.keyValue = 0x32;

	/*
	 * Now we set the colors! RGB format
	 */

	packet.fields.red = (uint8_t)red;
	packet.fields.green = (uint8_t)green;
	packet.fields.blue = (uint8_t)blue;

	/*
	 * Set the checksum
	 */

	packet.fields.checksum = calculateCheck(packet);

	transmitPacket(packet);

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
