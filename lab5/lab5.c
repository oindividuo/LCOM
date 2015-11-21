#include <minix/drivers.h>
#include <minix/syslib.h>

#include "test5.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {

	/* Initialize service */

	sef_startup();

	if (argc == 1) {
		print_usage(argv);
		return 0;
	} else {
		proc_args(argc, argv);
	}
	return 0;

}

static void print_usage(char *argv[]) {
	printf(
			"Usage: one of the following:\n"
					"\t service run %s -args \"init <mode> <delay>\" - switch the video adapter to the graphics mode specified in its argument \n"
					"\t service run %s -args \"square <x> <y> <size> <color>\" - change the color of pixels on the screen \n"
					"\t service run %s -args \"line <xi> <yi> <xf> <yf> <color> \" - draw a line segment on the screen \n",
					"\t service run %s -args \"xmp <xi> <yi> <xpm> \" - draw a sprite that is provided as an XPM image \n",
					"\t service run %s -args \"move <xi> <yi> <xpm> <hor> <delta> <time> \" - move a sprite that is provided as an XPM image \n",
					"\t service run %s -args \"controller \" - use VBE function 0x0, Return VBE Controller Information \n",
			argv[0], argv[0], argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {
	unsigned short mode, delay, x, y, size, xi, yi, xf, yf, hor, time;
	unsigned long color;
	char *xpm;
	short delta;


	/* check the function to test: if the first characters match, accept it */
	if (strncmp(argv[1], "init", strlen("init")) == 0) {
		if (argc != 4) {
			printf(
					"graphics: wrong no of arguments for test of test_init() \n");
			return 1;
		}
		if ((mode = parse_ulong(argv[2], 10)) == ULONG_MAX)
			return 1;
		if ((delay = parse_ulong(argv[3], 10)) == ULONG_MAX)
			return 1;

		printf("graphics:: test_init(%d, %d)\n", mode, delay);
		test_init(mode, delay);
		return 0;
	} else if (strncmp(argv[1], "square", strlen("square")) == 0) {
		if (argc != 6) {
			printf(
					"graphics: wrong no of arguments for test of test_square() \n");
			return 1;
		}

		if ((x = parse_ulong(argv[2], 10)) == LONG_MAX
				|| ((y = parse_ulong(argv[3], 10)) == LONG_MAX) || ((size =
						parse_ulong(argv[4], 10)) == LONG_MAX) || ((color =
						parse_ulong(argv[5], 10)) == LONG_MAX))
			return 1;

		printf("graphics:: test_square(%d, %d, %d, %d) \n", x, y, size, color);
		return test_square(x, y, size, color);
	} else if (strncmp(argv[1], "line", strlen("line")) == 0) {
		if (argc != 7) {
			printf(
					"graphics: wrong no of arguments for test of test_line() \n");
			return 1;
		}

		if ((xi = parse_ulong(argv[2], 10)) == LONG_MAX
				|| ((yi = parse_ulong(argv[3], 10)) == LONG_MAX) || ((xf =
								parse_ulong(argv[4], 10)) == LONG_MAX) || ((yf =
								parse_ulong(argv[5], 10)) == LONG_MAX) || ((color =
								parse_ulong(argv[6], 10)) == LONG_MAX))
		return 1;

		printf("graphics:: test_line(%d, %d, %d, %d, %d)\n", xi, yi, xf, yf,
				color);
		return test_line(xi, yi, xf, yf, color);
	} else if (strncmp(argv[1], "xpm", strlen("xpm")) == 0) {
		if (argc != 5) {
			printf(
					"graphics: wrong no of arguments for test of test_line() \n");
			return 1;
		}

		if ((xi = parse_ulong(argv[2], 10)) == LONG_MAX
				|| ((yi = parse_ulong(argv[3], 10)) == LONG_MAX))
		return 1;

		printf("graphics:: test_xpm(%d, %d)\n", xi, yi);
		return test_xpm(xi, yi, &xpm);
	} else if (strncmp(argv[1], "move", strlen("move")) == 0) {
		if (argc != 8) {
			printf(
							"graphics: wrong no of arguments for test of test_move() \n");
			return 1;
		}

		if ((xi = parse_ulong(argv[2], 10)) == LONG_MAX
				|| ((yi = parse_ulong(argv[3], 10)) == LONG_MAX) || ((hor =
						parse_ulong(argv[5], 10)) == LONG_MAX) || ((delta =
						parse_ulong(argv[6], 10)) == LONG_MAX) || ((time =
						parse_ulong(argv[7], 10)) == LONG_MAX))
			return 1;

		printf("graphics:: test_move(%d, %d, %d, %d, %d, %d)\n", xi, yi, xf, yf,
				hor, delta, time);
		return test_move(xi, yi, &xpm, hor, delta, time);
	} else if (strncmp(argv[1], "controller", strlen("controller")) == 0) {
		if (argc != 2) {
			printf(
					"graphics: wrong no of arguments for test of test_controller() \n");
			return 1;
		}

		printf("graphics:: test_controller()\n");
		return test_controller();
	} else {
		printf("graphics: non valid function \"%s\" to test\n", argv[1]);
		return 1;
	}
}

static unsigned long parse_ulong(char *str, int base) {
	char *endptr;
	unsigned long val;

	val = strtoul(str, &endptr, base);

	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtol");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("graphics: parse_ulong: no digits were found in %s \n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}

static long parse_long(char *str, int base) {
	char *endptr;
	unsigned long val;

	val = strtol(str, &endptr, base);

	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
			|| (errno != 0 && val == 0)) {
		perror("strtol");
		return LONG_MAX;
	}

	if (endptr == str) {
		printf("graphics: parse_long: no digits were found in %s \n", str);
		return LONG_MAX;
	}

	/* Successful conversion */
	return val;
}
