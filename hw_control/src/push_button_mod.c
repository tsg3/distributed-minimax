/**
 * @file push_button_mod.c
 * @author Esteban Campos Granados (este0111@hotmail.com)
 * @brief Key push buttons and LED kernel module source code. 
 * @version 0.1
 * @date 2021-10-29
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/io.h>

/** \var lwbridgebase
 * @brief Memory address.
 * 
 * Virtual address used to access physical memory from the LED and buttons' registers. 
 */

void* lwbridgebase;

/** @fn irq_handler
 * @brief Interruption handler for the FPGA's push buttons.
 * 
 * @param irq interruption
 * @param dev_id device identifier
 * @param regs registers
 * @return irq_handler_t 
 * 
 * Whenever a main push button is released, this handler is called. It cleans the edge
 * capture register. Also, updates LED register for the system shutdown and execution
 * calls.
 */

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

/** @fn intitialize_pushbutton_handler
 * @brief Initialize module handler.
 * 
 * @return int 
 * 
 * Handler called when the kernel module is been initialized. 
 * Configures 'lwbridgebase' virtual address to the hardware register of the FPGA, and
 * requests the buttons IRQ.
 */

static int __init intitialize_pushbutton_handler(void)
{
    lwbridgebase = ioremap_nocache(0xff200000, 0x200000);
    iowrite32(0xf, lwbridgebase + 0x5c); 
    iowrite32(0xf, lwbridgebase + 0x58);
    
    return request_irq(73, (irq_handler_t)irq_handler,
        IRQF_SHARED, "pushbutton_irq_handler", (void*)(irq_handler));
}

/** @fn cleanup_pushbutton_handler
 * @brief Exit module handler.
 * 
 * Handler called when the kernel module is removed. It frees the buttons IRQ requested.
 */

static void __exit cleanup_pushbutton_handler(void)
{
    free_irq(73, (void*)irq_handler);
}

module_init(intitialize_pushbutton_handler);
module_exit(cleanup_pushbutton_handler);