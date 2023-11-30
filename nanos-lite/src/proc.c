#include <proc.h>

#define MAX_NR_PROC 4
extern void naive_uload(PCB *pcb, const char *filename);
static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  char* str = (char*) arg;
  while (1) {
    Log("Hello World from Nanos-lite with arg '%s' for the %dth time!", str, j);
    j ++;
    yield();
  }
}

void context_kload(PCB* pro, void (*func)(void *), void * arg) {
  Area kstack;
  kstack.end = &pro->stack[STACK_SIZE - 1];
  kstack.start = &pro->stack[0];
  Context* cp = kcontext(kstack, func, arg);
  pro->cp = cp;
}

void init_proc() {
  context_kload(&pcb[0], hello_fun, "I'm 1");
  context_kload(&pcb[1], hello_fun, "I'm 2");
  switch_boot_pcb();
  Log("Initializing processes...");
  
  // load program here

}

Context* schedule(Context *prev) {
    // save the context pointer
  current->cp = prev;

  // always select pcb[0] as the new process
  current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);

  // then return the new context
  return current->cp;
}
