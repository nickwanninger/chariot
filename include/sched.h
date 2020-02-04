// A single header file for all things task and scheduler related

#pragma once

#include <func.h>
#include <task.h>


// forward declare
class thread;
class process;

namespace sched {

bool init(void);

bool enabled();

#define SCHED_MLFQ_DEPTH 10
#define PRIORITY_HIGH (SCHED_MLFQ_DEPTH - 1)
#define PRIORITY_IDLE 0

process &kernel_proc(void);

void yield(void);

void do_yield(int status);

void block();

// does not return
void run(void);

void handle_tick(u64 tick);

// force the process to exit, (yield with different state)
void exit();

void play_tone(int frq, int dur);

int add_task(struct task*);


// make a 440hz beep for 100ms
void beep();


// called before dropping back into user space. This is needed
// when a thread should not return to userspace because it must
// die or something else
void before_iret(void);
}  // namespace sched
