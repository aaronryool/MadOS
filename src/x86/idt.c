////////////////////////////////////////////////////////////////////////////////
// THE SCOTCH-WARE LICENSE (Revision 0):
// <aaronryool/gmail.com> wrote this file. As long as you retain this notice you
// can do whatever you want with this stuff. If we meet some day, and you think
// this stuff is worth it, you can buy me a shot of scotch in return
////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <machine.h>


struct idt_entry
{
    unsigned short base_lo;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_load();

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
    // The interrupt routine's base address
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    // selector
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void idt_install()
{
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = (unsigned int) &idt;
    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    // Add any new ISRs to the IDT here using idt_set_gate

    idt_load();
}
