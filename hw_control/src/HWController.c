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

void close_physical (int fd)
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
    volatile int* LEDR_ptr;
    int fd = -1;
    void* LW_virtual;

    if ((fd = open_physical(fd)) == -1)
    {
        return -1;
    }

    if ((LW_virtual = map_physical(fd)) == NULL)
    {
        return -1;
    }

    LEDR_ptr = (unsigned int*)(LW_virtual + (LED_PIO_BASE & HW_REGS_MASK));
    *LEDR_ptr = *LEDR_ptr + 1;

    if (unmap_physical(LW_virtual) == -1)
    {
        return -1;
    }

    close_physical(fd);
    return 0;
}

int configure_state(char state);

int shutdown_system()
{
    system("sudo shutdown -P now");
    return 0;
}