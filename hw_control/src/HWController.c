#include <HWController.h>

int shutdown_system()
{
    system("shutdown -P now");
    return 0;
}