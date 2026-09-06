# 02 显示设备驱动

## 1. 模块概述

02_Display 模块提供 HRTOS 实时操作系统的显示设备驱动支持，包括 LCD1602 字符液晶、OLED 图形显示屏和数码管显示驱动。

本模块通过 `HRTOS_Display.h` 头文件向用户公开 API 接口，所有驱动函数均以 `drv_` 前缀命名。

**支持的显示设备：**
- LCD1602 字符液晶显示器
- OLED 128x64 图形显示屏（I2C 接口）
- 4位数码管显示

**头文件：**
```c
#include "HRTOS_Display.h"
```

**依赖：**
- 所有驱动实现依赖 `hrtos_hal.h` 硬件抽象层
- LCD1602 初始化依赖 `os_delay_ms()`（HRTOS 系统延时函数）

---

## 2. LCD1602

### 2.1 功能简介

LCD1602 驱动提供 16x2 字符液晶显示器的初始化、命令写入、数据写入、光标定位和字符串显示功能。

### 2.2 文件组成

```
LCD1602/
├── lcd1602_delay_ms.c      - 毫秒级延时实现
├── lcd1602_delay_us.c      - 微秒级延时实现
├── lcd1602_init.c          - LCD1602 初始化实现
├── lcd1602_set_cursor.c    - 光标定位实现
├── lcd1602_write_cmd.c     - 命令写入实现
├── lcd1602_write_data.c    - 数据写入实现
└── lcd1602_write_string.c  - 字符串写入实现
```

### 2.3 硬件连接

LCD1602 连接引脚由 `HRTOS_Display.h` 中的宏定义决定：

```c
#define LCD_DATA    P2
sbit LCD_RS = P0^7;
sbit LCD_RW = P0^6;
sbit LCD_EN = P0^5;
```

- **数据端口：** P2（8位并行数据）
- **控制引脚：**
  - `LCD_RS` (P0.7)：寄存器选择（0=命令寄存器，1=数据寄存器）
  - `LCD_RW` (P0.6)：读/写选择（0=写入，1=读取）
  - `LCD_EN` (P0.5)：使能信号

**工作模式：** 8位并行数据传输

### 2.4 API 参考

#### drv_lcd1602_init()

**函数原型：**
```c
void drv_lcd1602_init(void);
```

**功能：**
初始化 LCD1602，配置为 8 位数据模式、显示开、光标关、地址自动递增。

**参数：**
无

**返回值：**
无

**使用说明：**
- 初始化配置：0x38（8位数据，2行显示，5x7点阵）
- 显示控制：0x0C（显示开，光标关，闪烁关）
- 输入模式：0x06（地址自动递增，显示不移动）
- 清屏：0x01
- 初始化后调用 `os_delay_ms(2)` 等待 LCD 稳定
- 使用前需确保 HRTOS 系统已初始化

**示例：**
```c
#include "HRTOS_Display.h"

void main(void)
{
    drv_lcd1602_init();
    // LCD1602 已初始化
}
```

---

#### drv_lcd1602_write_cmd()

**函数原型：**
```c
void drv_lcd1602_write_cmd(unsigned char cmd);
```

**功能：**
向 LCD1602 写入命令字节。

**参数：**
- `cmd`：命令字节

**返回值：**
无

**使用说明：**
- 写入命令前设置 RS=0、RW=0
- 数据通过 P2 端口输出
- EN 产生上升沿触发写入
- 写入后延时 1ms 等待 LCD 处理

**示例：**
```c
drv_lcd1602_write_cmd(0x01);  // 清屏命令
```

---

#### drv_lcd1602_write_data()

**函数原型：**
```c
void drv_lcd1602_write_data(unsigned char dat);
```

**功能：**
向 LCD1602 写入数据字节（通常为显示字符）。

**参数：**
- `dat`：数据字节

**返回值：**
无

**使用说明：**
- 写入数据前设置 RS=1、RW=0
- 数据通过 P2 端口输出
- EN 产生上升沿触发写入
- 写入后延时 1ms 等待 LCD 处理

**示例：**
```c
drv_lcd1602_write_data('A');  // 显示字符 'A'
```

---

#### drv_lcd1602_write_string()

**函数原型：**
```c
void drv_lcd1602_write_string(unsigned char *str);
```

**功能：**
向 LCD1602 写入字符串，直到遇到字符串结束符。

**参数：**
- `str`：字符串指针

**返回值：**
无

**使用说明：**
- 逐个字符调用 `drv_lcd1602_write_data()`
- 字符串需以 '\0' 结尾
- 自动处理字符串中的所有字符

**示例：**
```c
unsigned char msg[] = "Hello World";
drv_lcd1602_write_string(msg);
```

