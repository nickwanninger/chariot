#include <asm.h>
#include <fs.h>
#include <syscall.h>


static inline void a20_reboot(void) {
  /* resetting A20 should be a bit quicker */
  outb(0x92, 1);
}


int sys::shutdown(void) {

	printk(KERN_WARN "Shutting down...\n");
	printk(KERN_WARN "Flushing block cache\n");
  block::sync_all();
  /* TODO: kill everyone! */


	printk(KERN_WARN "Trying BOCHS/Old-Qemu method\n");
	/* Bochs and old QEMU */
	outw(0xB004, 0x2000);
	printk(KERN_WARN "Trying new QEMU method\n");
	/* Newer QEMU */
	outw(0x604, 0x2000);
	printk(KERN_WARN "Trying VirtualBox method\n");
	/* Virtual box */
	outw(0x4004, 0x3400);

	printk(KERN_WARN "Trying QEMU isa-debug-exit method\n");
  outb(0x501, 0x00);

	printk(KERN_ERROR "Failed to shutdown. Guess I gotta implement ACPI AML?\n");

  return -ENOTIMPL;
}
