#include "hrtos_hal.h"


/**
 * @brief  æÿ’Ûº¸≈Ã…®√Ë
 * @return º¸÷µ 0-F
 */
unsigned char drv_matrixkey_scan(void)
{
    unsigned char key = MATRIXKEY_NONE;


    GPIO_KEY = 0x0f;

    if(GPIO_KEY != 0x0f)
    {
        key = 0;


        // œ˚∂∂
        // delay(10ms);


        // ºÏ≤‚¡–
        GPIO_KEY = 0x0f;

        switch(GPIO_KEY)
        {
            case 0x07:
                key = 0;
                break;

            case 0x0b:
                key = 1;
                break;

            case 0x0d:
                key = 2;
                break;

            case 0x0e:
                key = 3;
                break;
        }


        // ºÏ≤‚––
        GPIO_KEY = 0xf0;

        switch(GPIO_KEY)
        {
            case 0x70:
                key += 0;
                break;

            case 0xb0:
                key += 4;
                break;

            case 0xd0:
                key += 8;
                break;

            case 0xe0:
                key += 12;
                break;
        }


        // µ»¥˝ Õ∑≈
        while(GPIO_KEY != 0xf0);
    }


    return key;
}