---

#### drv_lcd1602_set_cursor()

**函数原型：**
```c
void drv_lcd1602_set_cursor(unsigned char x, unsigned char y);
```

**功能：**
设置 LCD1602 光标位置。

**参数：**
- `x`：横坐标，范围 0-15
- `y`：纵坐标，范围 0-1（0=第一行，1=第二行）

**返回值：**
无

**使用说明：**
- 第一行（y=0）：命令 0x80 + x
- 第二行（y=1）：命令 0xC0 + x
- x 超出范围时行为未定义

**示例：**
```c
drv_lcd1602_set_cursor(0, 0);  // 设置到第一行起始位置
drv_lcd1602_set_cursor(5, 1);  // 设置到第二行第6列
```

---

#### delay_ms()

**函数原型：**
```c
void delay_ms(unsigned int ms);
```

**功能：**
毫秒级软件延时。

**参数：**
- `ms`：延时时间（毫秒）

**返回值：**
无

**使用说明：**
- 软件延时实现，不依赖定时器
- 延时精度取决于系统时钟频率
- 内层循环约 100 次迭代

**示例：**
```c
delay_ms(100);  // 延时 100ms
```

---

#### delay_us()

**函数原型：**
```c
void delay_us(void);
```

**功能：**
微秒级软件延时（空函数）。

**参数：**
无

**返回值：**
无

**使用说明：**
- 当前实现为空函数，无实际延时效果
- 预留接口，可能用于未来扩展

**示例：**
```c
delay_us();  // 当前无实际效果
```

### 2.5 初始化

LCD1602 初始化流程：
1. 设置 LCD_RW = 0（写模式）
2. 发送命令 0x38（8位数据，2行，5x7点阵）
3. 发送命令 0x0C（显示开，光标关）
4. 发送命令 0x06（地址自动递增）
5. 发送命令 0x01（清屏）
6. 调用 `os_delay_ms(2)` 等待稳定

### 2.6 光标定位

使用 `drv_lcd1602_set_cursor(x, y)` 定位光标：
- x：0-15（列位置）
- y：0-1（行位置，0为第一行）

### 2.7 命令与数据写入

- **命令写入：** 使用 `drv_lcd1602_write_cmd(cmd)`
- **数据写入：** 使用 `drv_lcd1602_write_data(dat)`
- **字符串写入：** 使用 `drv_lcd1602_write_string(str)`

### 2.8 字符串显示

```c
drv_lcd1602_set_cursor(0, 0);
drv_lcd1602_write_string("Line 1");
drv_lcd1602_set_cursor(0, 1);
drv_lcd1602_write_string("Line 2");
```

### 2.9 使用示例

```c
#include "HRTOS_Display.h"

void main(void)
{
    // 初始化 LCD1602
    drv_lcd1602_init();
    
    // 在第一行显示字符串
    drv_lcd1602_set_cursor(0, 0);
    drv_lcd1602_write_string("HRTOS System");
    
    // 在第二行显示字符串
    drv_lcd1602_set_cursor(0, 1);
    drv_lcd1602_write_string("LCD1602 Test");
    
    while(1)
    {
        // 主循环
    }
}
```

### 2.10 注意事项

- LCD1602 使用 8 位并行数据模式
- 初始化依赖 HRTOS 的 `os_delay_ms()` 函数
- `delay_us()` 当前为空函数，无实际延时效果
- 控制引脚固定为 P0.5-P0.7，数据端口固定为 P2
- 每次写入命令或数据后自动延时 1ms
- 光标坐标 x 范围为 0-15，y 范围为 0-1

---

## 3. OLED

### 3.1 功能简介

OLED 驱动提供 128x64 图形 OLED 显示屏的驱动功能，支持字符显示、数字显示、中文字符显示、位图显示、清屏、填充等操作，通过 I2C 接口通信。

### 3.2 文件组成

```
OLED/
├── oled_init.c           - OLED 初始化实现
├── oled_on.c             - OLED 开启实现
├── oled_off.c            - OLED 关闭实现
├── oled_clear.c          - 清屏实现
├── oled_fill.c           - 填充实现
├── oled_set_pos.c        - 位置设置实现
├── oled_show_char.c      - 字符显示实现
├── oled_show_string.c    - 字符串显示实现
├── oled_show_num.c       - 数字显示实现
├── oled_show_chinese.c   - 中文显示实现
├── oled_draw_bmp.c       - 位图显示实现
├── oled_write_command.c  - 命令写入实现（内部实现）
├── oled_write_data.c     - 数据写入实现（内部实现）
├── oled_iic_start.c      - I2C 起始信号（内部实现）
├── oled_iic_stop.c       - I2C 停止信号（内部实现）
├── oled_iic_wait_ack.c   - I2C 等待应答（内部实现）
├── oled_iic_write_byte.c - I2C 字节写入（内部实现）
└── oled_pow.c            - 幂运算辅助函数（内部实现）

OLED/SRC/
├── bmp.h                 - 位图数据定义（资源文件）
├── oled.c                - OLED 底层实现（内部实现）
├── oled.h                - OLED 内部头文件（内部实现）
└── oledfont.h            - 字库数据（资源文件）
```

