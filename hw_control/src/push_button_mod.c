#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/io.h>

void* lwbridgebase;

irq_handler_t irq_handler(int irq, void* dev_id, struct pt_regs* regs)
{
    int key = ioread32(lwbridgebase + 0x5C);

    if (key == 8)
    {
        iowrite32(ioread32(lwbridgebase) | 0x20, lwbridgebase);
    }

    else if (key == 1)
    {
        iowrite32(ioread32(lwbridgebase) | 0x10, lwbridgebase);
    }

    else
    {
        iowrite32(ioread32(lwbridgebase) & ~(0x30), lwbridgebase);
    }

    iowrite32(0xf, lwbridgebase + 0x5c);
    
    return (irq_handler_t)IRQ_HANDLED;
}

static int __init intitialize_pushbutton_handler(void)
{
    lwbridgebase = ioremap_nocache(0xff200000, 0x200000);
    iowrite32(0xf, lwbridgebase + 0x5c); 
    iowrite32(0xf, lwbridgebase + 0x58);
    
    return request_irq(73, (irq_handler_t)irq_handler,
        IRQF_SHARED, "pushbutton_irq_handler", (void*)(irq_handler));
}

static void __exit cleanup_pushbutton_handler(void)
{
    free_irq(73, (void*)irq_handler);
}

module_init(intitialize_pushbutton_handler);
module_exit(cleanup_pushbutton_handler);