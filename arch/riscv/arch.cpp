#include <arch.h>
#include <cpu.h>
#include <riscv/arch.h>
#include <riscv/plic.h>

void arch_disable_ints(void) {
	rv::intr_off();
	
}
void arch_enable_ints(void) {
	rv::intr_on();
}


void arch_relax(void) {}

/* Simply wait for an interrupt :) */
void arch_halt() {
	asm volatile("wfi");
}

void arch_mem_init(unsigned long mbd) {}


void arch_initialize_trapframe(bool userspace, reg_t *r) {
	// auto *regs = (rv::regs*)r;
	// printk("pc: %p\n", regs->ra);
	// printk("sp: %p\n", regs->sp);
}


unsigned arch_trapframe_size(void) {
	return sizeof(rv::regs); 
}


void arch_dispatch_function(void *func, long arg) {}
void arch_sigreturn(void) {}
void arch_flush_mmu(void) {}
void arch_save_fpu(struct thread &) {}
void arch_restore_fpu(struct thread &) {}


unsigned long arch_read_timestamp(void) {
	rv::xsize_t x;
	asm volatile("csrr %0, time" : "=r"(x));
	return x;
}


struct rv::scratch &rv::get_scratch(void) {
	rv::xsize_t sscratch;
	asm volatile("csrr %0, sscratch" : "=r"(sscratch));
	return *(struct rv::scratch*)sscratch;
}

/*
 * Just offset into the cpu array with mhartid :^). I love this arch.
 * No need for bloated thread pointer bogus or nothin'
 */
struct processor_state &cpu::current(void) {
	return cpus[rv::get_scratch().hartid];
}


void cpu::switch_vm(struct thread *thd) { /* TODO: nothin' */ }

void cpu::seginit(void *local) {
	auto &sc = rv::get_scratch();
	// printk(KERN_DEBUG "initialize hart %d\n", sc.hartid);
	auto &cpu = cpu::current();
	/* zero out the cpu structure. This might be bad idk... */
	memset(&cpu, 0, sizeof(struct processor_state));

	/* Forward this so other code can read it */
	cpu.cpunum = sc.hartid;
}


/* TODO */
extern "C" void trapret(void) {}

/* TODO */
ref<mm::pagetable> mm::pagetable::create() { return nullptr; }




/* TODO: */
static mm::space kspace(0, 0x1000, nullptr);
mm::space &mm::space::kernel_space(void) {
  /* TODO: something real :) */
  return kspace;
}


/*
 * TODO: we don't need some of these, as they are only called from within
 * x86. I won't implement them as a form of rebellion against complex
 * instruction sets
 */
int arch::irq::init(void) { return 0; /* TODO: */ }

void arch::irq::eoi(int i) {
	/* Forward to the PLIC */
	rv::plic::complete(i);
}
void arch::irq::enable(int num) {
	rv::plic::enable(num, 1);
}
void arch::irq::disable(int num) {
	rv::plic::disable(num);
}
