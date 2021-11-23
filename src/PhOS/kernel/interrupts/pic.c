#include "pic.h"

#include "kernel/io/io_access.h"

#define ICW1_ICW4       0x01 /* ICW4 (not) needed */
#define ICW1_SINGLE     0x02 /* Single (cascade) mode */
#define ICW1_INTERVAL4  0x04 /* Call address interval 4 (8) */
#define ICW1_LEVEL      0x08 /* Level triggered (edge) mode */
#define ICW1_INIT       0x10 /* Initialization - required! */

#define ICW4_8086       0x01 /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO       0x02 /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE  0x08 /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C /* Buffered mode/master */
#define ICW4_SFNM       0x10 /* Special fully nested (not) */

#define PIC_READ_IRR    0x0a
#define PIC_READ_ISR    0x0b

void picRemap(int32_t pic_master_offset, int32_t pic_slave_offset)
{
    unsigned char a1 = inb(PIC1_DATA);
    unsigned char a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);

    ioWait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    ioWait();
    outb(PIC1_DATA, pic_master_offset);
    ioWait();
    outb(PIC2_DATA, pic_slave_offset);
    ioWait();
    outb(PIC1_DATA, 4);

    ioWait();
    outb(PIC2_DATA, 2);
    ioWait();

    outb(PIC1_DATA, ICW4_8086);
    ioWait();
    outb(PIC2_DATA, ICW4_8086);
    ioWait();

    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}
void picDisable(void)
{
    outb(0xa1, 0xff);
    outb(0x21, 0xff);
}
void picSendEOI(byte_t irq)
{
    if (irq >= 8) outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}
void picSetIMR(byte_t irq_line)
{
    word_t port;
    byte_t value;

    if (irq_line < 8) port = PIC1_DATA;
    else
    {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    value = inb(port) | (1 << irq_line);
    outb(port, value);
}
void picClearIMR(byte_t irq_line)
{
    word_t port;
    byte_t value;

    if (irq_line < 8) port = PIC1_DATA;
    else
    {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    value = inb(port) & ~(1 << irq_line);
    outb(port, value);
}

static word_t __getIRQ_Register(int32_t ocw3)
{
    outb(PIC1_COMMAND, ocw3);
    outb(PIC2_COMMAND, ocw3);
    return (inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND);
}

// Interrupt Request Register
word_t picGetIRR(void) { return __getIRQ_Register(PIC_READ_IRR); }
// Interrupt In Service Register
word_t picGetISR(void) { return __getIRQ_Register(PIC_READ_ISR); }