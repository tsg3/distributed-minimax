#ifndef HWCONTROLLER_H_
#define HWCONTROLLER_H_

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int open_physical(int);
void* map_physical(int, unsigned int, unsigned int);
void close_physical(int);
int unmap_physical(void*, unsigned int);

int configure_id(char);
int configure_state(char);
int shutdown_system();

#endif