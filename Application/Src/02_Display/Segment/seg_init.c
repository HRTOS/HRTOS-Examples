#include "hrtos_hal.h"


/* 共阴数码管段码 0-F */
unsigned char code seg_table_[] =
{
    0x3f, //0
    0x06, //1
    0x5b, //2
    0x4f, //3
    0x66, //4
    0x6d, //5
    0x7d, //6
    0x07, //7
    0x7f, //8
    0x6f, //9
    0x77, //A
    0x7c, //b
    0x39, //C
    0x5e, //d
    0x79, //E
    0x71  //F
};

/* 共阳数码管段码 0-F */
unsigned char code seg_table[] =
{
    0xC0, // 0
    0xF9, // 1
    0xA4, // 2
    0xB0, // 3
    0x99, // 4
    0x92, // 5
    0x82, // 6
    0xF8, // 7
    0x80, // 8
    0x90, // 9
    0x88, // A
    0x83, // b
    0xC6, // C
    0xA1, // d
    0x86, // E
    0x8E  // F
};


/**
 * @brief 数码管初始化
 */
void drv_seg_init(void)
{
    SEG_PORT = 0x00;

    SEG1 = 1;
    SEG2 = 1;
    SEG3 = 1;
    SEG4 = 1;
}