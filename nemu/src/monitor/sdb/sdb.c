#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
#include "utils.h"
#include "../../../include/isa.h"
#include <memory/paddr.h>

static int is_batch_mode = false;
//extern NEMUState nemu_state; 
void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  nemu_state.state = NEMU_QUIT;
  return -1;
}

static int cmd_help(char *args);
static int cmd_si(char *args);
static int cmd_info(char *args);
static int cmd_mem(char *args);
static int cmd_p(char *args);
static int cmd_w(char *args);
static int cmd_d(char *args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Execute N instructions", cmd_si },
  { "info", "Print the state of the instructions", cmd_info},
  { "x", "Print the memory in given address", cmd_mem},
  { "p", "Print the expression", cmd_p},
  { "w", "make a watchpoint based on expr", cmd_w},
  { "d", "delete watchpoints", cmd_d}
  /* TODO: Add more commands */

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_si(char *args)
{
  char *arg = strtok(NULL, " ");
  if (arg == NULL) {
    cpu_exec(1);
  } else {
    cpu_exec(atoi(arg));
  } 
  return 0;
}

static int cmd_p(char *args)
{
  char *arg = strtok(NULL, " ");
  if (arg == NULL) {
    printf("You should enter a expression!\n");
  } else {
    bool succ;
    printf("%u\n", expr(arg, &succ));
  }
  return 0;
}

static int cmd_w(char *args)
{
  char *arg = strtok(NULL, " ");
  if (arg == NULL) {
    printf("You should enter a expression!\n");
  } else {
    WP* new_w = new_wp();
    strcpy(new_w->expr, arg);
    bool succ;
    new_w->val = expr(arg, &succ);
  }
  return 0;
}

static int cmd_d(char *args)
{
  char *arg = strtok(NULL, " ");
  if (arg == NULL) {
    printf("You should enter a expression!\n");
  } else {
    int i = atoi(arg);
    WP* p = head;
    while (p && i) {
      p = p->next;
    }
    assert(p != NULL);
    free_wp(p);
  }
  return 0;
}

static int cmd_info(char *args)
{
  char *arg = strtok(NULL, " ");
  if (arg[0] == 'r') {
    isa_reg_display();
  } else if (arg[0] == 'w') {
    WP* p = head;
    int index = 0;
    while (p) {
      printf("index: %d expr: %s \n", index ++, p->expr);
      p = p->next;
    }
  } else 
  {
    printf("Please enter correct para!\n");
  } 
  return 0;
}

static int cmd_mem(char *args) 
{
  char *arg1 = strtok(NULL, " ");
  char *arg2 = strtok(NULL, " ");
  if (arg1 == NULL || arg2 == NULL) {
    printf("Please enter correct para!\n");
  }
  int n = atoi(arg1);
  paddr_t addr = 0;
  sscanf(arg2, "%x", &addr);
  for (int i = 0; i < n; i ++)
  {
    printf("0x%08x\n", paddr_read(addr + 4 * i, 4));
  } 
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
 
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }
  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
