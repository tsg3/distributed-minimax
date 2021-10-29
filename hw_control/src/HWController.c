/**
 * @file HWController.c
 * @author Esteban Campos Granados (este0111@hotmail.com)
 * @brief Hardware Controller source code.
 * @version 0.1
 * @date 2021-10-29
 */

#include <HWController.h>

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

void close_physical(int fd)
{
    close(fd);
}

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

int unmap_physical(void* virtual_base)
{
    if (munmap(virtual_base, HW_REGS_SPAN) != 0)
    {
        printf ("ERROR: 'munmap()' failed!\n");
        return -1;
    }
    return 0;
}

int configure_id(char id)
{
    return configure_led_r(id, 1);
}

int configure_state(char state)
{
    return configure_led_r(state, 9);
}

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

int closing_space(int* fd, void** LW_virtual)
{
    if (unmap_physical(*LW_virtual) == -1)
    {
        return -1;
    }

    close_physical(*fd);
    return 0;
}

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

int shutdown_system()
{
    system("sudo shutdown -P now");
    return 0;
}

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