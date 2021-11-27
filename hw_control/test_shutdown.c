/**
 * @file test_shutdown.c
 * @author Esteban Campos Granados (este0111@hotmail.com)
 * @brief Testing of 'shutdown_system'.
 * @version 0.1
 * @date 2021-10-29
 */

#include <stdio.h>
#include <HWController.h>

/** @fn main
 * @brief Main execution method.
 * 
 * Test the shutdown method in the Hardware Controller library.
 * 
 * @return int 
 */

int main()
{
    return shutdown_system();
}