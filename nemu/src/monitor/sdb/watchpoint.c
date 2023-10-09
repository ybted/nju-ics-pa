
#include <common.h>
#include "sdb.h"

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
    //if (head)
    //  printf("YB !!\n");
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