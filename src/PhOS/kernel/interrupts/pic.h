#ifndef PIC_H
#define PIC_H

#include "common.h"

#define PIC1         0x20
#define PIC2         0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2 + 1)
#define PIC_EOI      0x20

extern void   pic_Remap(int32_t pic_master_offset, int32_t pic_slave_offset);
extern void   pic_Disable(void);
// End of Interrupt
extern void   pic_SendEOI(byte_t irq);
extern void   pic_SetIMR(byte_t irq_line);
extern void   pic_ClearIMR(byte_t irq_line);

extern word_t pic_GetIRR(void);
extern word_t pic_GetISR(void);

#endif // PIC_H