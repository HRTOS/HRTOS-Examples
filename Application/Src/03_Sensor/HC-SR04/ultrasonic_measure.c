#include "hrtos_hal.h"

/*------------------------------------------------
 * 超声波回波时间测量
 *
 * 功能：
 *     测量超声波回波高电平持续时间
 *
 * 返回：
 *     Timer1计数值
 *
 * 注意：
 *     回波测量属于严格时序操作，
 *     测量期间关闭总中断，
 *     防止被其他中断打断。
 *------------------------------------------------*/
unsigned int drv_ultrasonic_measure(void)
{
    unsigned char ea_state;
    unsigned int count;
	u16 i;


    /* 保存当前总中断状态 */
    ea_state = EA;

    /* 关闭总中断，保护测量时序 */
    EA = 0;
	//ULTRASONIC_TX = 0;
	//_nop_(); 
	//_nop_(); 

    /* 清零Timer1 */
    TH1 = 0;
    TL1 = 0;
	ULTRASONIC_TX = 0;
	i=10;
	while(i--)_nop_();
	ULTRASONIC_TX = 1;
	i=2;
	while(i--)_nop_(); 
	
	/*_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); 
	_nop_(); */
	ULTRASONIC_TX = 0;
    /* 等待回波信号开始 */
    while(!ULTRASONIC_RX)
    {
        ;
    }


    /* 开始计时 */
    TR1 = 1;
//P1=0;ULTRASONIC_TX = 1;ULTRASONIC_RX=1;while(1);
i=1;
    /* 等待回波信号结束 */
    while(ULTRASONIC_RX&& i++)
    {
        ;
    }


    /* 停止计时 */
    TR1 = 0;


    /* 读取Timer1计数值 */
    count = ((unsigned int)TH1 << 8) | TL1;


    /* 清零Timer1 */
    TH1 = 0;
    TL1 = 0;


    /* 恢复原来的总中断状态 */
    EA = ea_state;


    return count;
}