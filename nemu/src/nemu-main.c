#include <common.h>
#include "./monitor/sdb/sdb.h"


void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif
   FILE* fp = fopen("/home/ybreeze/course/ics2021/nemu/tools/gen-expr/input", "r+");
  if (!fp) {
    printf("Failure open file!\n");
    exit(0);
  }
  char str[10000];
  while (fgets(str, 10000, fp) != NULL) {
    char exp[1000];
    int answer;
    bool success;
    sscanf(str, "%d %s", &answer, exp);
    printf("expr: %s answer: %d  mine: %d \n", exp, answer, expr(exp, &success));
  }
  
  /* Start engine. */
  engine_start();

  return is_exit_status_bad();
}