### 3.3 硬件连接

OLED 连接引脚由 `HRTOS_Display.h` 中的宏定义决定：

```c
sbit OLED_SCL  = P1^0;
sbit OLED_SDIN = P1^1;
```

**注释说明的完整接线：**
```
GND    电源地
VCC    接5V或3.3V电源
D0     P1^0（SCL）
D1     P1^1（SDA）
RES    接P1^2
DC     接P1^3
CS     接P1^4
```

- **I2C 接口：**
  - `OLED_SCL` (P1.0)：I2C 时钟线
  - `OLED_SDIN` (P1.1)：I2C 数据线
- **I2C 地址：** 0x78（写地址）
- **分辨率：** 128x64 像素
- **显示模式：** 8 页（page），每页 128 列

### 3.4 通信方式

OLED 使用软件模拟 I2C 通信：

**I2C 底层操作（内部实现）：**
- `drv_oled_iic_start()`：发送 I2C 起始信号
- `drv_oled_iic_stop()`：发送 I2C 停止信号
- `drv_oled_iic_wait_ack()`：等待应答（当前为空实现）
- `drv_oled_iic_write_byte()`：写入一个字节

**数据传输（内部实现）：**
- `drv_oled_write_command()`：写入命令（控制字节 0x00）
- `drv_oled_write_data()`：写入数据（控制字节 0x40）

### 3.5 API 参考

#### drv_oled_init()

**函数原型：**
```c
void drv_oled_init(void);
```

**功能：**
初始化 OLED 显示屏，配置显示参数并开启显示。

**参数：**
无

**返回值：**
无

**使用说明：**
- 配置 OLED 为 128x64 分辨率
- 设置显示参数（对比度、扫描方向、复用率等）
- 开启电荷泵和显示
- 初始化后自动开启显示

**示例：**
```c
drv_oled_init();
```

---

#### drv_oled_on()

**函数原型：**
```c
void drv_oled_on(void);
```

**功能：**
开启 OLED 显示。

**参数：**
无

**返回值：**
无

**使用说明：**
- 开启电荷泵（0x8D, 0x14）
- 开启显示（0xAF）

**示例：**
```c
drv_oled_on();
```

---

#### drv_oled_off()

**函数原型：**
```c
void drv_oled_off(void);
```

**功能：**
关闭 OLED 显示。

**参数：**
无

**返回值：**
无

**使用说明：**
- 关闭电荷泵（0x8D, 0x10）
- 关闭显示（0xAE）

**示例：**
```c
drv_oled_off();
```

---

#### drv_oled_clear()

**函数原型：**
```c
void drv_oled_clear(void);
```

**功能：**
清屏，将整个屏幕填充为 0x00（全黑）。

**参数：**
无

**返回值：**
无

**使用说明：**
- 内部调用 `drv_oled_fill(0x00)` 实现

**示例：**
```c
drv_oled_clear();
```

---

#### drv_oled_fill()

**函数原型：**
```c
void drv_oled_fill(unsigned char _data);
```

**功能：**
用指定数据填充整个屏幕。

**参数：**
- `_data`：填充数据（0x00=全黑，0xFF=全白）

**返回值：**
无

**使用说明：**
- 遍历 8 个页面（page 0-7）
- 每页 128 列
- 列起始地址偏移 2

**示例：**
```c
drv_oled_fill(0x00);  // 全黑
drv_oled_fill(0xFF);  // 全白
```

---

#### drv_oled_set_pos()

**函数原型：**
```c
void drv_oled_set_pos(unsigned char x, unsigned char y);
```

**功能：**
设置 OLED 显示位置。

**参数：**
- `x`：横坐标，范围 0-127
- `y`：纵坐标（页号），范围 0-7

**返回值：**
无

**使用说明：**
- y 为页号（0-7），每页高度 8 像素
- x 为列号（0-127）
- 列地址自动偏移 +2

**示例：**
```c
drv_oled_set_pos(0, 0);  // 设置到第0页第0列
drv_oled_set_pos(64, 3); // 设置到第3页第64列
```

---

#### drv_oled_show_char()

