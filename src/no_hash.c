
#include <stdio.h>
#include <string.h>
#include <time.h>

long long tdiff(struct timespec clock1, struct timespec clock2)
{
  clock2.tv_sec -= clock1.tv_sec;
  clock2.tv_nsec -= clock1.tv_nsec;
  if (clock2.tv_nsec < 0) {
    clock2.tv_sec -= 1;
    clock2.tv_nsec += 1000000000;
  }
  return 1000000000*clock2.tv_sec + clock2.tv_nsec;
}

extern const char list[100000][2][200];

int
main()
{
  const char *data = NULL;
  struct timespec clock1, clock2;

  clock_gettime(CLOCK_MONOTONIC, &clock1);
  for (size_t i = 0; i < (sizeof(list)/sizeof(list[0])); i++) {
    if (!strcmp(list[i][0], "SutNmukuaOWUZJaHtaayfBrwXkEvyURBdwiTXneTIvsVSkPlhWbYBxRmUhWaFnbdWqyAnqsxXqifoZsJIDiFqWVvlBrilzUbqcgR")) {
      data = list[i][1];
      break;
    }
  }
  clock_gettime(CLOCK_MONOTONIC, &clock2);
  printf("%8lli ms\n", tdiff(clock1, clock2)/1000000);

  if (data != NULL) {
    printf("%s\n", data);
  } else {
    printf("Not Found\n");
  }
}
