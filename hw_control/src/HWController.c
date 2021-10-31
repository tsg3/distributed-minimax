/**
 * @file HWController.c
 * @author Esteban Campos Granados (este0111@hotmail.com)
 * @brief Hardware Controller source code.
 * @version 0.1
 * @date 2021-10-31
 */

#include <HWController.h>

/**
 * Opens the '/dev/mem' device file to have access to the FPGA's physical addresses.
 * 
 * The function returns 0 if the device file was opened succesfully; otherwise, it
 * returns -1.
 */
int open_physical(int fd)
{
    if (fd == -1)
    {
        if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1)
        {
            printf("ERROR: Couldn't open '/dev/mem'!\n");
            return -1;
        }
    }
    return fd;
}

/**
 * Closes the '/dev/mem' device file.
 */
void close_physical(int fd)
{
    close(fd);
}

/**
 * Physical-to-virtual address mapping.
 * 
 * Calls 'mmap' to map the physical address of 'fd' to a virtual address to access the 
 * I/O device.
 */
void* map_physical(int fd)
{
    void *virtual_base = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, 
        fd, HW_REGS_BASE);
    
    if (virtual_base == MAP_FAILED)
    {
        printf("ERROR: 'mmap()' failed!\n");
        close(fd);
        return NULL;
    }
    return virtual_base;
}

/**
 * Physical-to-virtual address unmapping.
 * 
 * Closes the physical-to-virtual address mapping of the I/O device.
 */
int unmap_physical(void* virtual_base)
{
    if (munmap(virtual_base, HW_REGS_SPAN) != 0)
    {
        printf ("ERROR: 'munmap()' failed!\n");
        return -1;
    }
    return 0;
}

/**
 * Sets the FPGA node's identifier.
 * 
 * It invokes 'configure_led_r' function with offset=1 to configure the FPGA's LED which
 * are used as the nodes identifier.
 */
int configure_id(char id)
{
    return configure_led_r(id, 1);
}

/**
 * Sets the execution current state.
 * 
 * It invokes 'configure_led_r' function with offset=9 to configure the FPGA's LED which
 * are used to represent the execution state of the minimax evaluation.
 */
int configure_state(char state)
{
    return configure_led_r(state, 9);
}

/**
 * Opens the virtual memory address of the I/O devices.
 * 
 * Invokes the 'open_physical' and 'map_physical' methods.
 */
int opening_space(int* fd, void** LW_virtual)
{
    if ((*fd = open_physical(*fd)) == -1)
    {
        return -1;
    }

    if ((*LW_virtual = map_physical(*fd)) == NULL)
    {
        return -1;
    }

    return 0;
}

/**
 * Closes the virtual memory address of the I/O devices.
 * 
 * Invokes the 'close_physical' and 'unmap_physical' methods.
 */
int closing_space(int* fd, void** LW_virtual)
{
    if (unmap_physical(*LW_virtual) == -1)
    {
        return -1;
    }

    close_physical(*fd);
    return 0;
}

/**
 * LED configuration method.
 * 
 * This function calls 'opening_space' to open the '/dev/mem' device file and map the base
 * virtual address. Then, it calls 'toggle_led_r_bits' to configure the bits in positions
 * ['offset':'offset'-1] with the 2-bit value given.
 * Finally, unmaps the virtual address and closes the file descriptor.
 */
int configure_led_r(char value, int offset)
{
    volatile int* LEDR_ptr;
    int fd = -1;
    void* LW_virtual;

    if (opening_space(&fd, &LW_virtual) == -1)
    {
        return -1;
    }

    LEDR_ptr = (unsigned int*)(LW_virtual + (LED_PIO_BASE & HW_REGS_MASK));

    toggle_led_r_bits(LEDR_ptr, value, offset);

    return closing_space(&fd, &LW_virtual);
}

/**
 * Updates bits on LEDs register.
 * 
 * This function sets or clears bits on 'LEDR_ptr' memory in the positions 'offset' and
 * 'offset'-1 with the second and first bits of the value given, respectively.
 */
void toggle_led_r_bits(volatile int* LEDR_ptr, char value, int offset)
{
    *LEDR_ptr = BIT_CLEAR(*LEDR_ptr, offset);
    *LEDR_ptr = BIT_CLEAR(*LEDR_ptr, offset - 1);

    if (BIT_CHECK(value, 0) == 1)
    {
        *LEDR_ptr = BIT_SET(*LEDR_ptr, offset - 1);
    }

    if (BIT_CHECK(value, 1) == 1)
    {
        *LEDR_ptr = BIT_SET(*LEDR_ptr, offset);
    }
}

/**
 * Calls "sudo shutdown -P now" to halt the system.
 */
int shutdown_system()
{
    system("sudo shutdown -P now");
    return 0;
}

/**
 * Shutdown checking.
 * 
 * Opens the '/dev/mem' device file and maps the base virtual address. Then, it checks
 * the LED4's value configured by the custom kernel module. Finally, unmaps the virtual 
 * address, closes the file descriptor and returns the value read.
 */
int check_shutdown()
{
    volatile int* LEDR_ptr;
    int fd = -1;
    void* LW_virtual;

    if (opening_space(&fd, &LW_virtual) == -1)
    {
        return -1;
    }

    LEDR_ptr = (unsigned int*)(LW_virtual + (LED_PIO_BASE & HW_REGS_MASK));
    int res = BIT_CHECK(*LEDR_ptr, 4);

    if (closing_space(&fd, &LW_virtual) == -1)
    {
        return -1;
    }

    return res;
}

/**
 * Continue execution waiting.
 * 
 * Opens the '/dev/mem' device file and maps the base virtual address. Then, it 
 * continuously checks LED5's value configured by the kernel module implemented, until
 * the value is equal to '1'. Finally, unmaps the virtual addressa and closes the file 
 * descriptor.
 */
int wait_continue()
{
    volatile int* LEDR_ptr;
    int fd = -1;
    void* LW_virtual;

    if (opening_space(&fd, &LW_virtual) == -1)
    {
        return -1;
    }

    LEDR_ptr = (unsigned int*)(LW_virtual + (LED_PIO_BASE & HW_REGS_MASK));
    while (BIT_CHECK(*LEDR_ptr, 5) != 1)
    {
        usleep(500);
    }

    return closing_space(&fd, &LW_virtual);
}