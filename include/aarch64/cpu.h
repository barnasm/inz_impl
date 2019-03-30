#ifndef ARMCPU_H
#define ARMCPU_H

#include <aarch64/cpureg.h>

static inline unsigned arm_cpu_id(void) {
  return reg_mpidr_el1_read() & MPIDR_AFF0;
}

#if 0
static inline uint32_t arm_set_cpsr_c(uint32_t clr, uint32_t eor) {
  uint32_t tmp, ret;
  
  __asm__ volatile(//"mrs %0, cpsr\n"   /* Get the CPSR */
                   "bic %1, %0, %2\n" /* Clear bits */
                   "eor %1, %1, %3\n" /* XOR bits */
                   //"msr cpsr_c, %1\n" /* Set the control field of CPSR */
                   : "=&r"(ret), "=&r"(tmp)
                   : "r"(clr), "r"(eor)
                   : "memory");

  return ret;
}
#endif

#define ENABLE_INTERRUPT_ASM    \
  msr daifclr, #((DAIF_I|DAIF_F) >> DAIF_SETCLR_SHIFT)
#define DISABLE_INTERRUPT_ASM   \
  msr daifset, #((DAIF_I|DAIF_F) >> DAIF_SETCLR_SHIFT)


#define ENABLE_INTERRUPT()	daif_enable(DAIF_I|DAIF_F)
#define DISABLE_INTERRUPT()	daif_disable(DAIF_I|DAIF_F)

#define DAIF_MASK		(DAIF_D|DAIF_A|DAIF_I|DAIF_F)

typedef uint64_t register_t;
static inline void __unused
daif_enable(register_t psw)
{
	if (!__builtin_constant_p(psw)) {
		reg_daif_write(reg_daif_read() & ~psw);
	} else {
		reg_daifclr_write((psw & DAIF_MASK) >> DAIF_SETCLR_SHIFT);
	}
}

static inline register_t __unused
daif_disable(register_t psw)
{
	register_t oldpsw = reg_daif_read();
	if (!__builtin_constant_p(psw)) {
		reg_daif_write(oldpsw | psw);
	} else {
		reg_daifset_write((psw & DAIF_MASK) >> DAIF_SETCLR_SHIFT);
	}
	return oldpsw;
}


static inline void arm_irq_enable(void) {
  //arm_set_cpsr_c(DAIF_I, 0);
  ENABLE_INTERRUPT();
}

static inline void arm_irq_disable(void) {
  //arm_set_cpsr_c(DAIF_I, DAIF_I);
  DISABLE_INTERRUPT();
}

/* Data Memory Barrier */
static inline void arm_dmb(void) { __asm__ volatile("dmb" ::: "memory"); }

/* Data Synchronization Barrier */
static inline void arm_dsb(void) { __asm__ volatile("dsb" ::: "memory"); }

/* Instruction Synchronization Barrier */
static inline void arm_isb(void) { __asm__ volatile("isb" ::: "memory"); }

#endif
