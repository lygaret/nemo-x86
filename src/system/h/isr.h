#ifndef __system_isr_h
#define __system_isr_h

#include <stdlib/stdlib.h>

typedef struct {
  uint32_t ds;                                     // data segment selector
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // pusha
  uint32_t isrno, errcode;                         // wrapper args
  uint32_t eip, cs, eflags, useresp, ss;           // pushed by cpu on interupt
} __attribute__((packed)) isr_registers_t;

typedef struct {
  uint16_t base_low;
  uint16_t selector;
  uint8_t  always0;
  uint8_t  flags;
  uint16_t base_high;
} __attribute__((packed)) idt_entry_t;

typedef void(*isr_handler_t)(isr_registers_t *);

void enable_interrupts();
void disable_interrupts();

void isr_install();
void isr_dispatch(isr_registers_t * regs);
void isr_set_handler(unsigned int num, isr_handler_t handler);
void irq_set_handler(unsigned int irq, isr_handler_t handler);

extern idt_entry_t idt[];

extern void isr_0();
extern void isr_1();
extern void isr_2();
extern void isr_3();
extern void isr_4();
extern void isr_5();
extern void isr_6();
extern void isr_7();
extern void isr_8();
extern void isr_9();
extern void isr_10();
extern void isr_11();
extern void isr_12();
extern void isr_13();
extern void isr_14();
extern void isr_15();
extern void isr_16();
extern void isr_17();
extern void isr_18();
extern void isr_19();
extern void isr_20();
extern void isr_21();
extern void isr_22();
extern void isr_23();
extern void isr_24();
extern void isr_25();
extern void isr_26();
extern void isr_27();
extern void isr_28();
extern void isr_29();
extern void isr_30();
extern void isr_31();
extern void isr_32();
extern void isr_33();
extern void isr_34();
extern void isr_35();
extern void isr_36();
extern void isr_37();
extern void isr_38();
extern void isr_39();
extern void isr_40();
extern void isr_41();
extern void isr_42();
extern void isr_43();
extern void isr_44();
extern void isr_45();
extern void isr_46();
extern void isr_47();

#endif
