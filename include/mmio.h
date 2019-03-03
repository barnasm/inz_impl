#ifndef MMIO_H
#define MMIO_H

#include <types.h>

extern uint8_t _kernel[];

static inline void mmio_write(uint32_t reg, uint32_t data) {
  uint32_t *ptr = (void *)(reg + _kernel);
  __asm__ volatile("str %[data], [%[reg]]"
                   :
                   : [reg] "r"(ptr), [data] "r"(data));
}

static inline uint32_t mmio_read(uint32_t reg) {
  uint32_t *ptr = (void *)(reg + _kernel);
  uint32_t data;
  __asm__ volatile("ldr %[data], [%[reg]]"
                   : [data] "=r"(data)
                   : [reg] "r"(ptr));
  return data;
}

#endif /* MMIO_H */