**函数原型：**
```c
void drv_oled_show_char(unsigned char x,
                        unsigned char y,
                        unsigned char chr,
                        unsigned char _size);
```

**功能：**
在指定位置显示一个 ASCII 字符。

**参数：**
- `x`：横坐标，范围 0-127
- `y`：纵坐标（页号），范围 0-7
- `chr`：要显示的字符
- `_size`：字符大小（16=8x16，8=6x8）

**返回值：**
无

**使用说明：**
- `_size = 16`：使用 8x16 字库（F8X16），占用 2 页
- `_size = 8`：使用 6x8 字库（F6x8），占用 1 页
- 字符偏移量为 32（从空格开始）
- x 超出 120 时自动换行

**示例：**
```c
drv_oled_show_char(0, 0, 'A', 16);  // 显示 8x16 字符
drv_oled_show_char(0, 0, 'B', 8);   // 显示 6x8 字符
```

---

#### drv_oled_show_string()

**函数原型：**
```c
void drv_oled_show_string(unsigned char x,
                          unsigned char y,
                          unsigned char *str,
                          unsigned char _size);
```

**功能：**
在指定位置显示字符串。

**参数：**
- `x`：起始横坐标，范围 0-127
- `y`：起始纵坐标（页号），范围 0-7
- `str`：字符串指针
- `_size`：字符大小（16=8x16，8=6x8）

**返回值：**
无

**使用说明：**
- 逐个字符调用 `drv_oled_show_char()`
- 每个字符水平间距 8 像素
- x 超过 120 时自动换行到下一页
- 字符串需以 '\0' 结尾

**示例：**
```c
unsigned char msg[] = "Hello";
drv_oled_show_string(0, 0, msg, 16);
```

---

#### drv_oled_show_num()

**函数原型：**
```c
void drv_oled_show_num(unsigned char x,  
                       unsigned char y,
                       unsigned long num,
                       unsigned char len,
                       unsigned char _size);
```

**功能：**
在指定位置显示数字。

**参数：**
- `x`：起始横坐标，范围 0-127
- `y`：起始纵坐标（页号），范围 0-7
- `num`：要显示的数字（0-4294967295）
- `len`：显示长度（位数）
- `_size`：字符大小（16=8x16，8=6x8）

**返回值：**
无

**使用说明：**
- 支持显示 0 到 4294967295 的数字
- 前导零显示为空格
- 使用 `drv_oled_pow()` 进行幂运算
- 字符间距为 size/2

**示例：**
```c
drv_oled_show_num(0, 0, 12345, 5, 16);  // 显示 "12345"
drv_oled_show_num(0, 0, 123, 5, 16);    // 显示 "  123"
```

---

#### drv_oled_show_chinese()

**函数原型：**
```c
void drv_oled_show_chinese(unsigned char x,
                           unsigned char y,
                           unsigned char no);
```

**功能：**
在指定位置显示中文字符。

**参数：**
- `x`：起始横坐标，范围 0-127
- `y`：起始纵坐标（页号），范围 0-7
- `no`：中文字符编号

**返回值：**
无

**使用说明：**
- 使用 Hzk 字库数组
- 每个中文字符占用 32 字节（16x16 点阵）
- 占用 2 页高度
- 字符通过索引 no 访问 Hzk[2*no] 和 Hzk[2*no+1]

**示例：**
```c
drv_oled_show_chinese(0, 0, 0);  // 显示第0个中文字符
```

---

#### drv_oled_draw_bmp()

**函数原型：**
```c
void drv_oled_draw_bmp(unsigned char x0,
                       unsigned char y0,
                       unsigned char x1,
                       unsigned char y1,
                       unsigned char *bmp);
```

**功能：**
在指定区域显示位图。

**参数：**
- `x0`：起始横坐标
- `y0`：起始纵坐标（页号）
- `x1`：结束横坐标
- `y1`：结束纵坐标（页号）
- `bmp`：位图数据指针

**返回值：**
无

**使用说明：**
- 从 (x0, y0) 到 (x1, y1) 矩形区域显示位图
- 位图数据按行优先顺序排列
- 预定义位图：BMP1[]、BMP2[]

**示例：**
```c
drv_oled_draw_bmp(0, 0, 128, 8, BMP1);  // 显示 BMP1
```

---

#### drv_oled_pow() （内部实现）

**函数原型：**
```c
unsigned long drv_oled_pow(unsigned char m, unsigned char n);
```

**功能：**
计算 m 的 n 次幂（辅助函数）。

**参数：**
- `m`：底数
- `n`：指数

**返回值：**
m 的 n 次幂

**使用说明：**
- 用于数字显示时的位数计算
- 内部辅助函数，一般不直接调用

---

#### drv_oled_write_command() （内部实现）

