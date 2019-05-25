/* clock.c - System timer */

#include <aarch64/cpu.h>
#include <klibc.h>
#include <rpi/irq.h>
#include <pmap.h>

#define CLK_FREQ 192000000

static uint32_t ticks = 0;
static uint64_t clk_freq = 0;

static void clock_irq(unsigned irq __unused) {
  uint64_t val = reg_cntp_cval_el0_read();
  reg_cntp_cval_el0_write(val + clk_freq);
  reg_cntp_ctl_el0_write(CNTCTL_ENABLE);

  arm_isb();
  ticks++;

  vaddr_t va = ticks * 0x00024321 + 0xffffFFFF00000000;
  paddr_t pa = 0;
  if(ticks % 4 == 0)
    pa = *((uint64_t*)va);

  pmap_kextract(va, &pa);
  printf("\t %.16p -> %.8p af=%d db=%d \t", va, pa, pmap_is_referenced(va), pmap_is_modified(va));
  printf("tick %d!\n", ticks);
}

void clock_init(void) {

  //reg_cntvct_el0_write(CLK_FREQ);
  clk_freq = reg_cntfrq_el0_read();
  reg_cntp_cval_el0_write(clk_freq );
  reg_cntp_ctl_el0_write(CNTCTL_ENABLE);

  arm_isb();

  /* Enable CP0 physical timer interrupt. */
  bcm2836_local_irq_register(BCM2836_INT_CNTPSIRQ, clock_irq);
  bcm2836_local_irq_enable(BCM2836_INT_CNTPSIRQ);

  bcm2836_local_irq_register(BCM2836_INT_CNTPNSIRQ, clock_irq);
  bcm2836_local_irq_enable(BCM2836_INT_CNTPNSIRQ);

}


