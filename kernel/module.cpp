#include <module.h>
#include <printk.h>

void initialize_builtin_modules(void) {
  extern struct kernel_module_info __start__kernel_modules[];
  extern struct kernel_module_info __stop__kernel_modules[];
  struct kernel_module_info *mod = __start__kernel_modules;
  int i = 0;
  while (mod != __stop__kernel_modules) {
    // printk(KERN_DEBUG "Calling builtin module %s\n", mod->name);
    mod->initfn();
    // printk(KERN_DEBUG "Module '%s' done.\n", mod->name);
    mod = &(__start__kernel_modules[++i]);
  }
}
