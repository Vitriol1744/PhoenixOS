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

extern void   picRemap(int32_t pic_master_offset, int32_t pic_slave_offset);
extern void   picDisable(void);
// End of Interrupt
extern void   picSendEOI(byte_t irq);
extern void   picSetIMR(byte_t irq_line);
extern void   picClearIMR(byte_t irq_line);

extern word_t picGetIRR(void);
extern word_t picGetISR(void);

#endif // PIC_H