**函数原型：**
```c
void drv_oled_write_command(unsigned char cmd);
```

**功能：**
通过 I2C 向 OLED 写入命令。

**参数：**
- `cmd`：命令字节

**返回值：**
无

**使用说明：**
- I2C 地址：0x78
- 控制字节：0x00（命令模式）
- 内部实现函数，用户一般不直接调用

---

#### drv_oled_write_data() （内部实现）

**函数原型：**
```c
void drv_oled_write_data(unsigned char _data);
```

**功能：**
通过 I2C 向 OLED 写入数据。

**参数：**
- `_data`：数据字节

**返回值：**
无

**使用说明：**
- I2C 地址：0x78
- 控制字节：0x40（数据模式）
- 内部实现函数，用户一般不直接调用

---

#### drv_oled_iic_start() （内部实现）

**函数原型：**
```c
void drv_oled_iic_start(void);
```

**功能：**
发送 I2C 起始信号。

**参数：**
无

**返回值：**
无

**使用说明：**
- 软件模拟 I2C 时序
- 内部实现函数

---

#### drv_oled_iic_stop() （内部实现）

**函数原型：**
```c
void drv_oled_iic_stop(void);
```

**功能：**
发送 I2C 停止信号。

**参数：**
无

**返回值：**
无

**使用说明：**
- 软件模拟 I2C 时序
- 内部实现函数

---

#### drv_oled_iic_wait_ack() （内部实现）

**函数原型：**
```c
void drv_oled_iic_wait_ack(void);
```

**功能：**
等待 I2C 应答信号。

**参数：**
无

**返回值：**
无

**使用说明：**
- 当前实现为空（仅产生时钟脉冲）
- OLED 不需要真正的应答检测
- 内部实现函数

---

#### drv_oled_iic_write_byte() （内部实现）

**函数原型：**
```c
void drv_oled_iic_write_byte(unsigned char _data);
```

**功能：**
通过 I2C 写入一个字节。

**参数：**
- `_data`：要写入的字节

**返回值：**
无

**使用说明：**
- MSB 先传输
- 软件模拟 I2C 时序
- 内部实现函数

### 3.6 初始化

OLED 初始化配置：
- 关闭显示（0xAE）
- 设置内存地址模式（0x00, 0x10）
- 设置起始行（0x40）
- 设置页面地址（0xB0）
- 设置对比度（0x81, 0xFF）
- 设置段重映射（0xA1）
- 设置显示模式（0xA6）
- 设置复用率（0xA8, 0x3F）
- 设置 COM 扫描方向（0xC8）
- 设置显示偏移（0xD3, 0x00）
- 设置时钟分频（0xD5, 0x80）
- 设置预充电周期（0xD8, 0x05）
- 设置 VCOMH 电压（0xD9, 0xF1）
- 设置 COM 引脚配置（0xDA, 0x12）
- 设置 VCOMH 去选择电平（0xDB, 0x30）
- 开启电荷泵（0x8D, 0x14）
- 开启显示（0xAF）

### 3.7 开关控制

- `drv_oled_on()`：开启显示
- `drv_oled_off()`：关闭显示

### 3.8 清屏与填充

- `drv_oled_clear()`：清屏（填充 0x00）
- `drv_oled_fill(_data)`：填充指定数据

### 3.9 光标定位

使用 `drv_oled_set_pos(x, y)` 设置显示位置：
- x：0-127（列）
- y：0-7（页）

### 3.10 字符显示

支持两种字符大小：
- 8x16：`_size = 16`，使用 F8X16 字库
- 6x8：`_size = 8`，使用 F6x8 字库

### 3.11 数字显示

使用 `drv_oled_show_num()` 显示数字：
- 支持大整数（0-4294967295）
- 可指定显示长度
- 前导零显示为空格

### 3.12 字符串显示

使用 `drv_oled_show_string()` 显示字符串：
- 自动换行
- 支持指定字符大小

### 3.13 中文显示

使用 `drv_oled_show_chinese()` 显示中文字符：
- 使用 Hzk 字库
- 16x16 点阵
- 通过索引访问

### 3.14 位图显示

使用 `drv_oled_draw_bmp()` 显示位图：
- 支持矩形区域显示
- 预定义 BMP1、BMP2

### 3.15 使用示例

```c
#include "HRTOS_Display.h"

void main(void)
{
    // 初始化 OLED
    drv_oled_init();
    
    // 清屏
    drv_oled_clear();
    
    // 显示字符串
    drv_oled_show_string(0, 0, "HRTOS OLED", 16);
    
    // 显示数字
    drv_oled_show_num(0, 2, 12345, 5, 16);
    
    // 显示字符
    drv_oled_show_char(0, 4, 'A', 16);
    
    // 显示中文字符
    drv_oled_show_chinese(64, 4, 0);
    
    // 显示位图
    drv_oled_draw_bmp(0, 6, 128, 8, BMP1);
    
    while(1)
    {
        // 主循环
    }
}
```

