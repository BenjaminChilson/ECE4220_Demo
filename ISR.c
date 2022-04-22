#ifndef MODULE
#define MODULE
#endif
#ifndef __KERNEL__
#define __KERNEL__
#endif

#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");

//LED constants
#define kALL_LEDS_SELECT 0x9240
#define kALL_LEDS 0x3C
#define kRED 0x4
#define kYELLOW 0x8
#define kGREEN 0x10
#define kBLUE 0x20

//Button constants
#define kALL_BUTTONS 0x1F0000
#define kBUTTON1 0x10000
#define kBUTTON2 0x20000
#define kBUTTON3 0x40000
#define kBUTTON4 0x80000
#define kBUTTON5 0x100000

int mydev_id;	// variable needed to identify the handler

// Interrupt handler function.
static irqreturn_t button_isr(int irq, void *dev_id)
{
	// Make sure interrupt cannot be further interrupted
	disable_irq_nosync(79);

	enable_irq(79);		// re-enable interrupt before exiting ISR
	
    return IRQ_HANDLED;
}

int init_module()
{
	int dummy = 0;

	// Map GPIO registers
	
	// Both FSEL1 and FSEL2 needed to cover all 5 buttons
	unsigned long * GPFSEL1 = (unsigned long *) ioremap(0x3F200004, 4096);
	unsigned long * GPFSEL2 = (unsigned long *) ioremap(0x3F200008, 4096);
	unsigned long * GPFSEL0 = (unsigned long *) ioremap(0x3F200000, 4096);
	
	iowrite32(0x9240, GPFSEL0);
	
	// GPAREN0 allows for rising edge detection activation of desired buttons
	unsigned long * GPAREN0 = (unsigned long *) ioremap(0x3F20007C, 4096);
	
	// Both GPPUD and GPPUDCLK0 needed to configure pull-downs
	/*
	unsigned long * GPPUD = (unsigned long *) ioremap(0x3F200094, 4096);
	unsigned long * GPPUDCLK0 = (unsigned long *) ioremap(0x3F200098, 4096);
	*/
	
	// Use FSEL1 and FSEL2 to configure all buttons as inputs
	iowrite32(0, GPFSEL1);
	iowrite32(0, GPFSEL2);
	
	// Configure the pull-downs for all relevant ports
	/*
	iowrite32(kALL_BUTTONS, GPPUD);
	udelay(100);
	iowrite32(kALL_BUTTONS, GPPUDCLK0);
	udelay(100);
	*/
		
	// Enable (Async) Rising Edge detection for all 5 GPIO ports.
	iowrite32(kALL_BUTTONS, GPAREN0);
			
	// Request the interrupt / attach handler (line 79, doesn't match the manual...)
	// The third argument string can be different
	dummy = request_irq(79, button_isr, IRQF_SHARED, "Button_handler", &mydev_id);

	return 0;
}

// Cleanup - undo whatever init_module did
void cleanup_module()
{
	// Good idea to clear the Event Detect status register here
	
	unsigned long * GPPUDCLK0 = (unsigned long *) ioremap(0x3F200098, 4096);
	unsigned long * GPPUD = (unsigned long *) ioremap(0x3F200094, 4096);
	unsigned long * GPAREN0 = (unsigned long *) ioremap(0x3F200040, 4096);
	
	iowrite32(0, GPPUDCLK0);
	iowrite32(0, GPPUD);
	
	// Disable (Async) Rising Edge detection for all 5 GPIO ports.
	iowrite32(0, GPAREN0);
	
	// Remove the interrupt handler
    free_irq(79, &mydev_id);
}





















































