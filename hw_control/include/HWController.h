#ifndef HWCONTROLLER_H_
#define HWCONTROLLER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define HW_REGS_BASE ( 0xff200000 )
#define HW_REGS_SPAN ( 0x00200000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )
#define LED_PIO_BASE 0x0

int open_physical(int);
void close_physical(int);

void* map_physical(int);
int unmap_physical(void*);

int configure_id(char);
int configure_state(char);
int shutdown_system();

#endif