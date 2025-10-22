/***************************************************************************
 **
 **    Common definition for IAR EW ARM
 **
 **    Used with ARM IAR C/C++ Compiler and Assembler.
 **
 **    (c) Copyright IAR Systems 2006
 **
 **    $Revision: 57747 $
 **
 ***************************************************************************/
#include <intrinsics.h>
#include <stdint.h>

#ifndef __ARM_COMM_DEF_H
#define __ARM_COMM_DEF_H

#define MHZ           *1000000l
#define KHZ           *1000l
#define HZ            *1l

#ifndef FALSE
#define FALSE (1 == 0)
#endif

#ifndef TRUE
#define TRUE  (1 == 1)
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

typedef unsigned int          Boolean;  // Boolean
typedef unsigned int        * pBoolean;

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define _2BL(a)   (uint8_t)(a),(uint8_t)(a>>8)
#define _2BB(a)   (uint8_t)(a>>8),(uint8_t)(a),
#define _3BL(a)   (uint8_t)(a),(uint8_t)(a>>8),(uint8_t)(a>>16)
#define _3BB(a)   (uint8_t)(a>>16),(uint8_t)(a>>8),(uint8_t)(a)
#define _4BL(a)   (uint8_t)(a),(uint8_t)(a>>8),(uint8_t)(a>>16),(uint8_t)(a>>24)
#define _4BB(a)   (uint8_t)(a>>24),(uint8_t)(a>>16),(uint8_t)(a>>8),(uint8_t)(a)

typedef void * (*CommUserFpnt_t)(void *);
typedef void   (*VoidFpnt_t)(void);

// Atomic exchange of data between a memory cell and a register
// return value of the memory cell
#if __CORE__ < 7
inline __arm uint32_t AtomicExchange (uint32_t State, uint32_t * Flag)
{
  asm("swp  r0, r0, [r1]");
  return(State);
}

#define IRQ_FLAG        0x80
#define FIQ_FLAG        0x40

inline __arm uint32_t EntrCritSection(void)
{
unsigned long tmp;
  tmp = __get_CPSR();
  __set_CPSR(tmp | IRQ_FLAG);
  return(tmp);
}

inline __arm void ExtCritSection(uint32_t Save)
{
unsigned long tmp;
  tmp = __get_CPSR();
  __set_CPSR(tmp & (Save | ~IRQ_FLAG));
}

inline __arm uint32_t EntrCritSectionFiq(void)
{
unsigned long tmp;
  tmp = __get_CPSR();
  __set_CPSR(tmp | (IRQ_FLAG | FIQ_FLAG));
  return(tmp);
}

inline __arm void ExtCritSectionFiq(uint32_t Save)
{
unsigned long tmp;
  tmp = __get_CPSR();
  __set_CPSR(tmp & (Save | ~(IRQ_FLAG | FIQ_FLAG)));
}

#define ENTR_CRT_SECTION(Save) Save = EntrCritSection()
#define EXT_CRT_SECTION(Save)  ExtCritSection(Save)

#define ENTR_CRT_SECTION_F(Save) Save = EntrCritSectionFiq()
#define EXT_CRT_SECTION_F(Save)  ExtCritSectionFiq(Save)

#elif  ((__CORE__ == __ARM6M__) || (__CORE__ == __ARM6SM__) || (__CORE__ == __ARM7M__) || (__CORE__ == __ARM7EM__))

extern volatile uint32_t CriticalSecCntr;

inline void EntrCritSection(void)
{
  if(CriticalSecCntr == 0)
  {
    asm("CPSID i");
  }
  // avoid lost of one count in case of simultaneously calling from both places
  ++CriticalSecCntr;
}

inline void ExtCritSection(void)
{
  if(--CriticalSecCntr == 0)
  {
    asm("CPSIE i");
  }
}

inline uint32_t AtomicExchange (uint32_t State, uint32_t * Flag)
{
uint32_t Hold;
  EntrCritSection();
  Hold = *Flag;
  *Flag = State;
  ExtCritSection();
  return(Hold);
}

#define ENTR_CRT_SECTION() EntrCritSection()
#define EXT_CRT_SECTION()  ExtCritSection()
#endif

#define LongToBin(n) (((n >> 21) & 0x80) | \
                      ((n >> 18) & 0x40) | \
                      ((n >> 15) & 0x20) | \
                      ((n >> 12) & 0x10) | \
                      ((n >>  9) & 0x08) | \
                      ((n >>  6) & 0x04) | \
                      ((n >>  3) & 0x02) | \
                      ((n      ) & 0x01))

#define __BIN(n) LongToBin(0x##n##l)

#define BIN8(n)                       __BIN(n)
#define BIN(n)                        __BIN(n)
#define BIN16(b1,b2)        ((        __BIN(b1)  <<  8UL) + \
                                      __BIN(b2))
#define BIN32(b1,b2,b3,b4) ((((uint32_t)__BIN(b1)) << 24UL) + \
                            (((uint32_t)__BIN(b2)) << 16UL) + \
                            (((uint32_t)__BIN(b3)) <<  8UL) + \
                              (uint32_t)__BIN(b4))

#endif // __ARM_COMM_DEF_H
