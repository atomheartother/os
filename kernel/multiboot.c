#include "multiboot.h"
#include "kmem.h"

multibootModule** initModules(multiboot_info_t *mbi) {
  multibootModule** modules = kmalloc(sizeof(multibootModule*) * (mbi->mods_count + 1));
  return modules;
}
