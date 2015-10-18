#include <minix/drivers.h>
#include <minix/syslib.h>

#include "test3.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {

  /* Initialize service */

  sef_startup();

  if ( argc == 1 ) {
      print_usage(argv);
      return 0;
  } else {
      proc_args(argc, argv);
  }
  return 0;

}

static void print_usage(char *argv[]) {
  printf("Usage: one of the following:\n"
	 "\t service run %s -args \"scan <arg>\" - read the scancodes from the KBC \n"
	 "\t service run %s -args \"leds <n, toggle>\" - changes the state of the keyboard indicator Leds \n"
	 "\t service run %s -args \"timed_scan <n>\" - handles interrupts from more than one device \n",
	 argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {

  unsigned long arg, n, toogle;

  /* check the function to test: if the first characters match, accept it */
  if (strncmp(argv[1], "scan", strlen("scan")) == 0) {
	  if( argc != 3 ) {
		  printf("keyboard: wrong no of arguments for test of kbd_test_scan() \n");
		  return 1;
	  }
	  if( (timer = parse_ulong(argv[2], 10)) == ULONG_MAX )
	  		  return 1;
	  printf("keyboard:: kbd_test_scan(%lu)\n", arg);
	  return  kbd_test_scan(arg);
  } else if (strncmp(argv[1], "leds", strlen("leds")) == 0) {
	  if( argc != 3 ) {
		  printf("keyboard: wrong no of arguments for test of kbd_test_leds() \n");
		  return 1;
	  }
	  if( (freq = parse_ulong(argv[2], 10)) == ULONG_MAX )
		  return 1;
	  printf("keyboard:: kbd_test_scan() \n", freq); //completar
	  return kbd_test_leds(n, toogle);
  } else if (strncmp(argv[1], "timed_scan", strlen("timed_scan")) == 0) {
	  if( argc != 3 ) {
		  printf("keyboard: wrong no of arguments for test of kbd_test_timed_scan() \n");
		  return 1;
	  }
	  if( (time = parse_ulong(argv[2], 10)) == ULONG_MAX )
		  return 1;
	  printf("keyboard:: kbd_test_timed_scan(%lu)\n", n);
	  return kbd_test_timed_scan(n);
  } else {
	  printf("timer: non valid function \"%s\" to test\n", argv[1]);
	  return 1;
  }
}

static unsigned long parse_ulong(char *str, int base) {
  char *endptr;
  unsigned long val;

  val = strtoul(str, &endptr, base);

  if ((errno == ERANGE && val == ULONG_MAX )
	  || (errno != 0 && val == 0)) {
	  perror("strtol");
	  return ULONG_MAX;
  }

  if (endptr == str) {
	  printf("keyboard: parse_ulong: no digits were found in %s \n", str);
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
	  printf("keyboard: parse_long: no digits were found in %s \n", str);
	  return LONG_MAX;
  }

  /* Successful conversion */
  return val;
}