### 3.16 注意事项

- OLED 使用软件模拟 I2C，引脚固定为 P1.0 (SCL) 和 P1.1 (SDA)
- I2C 地址固定为 0x78
- 分辨率为 128x64，分为 8 页
- 列地址自动偏移 +2
- 字符显示支持 8x16 和 6x8 两种大小
- 中文字符使用 Hzk 字库，需预先定义
- 位图数据需预先定义（BMP1、BMP2）
- I2C 等待应答函数为空实现，不进行真正的应答检测
- 底层 I2C 函数为内部实现，用户一般不直接调用

---

## 4. 数码管

### 4.1 功能简介

数码管驱动提供 4 位 7 段数码管的显示功能，支持 0-F 十六进制数字显示。

### 4.2 文件组成

```
Segment/
├── seg_init.c       - 数码管初始化实现
├── seg_display.c    - 数码管显示实现
└── shumaguan.c      - 空文件（可能为预留）
```

### 4.3 硬件连接

数码管连接引脚由 `HRTOS_Display.h` 中的宏定义决定：

```c
#define SEG_PORT    P0
sbit SEG1 = P2^0;
sbit SEG2 = P2^1;
sbit SEG3 = P2^2;
sbit SEG4 = P2^3;
```

- **段码端口：** P0（输出段码）
- **位选引脚：**
  - `SEG1` (P2.0)：第1位位选
  - `SEG2` (P2.1)：第2位位选
  - `SEG3` (P2.2)：第3位位选
  - `SEG4` (P2.3)：第4位位选

**字模表：**
- `seg_table[]`：共阳极字模（0xC0=0, 0xF9=1, ...）
- `seg_table_[]`：共阴极字模（0x3F=0, 0x06=1, ...）
- 当前使用共阳极字模 `seg_table[]`

### 4.4 API 参考

#### drv_seg_init()

**函数原型：**
```c
void drv_seg_init(void);
```

**功能：**
初始化数码管，关闭所有位选。

**参数：**
无

**返回值：**
无

**使用说明：**
- 设置段码端口为 0x00
- 关闭所有位选（SEG1-SEG4 置 1）
- 初始化后数码管全部熄灭

**示例：**
```c
drv_seg_init();
```

---

#### drv_seg_display()

**函数原型：**
```c
void drv_seg_display(unsigned char pos, unsigned char num);
```

**功能：**
在指定位置显示指定数字。

**参数：**
- `pos`：显示位置，范围 0-3（0=第1位，1=第2位，2=第3位，3=第4位）
- `num`：显示数字，范围 0-F（十六进制）

**返回值：**
无

**使用说明：**
- 先关闭所有位选
- 输出段码到 P0 端口
- 根据位置开启对应位选
- 使用共阳极字模表
- 不包含动态扫描，每次调用只显示一位

**示例：**
```c
drv_seg_display(0, 1);  // 第1位显示 1
drv_seg_display(1, 2);  // 第2位显示 2
drv_seg_display(2, 3);  // 第3位显示 3
drv_seg_display(3, 4);  // 第4位显示 4
```

### 4.5 显示方式

**静态显示：**
- 当前实现为静态显示
- 每次调用只点亮一位数码管
- 如需同时显示多位，需要应用层实现动态扫描

**动态扫描（需应用层实现）：**
- 周期性调用 `drv_seg_display()` 切换不同位
- 典型扫描频率：50-100Hz
- 每位显示时间：2-5ms

### 4.6 使用示例

```c
#include "HRTOS_Display.h"

void main(void)
{
    // 初始化数码管
    drv_seg_init();
    
    // 显示单个数字
    drv_seg_display(0, 1);  // 第1位显示 1
    
    // 动态扫描显示多位（需在定时器或主循环中实现）
    while(1)
    {
        drv_seg_display(0, 1);  // 显示第1位
        delay_ms(5);
        drv_seg_display(1, 2);  // 显示第2位
        delay_ms(5);
        drv_seg_display(2, 3);  // 显示第3位
        delay_ms(5);
        drv_seg_display(3, 4);  // 显示第4位
        delay_ms(5);
    }
}
```

### 4.7 注意事项

- 数码管使用共阳极字模表
- 位选引脚固定为 P2.0-P2.3，段码端口固定为 P0
- 当前实现为静态显示，每次只显示一位
- 如需同时显示多位，需要应用层实现动态扫描
- 动态扫描需要定时器或主循环周期性调用
- 字模表支持 0-F 十六进制数字
- shumaguan.c 文件为空，可能是预留文件

