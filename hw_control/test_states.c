#include <stdio.h>
#include <HWController.h>

int main()
{
    printf("State #0 ...\n");
    configure_state('0');
    sleep(1);

    printf("State #1 ...\n");
    configure_state('1');
    sleep(1);

    printf("State #2 ...\n");
    configure_state('2');
    sleep(1);

    printf("State #3 ...\n");
    configure_state('3');
    sleep(1);

    printf("State #0 ...\n");
    configure_state('0');
    sleep(1);

    return 0;
}