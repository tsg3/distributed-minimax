#include <stdio.h>
#include <HWController.h>

int main()
{
    wait_continue();
    printf("¿Shutdown? = %d\n", check_shutdown());
    return 0;
}