#include <arch.h>
#include <cpu.h>
#include <map.h>
#include <sleep.h>
#include <string.h>
#define NIRQS 130

struct irq_registration {
  const char *name;
  void *data;
  irq::handler handler;
};

static struct irq_registration irq_handlers[NIRQS];

// install and remove handlers
int irq::install(int irq, irq::handler handler, const char *name, void *data) {
  printk(KERN_DEBUG "Register irq %d for '%s'. Data=%p\n", irq, name, data);
  if (irq < 0 || irq > NIRQS) return -1;
  irq_handlers[irq].name = name;
  irq_handlers[irq].data = data;
  irq_handlers[irq].handler = handler;
  arch::irq::enable(irq);
  return 0;
}

irq::handler irq::uninstall(int irq) {
  if (irq < 0 || irq > NIRQS) return nullptr;

  irq::handler old = irq_handlers[irq].handler;

  if (old != nullptr) {
    arch::irq::disable(irq);
    irq_handlers[irq].name = nullptr;
    irq_handlers[irq].handler = nullptr;
  }
  return old;
}

int irq::init(void) {
  if (!arch::irq::init()) return -1;
  return 0;
}

// cause an interrupt to be handled by the kernel's interrupt dispatcher
void irq::dispatch(int irq, reg_t *regs) {
  // store the current register state in the CPU for introspection
  // if (cpu::in_thread()) cpu::thread()->trap_frame = regs;

  auto *cpu = cpu::get();
  if (cpu != NULL) cpu->interrupt_depth++;

  auto handler = irq_handlers[irq].handler;
  if (handler != nullptr) {
    handler(irq, regs, irq_handlers[irq].data);
  }


  if (cpu != NULL) cpu->interrupt_depth--;
}
