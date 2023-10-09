#ifndef __SDB_H__
#define __SDB_H__

#include <common.h>
#define NR_WP 32
word_t expr(char *e, bool *success);
typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  /* TODO: Add more members if necessary */
  word_t val;
  char expr[100];
} WP;
__attribute__((unused)) static WP wp_pool[NR_WP] = {};
__attribute__((unused)) static WP *head = NULL, *free_ = NULL;
WP* new_wp();
void free_wp(WP *wp);
void init_wp_pool();
#endif
