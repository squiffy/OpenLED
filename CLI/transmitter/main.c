#include <stdio.h>

void applyColor(int red, int green, int blue) {

	printf("function stub\n.");

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
