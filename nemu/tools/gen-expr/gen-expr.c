#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";
int ind = 0;
void gen(char c) {
  buf[ind ++] = c;
  if (ind >= 65536) 
    assert(0);
}

void gen_num() {
  int random_number = rand() % 100;
  if (random_number > 9) {
    buf[ind ++] = '0' + random_number / 10;
    buf[ind ++] = '0' + random_number % 10;
    if (ind >= 65536) 
      assert(0);
  } else {
    buf[ind ++] = '0' + random_number % 10;
    if (ind >= 65536) 
      assert(0);
  }
}

void gen_rand_op() {
  char ops[4] = {'-', '+', '*', '/'};
  int in = rand() % 4;
  buf[ind ++] = ops[in];
  if (ind >= 65536) 
    assert(0);
}

static void gen_rand_expr(int i) {
  if (i == 0) {
    gen_num();
    return ;
  }
  switch (rand() % 3) {
    case 0: gen_num(); break;
    case 1: gen('('); gen_rand_expr(i-1); gen(')'); break;
    default: gen_rand_expr(i-1); gen_rand_op(); gen_rand_expr(i-1); break;
  }
}
int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    ind = 0;
    gen_rand_expr(5);
    buf[ind] = '\0';
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
