#include "../include/stdint.h"
#include "../include/stdio.h"
#include "mem.h"

void init_mem(multiboot_info_t* boot_info) {
    for (int i = 0; i < boot_info->mmap_length; i += sizeof(multiboot_memory_map_t)) {
        multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*)(boot_info->mmap_addr+i);
        uint64_t addr = ((uint64_t)mmmt->addr_high << 32) | mmmt->addr_low;
        uint64_t len = ((uint64_t)mmmt->len_high << 32) | mmmt->len_low;
        printf("addr: %lx\tlen: %lx\tsize: %x\ttype: %x\n", addr, len, mmmt->size, mmmt->type);
    }
}