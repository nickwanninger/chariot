#include <process.h>
#include <cpu.h>

int sys::yield(void) {
  sched::yield();
  return 0;
}

