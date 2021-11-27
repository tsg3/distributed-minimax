/**
 * @file HWController.h
 * @author Esteban Campos Granados (este0111@hotmail.com)
 * @brief Hardware Controller header.
 * @version 0.1
 * @date 2021-10-29
 */

#ifndef HWCONTROLLER_H_
#define HWCONTROLLER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdbool.h>

/** @def BIT_SET
 * @brief Sets bit in an integer.
 * 
 * @param n integer number
 * @param b bit position
 * @return int
 * 
 * This sets the bit on the position \p b of the integer \p n .
 */

/** @def BIT_CLEAR
 * @brief Clears bit in an integer.
 * 
 * @param n integer number
 * @param b bit position
 * @return int
 * 
 * This clears the bit on the position \p b of the integer \p n .
 */

/** @def BIT_CHECK
 * @brief Checks bit in an integer.
 * 
 * @param n integer number
 * @param b bit position
 * @return bool
 * 
 * This checks the bit on the position \p b of the integer \p n .
 */

#define BIT_SET(n, b) ((n) |= (1ULL << (b)))
#define BIT_CLEAR(n, b) ((n) &= ~(1ULL << (b)))
#define BIT_CHECK(n, b) (!!((n >> b) & 1ULL))

/** @def HW_REGS_BASE
 * @brief Hardware registers' base address.
 */

/** @def HW_REGS_SPAN
 * @brief Virtual memory address span.
 */

/** @def HW_REGS_MASK
 * @brief Mask to access the Hardware registers.
 */

/** @def LED_PIO_BASE
 * @brief PIO LED's base address.
 */

#define HW_REGS_BASE ( 0xff200000 )
#define HW_REGS_SPAN ( 0x00200000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )
#define LED_PIO_BASE 0x0

/** @fn open_physical
 * @brief Opens the memory device file.
 * 
 * @param fd memory device file descriptor
 * @return int; 0 if success, otherwise -1
 * 
 * Opens '/dev/mem' device file to have access to the FPGA's physical addresses.
 */

/** @fn close_physical
 * @brief Closes the memory device file.
 * 
 * @param fd memory device file descriptor
 * @return int; 0 if success, otherwise -1
 * 
 * Closes '/dev/mem' device file.
 */

int open_physical(int);
void close_physical(int);

/** @fn map_physical
 * @brief Physical-to-virtual address mapping.
 * 
 * @param fd memory device file descriptor
 * @return virtual base memory address
 * 
 * Calls 'mmap' to map the physical address of \p fd to a virtual address to access the 
 * I/O device.
 */

/** @fn unmap_physical
 * @brief Physical-to-virtual address unmapping.
 * 
 * @param fd memory device file descriptor
 * @return int; 0 if success, otherwise -1
 * 
 * Closes the physical-to-virtual address mapping of the I/O device.
 */

void* map_physical(int);
int unmap_physical(void*);

/** @fn configure_id
 * @brief Sets the FPGA node's identifier.
 * 
 * @param id char identifier
 * @return int; 0 if success, otherwise -1
 * 
 * Invokes 'configure_led_r' to set the FPGA identifier.
 */

/** @fn configure_state
 * @brief Sets the execution current state.
 * 
 * @param state char state
 * @return int; 0 if success, otherwise -1
 * 
 * Invokes 'configure_led_r' to set the FPGA current execution state.
 */

int configure_id(char);
int configure_state(char);

/** @fn opening_space
 * @brief Opens the virtual memory address of the I/O devices.
 * 
 * @param fd memory device file descriptor
 * @param LW_virtual virtual base memory address
 * @return int; 0 if success, otherwise -1 
 * 
 * Wraps 'open_physical' and 'map_physical' methods.
 */

/** @fn closing_space
 * @brief Closes the virtual memory address of the I/O devices.
 * 
 * @param fd memory device file descriptor
 * @param LW_virtual virtual base memory address
 * @return int; 0 if success, otherwise -1 
 * 
 * Wraps 'close_physical' and 'unmap_physical' methods.
 */

int opening_space(int*, void**);
int closing_space(int*, void**);

/** @fn configure_led_r
 * @brief LED configuration.
 * 
 * @param value char value from '0' to '3'
 * @param offset bit offset
 * @return int; 0 if success, otherwise -1 
 * 
 * Opens the '/dev/mem' device file and maps the base virtual address. Then, it calls
 * 'toggle_led_r_bits' to configure the bits in positions [ \p offset : \p offset - 1]
 * with \p value .
 * Finally, unmaps the virtual address and closes the file descriptor.
 */

/** @fn toggle_led_r_bits
 * @brief Updates bits on LEDs register.
 * 
 * @param LEDR_ptr leds register virtual base memory address
 * @param value char value from '0' to '3'
 * @param offset bit offset
 * @return int; 0 if success, otherwise -1 
 * 
 * Updates \p LEDR_ptr memory bits in positions \p offset and \p offset - 1 with the
 * second and first bits of \p value , respectively.
 */

int configure_led_r(char, int);
void toggle_led_r_bits(volatile int*, char, int);

/** @fn shutdown_system
 * @brief System shutdown.
 * 
 * @return int; 0 if success, otherwise -1 
 * 
 * Calls "sudo shutdown -P now" to halt the system.
 */

/** @fn check_shutdown
 * @brief Shutdown checking.
 * 
 * @return LED4's value
 * 
 * Opens the '/dev/mem' device file and maps the base virtual address. Then, it checks
 * the LED4's value configured by the custom kernel module. Finally, unmaps the virtual 
 * address, closes the file descriptor and returns the value read.
 */

/** @fn wait_continue
 * @brief Continue execution waiting.
 * 
 * @return int; 0 if success, otherwise -1
 * 
 * Opens the '/dev/mem' device file and maps the base virtual address. Then, it 
 * continuously checks LED5's value configured by the kernel module implemented, until
 * the value is equal to '1'. Finally, unmaps the virtual addressa and closes the file 
 * descriptor.
 */

int shutdown_system();
int check_shutdown();
int wait_continue();

#endif