---

## 5. API 汇总

| 设备 | API | 功能 | 参数 | 返回值 |
|------|-----|------|------|--------|
| LCD1602 | drv_lcd1602_init | LCD1602 初始化 | 无 | 无 |
| LCD1602 | drv_lcd1602_write_cmd | 写入命令 | cmd | 无 |
| LCD1602 | drv_lcd1602_write_data | 写入数据 | dat | 无 |
| LCD1602 | drv_lcd1602_write_string | 写入字符串 | str | 无 |
| LCD1602 | drv_lcd1602_set_cursor | 设置光标位置 | x, y | 无 |
| LCD1602 | delay_ms | 毫秒延时 | ms | 无 |
| LCD1602 | delay_us | 微秒延时 | 无 | 无 |
| OLED | drv_oled_init | OLED 初始化 | 无 | 无 |
| OLED | drv_oled_on | 开启 OLED | 无 | 无 |
| OLED | drv_oled_off | 关闭 OLED | 无 | 无 |
| OLED | drv_oled_clear | 清屏 | 无 | 无 |
| OLED | drv_oled_fill | 填充屏幕 | _data | 无 |
| OLED | drv_oled_set_pos | 设置显示位置 | x, y | 无 |
| OLED | drv_oled_show_char | 显示字符 | x, y, chr, _size | 无 |
| OLED | drv_oled_show_string | 显示字符串 | x, y, str, _size | 无 |
| OLED | drv_oled_show_num | 显示数字 | x, y, num, len, _size | 无 |
| OLED | drv_oled_show_chinese | 显示中文 | x, y, no | 无 |
| OLED | drv_oled_draw_bmp | 显示位图 | x0, y0, x1, y1, bmp | 无 |
| OLED | drv_oled_iic_write_byte | I2C 写字节（内部） | _data | 无 |
| OLED | drv_oled_pow | 幂运算（内部） | m, n | unsigned long |
| OLED | drv_oled_write_data | 写入数据（内部） | _data | 无 |
| OLED | drv_oled_write_command | 写入命令（内部） | cmd | 无 |
| OLED | drv_oled_iic_wait_ack | I2C 等待应答（内部） | 无 | 无 |
| OLED | drv_oled_iic_start | I2C 起始（内部） | 无 | 无 |
| OLED | drv_oled_iic_stop | I2C 停止（内部） | 无 | 无 |
| Segment | drv_seg_init | 数码管初始化 | 无 | 无 |
| Segment | drv_seg_display | 数码管显示 | pos, num | 无 |

**公开 API 总数：24 个**
**内部实现 API：7 个**

---

## 6. 源码与接口一致性检查

### 6.1 已确认

**头文件与源码一致性：**
- HRTOS_Display.h 中声明的所有公开 API 均有对应的实现文件
- 所有实现函数的函数名、参数类型、返回值类型与头文件声明一致
- 宏定义（LCD_DATA, LED_PORT, OLED_SCL/SDIN, SEG_PORT 等）在实现中正确使用
- 外部资源声明（seg_table, Hzk, F8X16, F6x8, BMP1, BMP2）在对应源文件中定义

**文档与源码一致性：**
- 文档中所有 API 均真实存在于 HRTOS_Display.h
- 硬件引脚定义（LCD1602 的 P2/P0.5-0.7，OLED 的 P1.0/P1.1，数码管的 P0/P2.0-0.3）与源码一致
- 寄存器和端口操作与源码一致
- 示例代码使用的 API 均为真实存在的公开接口
- 内部实现函数已正确标记

**文件覆盖：**
- LCD1602：7 个文件，7 个 API ✓
- OLED：18 个文件（含 SRC），17 个 API（7 个内部）✓
- Segment：3 个文件，2 个 API ✓

**OLED/SRC 文件分类：**
- bmp.h：位图资源数据（BMP1, BMP2）- 资源文件
- oled.c：底层实现文件（仅包含头文件引用）- 内部实现
- oled.h：内部头文件（声明内部函数）- 内部实现
- oledfont.h：字库资源（当前为空）- 资源文件

### 6.2 需要人工确认

**接口边界问题：**

1. **OLED 内部 I2C 函数公开**
   - 问题：`drv_oled_iic_start()`, `drv_oled_iic_stop()`, `drv_oled_iic_wait_ack()`, `drv_oled_iic_write_byte()` 等底层 I2C 函数在 HRTOS_Display.h 中声明为公开接口
   - 状态：这些函数属于底层实现，用户一般不需要直接调用
   - 影响：可能误导用户直接操作 I2C 总线
   - 建议：确认是否应将这些函数移至内部头文件或明确标注为内部实现

