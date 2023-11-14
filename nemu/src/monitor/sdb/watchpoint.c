
#include <common.h>
#include "sdb.h"

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;
void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp() {
  if (!free_) {
    assert(0);
  } else {
    WP* temp = free_;
    free_ = free_->next;
    temp->next = head;
    head = temp;
    assert(head != NULL);

    return head;
  }
  return NULL;
}

void free_wp(WP *wp) {
  assert(wp != NULL);
  if (wp == head) {
    head = head->next;
  } else {
    WP* p = head;
    while (p->next != wp) {
      p = p->next;
    }
    p->next = p->next->next;
  }
  wp->next = free_;
  free_ = wp;
}

bool wp_changed_if() {
  WP* p = head;
  bool success = false;
  while (p) {
    word_t cur = expr(p->expr, &success);
    if (p->val != cur)
    {
      printf("Old value: %d\n", p->val);
      printf("Current value: %d\n", cur);
      p->val = cur;
      printf("\n");
      success = true;
    }
    p = p->next;
  }
  return success;
}

void watchpoint_display() {
   WP* p = head;
  int index = 0;
  while (p) {
    printf("index: %d expr: %s \n", index ++, p->expr);
    p = p->next;
  }
}

void watchpoint_delete(int i) {
  WP* p = head;
  while (p && i--) {
    p = p->next;
  }
  if (!p) assert(0);
  free_wp(p);
}