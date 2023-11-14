#ifndef __SDB_H__
#define __SDB_H__

#include <common.h>
#define NR_WP 32
typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  /* TODO: Add more members if necessary */
  word_t val;
  char expr[100];
} WP;

word_t expr(char *e, bool *success);

WP* new_wp();
void free_wp(WP *wp);
void init_wp_pool();
bool wp_changed_if();
void watchpoint_display();
void watchpoint_delete();
#endif