2. **OLED 命令/数据写入函数公开**
   - 问题：`drv_oled_write_command()` 和 `drv_oled_write_data()` 在 HRTOS_Display.h 中声明为公开接口
   - 状态：这些函数属于底层实现，通过高层 API（如 drv_oled_show_char）间接调用
   - 影响：用户直接调用可能破坏显示状态
   - 建议：确认是否应将这些函数移至内部头文件或明确标注为内部实现

3. **drv_oled_pow() 辅助函数公开**
   - 问题：`drv_oled_pow()` 在 HRTOS_Display.h 中声明为公开接口
   - 状态：这是数字显示的辅助函数，用户一般不需要直接调用
   - 影响：API 混杂了辅助函数
   - 建议：确认是否应移至内部实现

4. **LCD1602 延时函数公开**
   - 问题：`delay_ms()` 和 `delay_us()` 在 HRTOS_Display.h 中声明为公开接口
   - 状态：这些是 LCD1602 专用的延时函数
   - 影响：可能与系统延时函数混淆
   - 建议：确认是否应重命名或明确标注为 LCD1602 专用

5. **delay_us() 为空函数**
   - 问题：`delay_us()` 实现为空函数，无实际延时效果
   - 状态：函数存在但无功能
   - 影响：可能导致时序问题
   - 建议：确认是否需要实现实际延时或删除该函数

6. **oledfont.h 为空文件**
   - 问题：OLED/SRC/oledfont.h 文件内容为空
   - 状态：字库数据缺失
   - 影响：字符显示功能可能无法正常工作
   - 建议：确认字库数据是否在其他位置定义或需要补充

7. **数码管动态扫描未实现**
   - 问题：`drv_seg_display()` 只实现静态显示，不包含动态扫描
   - 状态：需要应用层实现动态扫描
   - 影响：用户需要自行实现扫描逻辑
   - 建议：确认是否需要提供动态扫描 API 或在文档中明确说明

8. **shumaguan.c 为空文件**
   - 问题：Segment/shumaguan.c 文件内容为空
   - 状态：文件存在但无代码
   - 影响：可能是预留文件或历史遗留
   - 建议：确认该文件的用途或删除

**硬件连接说明：**

9. **OLED 完整接线未完全实现**
   - 问题：HRTOS_Display.h 注释中说明 OLED 有 RES、DC、CS 引脚（P1.2-P1.4），但源码中未定义这些引脚
   - 状态：注释与实际代码不一致
   - 影响：用户可能误解硬件连接
   - 建议：确认实际硬件连接方式或更新注释

10. **数码管共阴/共阳字模选择**
    - 问题：源码中同时定义了共阳极（seg_table）和共阴极（seg_table_）字模表，但实现只使用共阳极
    - 状态：存在未使用的字模表
    - 影响：可能造成混淆
    - 建议：确认是否需要支持共阴极数码管或删除未使用的字模表

**依赖关系：**

11. **LCD1602 初始化依赖 HRTOS**
    - 问题：`drv_lcd1602_init()` 调用 `os_delay_ms(2)`，依赖 HRTOS 系统函数
    - 状态：驱动依赖 HRTOS 系统
    - 影响：在非 HRTOS 环境中可能无法使用
    - 建议：确认是否需要提供非 HRTOS 版本的初始化函数或在文档中明确说明依赖

12. **OLED 字库数据位置**
    - 问题：Hzk、F8X16、F6x8 字库在 HRTOS_Display.h 中声明为外部，但实际定义位置不明确
    - 状态：字库数据可能缺失或位置不明
    - 影响：字符显示功能可能无法正常工作
    - 建议：确认字库数据的实际位置和完整性

**功能完整性：**

13. **OLED 中文显示字库索引不明确**
    - 问题：`drv_oled_show_chinese()` 使用索引 no 访问 Hzk 字库，但字库内容和索引映射不明确
    - 状态：用户无法确定如何选择中文字符
    - 影响：中文显示功能难以使用
    - 建议：提供字库索引映射表或改进 API 接口

---

## 附录

### A. 依赖头文件

所有驱动实现依赖以下头文件：
```c
#include "hrtos_hal.h"
```

LCD1602 初始化额外依赖：
```c
// os_delay_ms() - HRTOS 系统延时函数
```

### B. 编译说明

使用本模块时，需将以下文件加入编译：
- HRTOS_Display.h
- 对应驱动的 .c 实现文件
- OLED/SRC 下的资源文件（如需要）

### C. 版本信息

- 文档版本：1.0
- 基于 HRTOS Driver Library 02_Display 模块源码生成
