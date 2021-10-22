#include <stdio.h>
#include <HWController.h>

int main()
{
    printf("ID #0 ...\n");
    configure_id('0');
    sleep(1);

    printf("ID #1 ...\n");
    configure_id('1');
    sleep(1);

    printf("ID #2 ...\n");
    configure_id('2');
    sleep(1);

    printf("ID #3 ...\n");
    configure_id('3');
    sleep(1);

    printf("ID #0 ...\n");
    configure_id('0');
    sleep(1);

    return 0;
}