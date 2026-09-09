#include "hrtos_hal.h"


/**
 * @brief  °´¼üÉ¨Ãè
 * @return °´¼ü±àºÅ
 */
unsigned char drv_key_scan(void)
{
    if(key1 == 0)
    {
        return KEY_1;
    }

    if(key2 == 0)
    {
        return KEY_2;
    }

    if(key3 == 0)
    {
        return KEY_3;
    }

    if(key4 == 0)
    {
        return KEY_4;
    }

    return KEY_NONE;
}