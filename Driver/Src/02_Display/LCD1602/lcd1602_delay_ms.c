#include "hrtos_hal.h"


void delay_ms(unsigned int ms)
{
    unsigned int i,j;

    for(i=ms;i>0;i--)
        for(j=100;j>0;j--);
}