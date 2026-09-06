# 01 基础外设驱动

## 1. 模块概述

01_Basic 模块提供 HRTOS 实时操作系统的基础外设驱动支持，包括蜂鸣器、独立按键、矩阵键盘、LED、定时器等常用外设的驱动接口。

本模块通过 `HRTOS_Basic.h` 头文件向用户公开 API 接口，所有驱动函数均以 `drv_` 前缀命名。

**支持的驱动模块：**
- Buzzer 蜂鸣器
- KEY 独立按键
- KEY_Matrix 矩阵键盘
- LED
- Timer1 定时器1

**头文件：**
```c
#include "HRTOS_Basic.h"
```

**依赖：**
- 所有驱动实现依赖 `hrtos_hal.h` 硬件抽象层

---

## 2. Buzzer 蜂鸣器

### 2.1 功能简介

蜂鸣器驱动提供蜂鸣器的初始化、开启、关闭和状态切换功能。

### 2.2 文件组成

```
Buzzer/
├── beep_init.c      - 蜂鸣器初始化实现
├── beep_on.c        - 蜂鸣器开启实现
├── beep_off.c       - 蜂鸣器关闭实现
└── beep_toggle.c    - 蜂鸣器状态切换实现
```

### 2.3 硬件连接

蜂鸣器连接引脚由 `HRTOS_Basic.h` 中的宏定义决定：

```c
sbit beep = P3^6;
```

- **控制引脚：** P3.6
- **控制电平：**
  - `BEEP_ON` (0)：蜂鸣器开启
  - `BEEP_OFF` (1)：蜂鸣器关闭

### 2.4 API 参考

#### drv_beep_init()

**函数原型：**
```c
void drv_beep_init(void);
```

**功能：**
初始化蜂鸣器，将蜂鸣器设置为关闭状态。

**参数：**
无

**返回值：**
无

**使用说明：**
在使用蜂鸣器其他功能前，应先调用此函数进行初始化。

**示例：**
```c
#include "HRTOS_Basic.h"

void main(void)
{
    drv_beep_init();
    // 蜂鸣器已初始化并关闭
}
```

---

#### drv_beep_on()

**函数原型：**
```c
void drv_beep_on(void);
```

**功能：**
开启蜂鸣器。

**参数：**
无

**返回值：**
无

**使用说明：**
调用前应先调用 `drv_beep_init()` 进行初始化。

**示例：**
```c
drv_beep_init();
drv_beep_on();  // 蜂鸣器开启
```

---

#### drv_beep_off()

**函数原型：**
```c
void drv_beep_off(void);
```

**功能：**
关闭蜂鸣器。

**参数：**
无

**返回值：**
无

**使用说明：**
调用前应先调用 `drv_beep_init()` 进行初始化。

**示例：**
```c
drv_beep_init();
drv_beep_on();
// ... 执行其他操作
drv_beep_off();  // 蜂鸣器关闭
```

---

#### drv_beep_toggle()

**函数原型：**
```c
void drv_beep_toggle(void);
```

**功能：**
切换蜂鸣器状态（开启/关闭）。

**参数：**
无

**返回值：**
无

**使用说明：**
调用前应先调用 `drv_beep_init()` 进行初始化。此函数会将当前蜂鸣器状态取反。

**示例：**
```c
drv_beep_init();
drv_beep_toggle();  // 切换到开启状态
drv_beep_toggle();  // 切换到关闭状态
```

### 2.5 使用示例

```c
#include "HRTOS_Basic.h"

void main(void)
{
    // 初始化蜂鸣器
    drv_beep_init();
    
    // 开启蜂鸣器
    drv_beep_on();
    
    // 关闭蜂鸣器
    drv_beep_off();
    
    // 切换蜂鸣器状态
    drv_beep_toggle();
}
```

### 2.6 注意事项

- 蜂鸣器控制引脚固定为 P3.6，如需修改请更改 `HRTOS_Basic.h` 中的 `beep` 定义
- 蜂鸣器为低电平有效（BEEP_ON = 0）
- 驱动不包含延时功能，如需产生蜂鸣音效需配合延时函数使用

---

## 3. INT0 外部中断

### 3.1 功能简介

INT0 驱动提供外部中断0的初始化功能，支持电平触发和下降沿触发两种模式。

**注意：** INT0 驱动在 `HRTOS_Basic.h` 中未声明公开 API，当前仅提供内部实现。

### 3.2 文件组成

```
INT0/
└── int0_init.c      - 外部中断0初始化实现（内部实现）
```

### 3.3 硬件连接

- **中断引脚：** P3.2 (INT0)
- **相关寄存器：**
  - `EX0`：外部中断0允许位
  - `IT0`：外部中断0触发方式选择位

### 3.4 API 参考

**当前 HRTOS_Basic.h 中未声明 INT0 相关公开 API。**

实现文件中存在以下函数，但未作为公开接口暴露：

#### drv_int0_init() （内部实现）

**函数原型：**
```c
void drv_int0_init(unsigned char mode);
```

**功能：**
初始化外部中断0，设置触发模式。

**参数：**
- `mode`：触发模式
  - 0：电平触发
  - 1：下降沿触发

**返回值：**
无

### 3.5 使用示例

由于该函数未在 `HRTOS_Basic.h` 中声明，不建议直接调用。如需使用，请手动添加函数声明。

### 3.6 注意事项

- 当前 INT0 驱动未在头文件中公开，使用需谨慎
- 中断服务函数需要用户自行实现
- P3.2 同时也是独立按键 key1 的输入引脚，使用时需注意冲突

---

## 4. INT1 外部中断

### 4.1 功能简介

INT1 驱动提供外部中断1的初始化功能，支持电平触发和下降沿触发两种模式。

**注意：** INT1 驱动在 `HRTOS_Basic.h` 中未声明公开 API，当前仅提供内部实现。

### 4.2 文件组成

```
INT1/
└── int1_init.c      - 外部中断1初始化实现（内部实现）
```

### 4.3 硬件连接

- **中断引脚：** P3.3 (INT1)
- **相关寄存器：**
  - `EX1`：外部中断1允许位
  - `IT1`：外部中断1触发方式选择位

### 4.4 API 参考

**当前 HRTOS_Basic.h 中未声明 INT1 相关公开 API。**

实现文件中存在以下函数，但未作为公开接口暴露：

#### drv_int1_init() （内部实现）

**函数原型：**
```c
void drv_int1_init(unsigned char mode);
```

**功能：**
初始化外部中断1，设置触发模式。

**参数：**
- `mode`：触发模式
  - 0：电平触发
  - 1：下降沿触发

**返回值：**
无

### 4.5 使用示例

由于该函数未在 `HRTOS_Basic.h` 中声明，不建议直接调用。如需使用，请手动添加函数声明。

### 4.6 注意事项

- 当前 INT1 驱动未在头文件中公开，使用需谨慎
- 中断服务函数需要用户自行实现
- P3.3 同时也是独立按键 key2 的输入引脚，使用时需注意冲突

---

## 5. KEY 独立按键

### 5.1 功能简介

独立按键驱动提供4个独立按键的扫描功能，通过轮询方式检测按键状态。

### 5.2 文件组成

```
KEY/
├── key_init.c       - 按键初始化实现
└── key_scan.c       - 按键扫描实现
```

### 5.3 硬件连接

按键连接引脚由 `HRTOS_Basic.h` 中的宏定义决定：

```c
sbit key1 = P3^2;
sbit key2 = P3^3;
sbit key3 = P3^4;
sbit key4 = P3^5;
#define GPIO_KEY P1
```

- **key1：** P3.2
- **key2：** P3.3
- **key3：** P3.4
- **key4：** P3.5
- **GPIO_KEY：** P1 端口（当前未使用）

**按键状态：**
- 低电平 (0)：按键按下
- 高电平 (1)：按键释放

### 5.4 API 参考

#### drv_key_init()

**函数原型：**
```c
void drv_key_init(void);
```

**功能：**
初始化独立按键驱动。

**参数：**
无

**返回值：**
无

**使用说明：**
当前实现为空函数，预留接口。在使用按键扫描前可调用此函数。

**示例：**
```c
drv_key_init();
```

---

#### drv_key_scan()

**函数原型：**
```c
unsigned char drv_key_scan(void);
```

**功能：**
扫描独立按键状态，返回当前按下的按键编码。

**参数：**
无

**返回值：**
- `KEY_NONE` (0)：无按键按下
- `KEY_1` (1)：key1 按下
- `KEY_2` (2)：key2 按下
- `KEY_3` (3)：key3 按下
- `KEY_4` (4)：key4 按下

**使用说明：**
此函数为轮询式扫描，不包含消抖延时。如需消抖，建议在应用层添加延时或多次扫描确认。

**示例：**
```c
unsigned char key_value;
key_value = drv_key_scan();

if(key_value == KEY_1)
{
    // key1 被按下
}
else if(key_value == KEY_2)
{
    // key2 被按下
}
```

### 5.5 按键扫描说明

- 扫描顺序：key1 → key2 → key3 → key4
- 检测到按键按下后立即返回，不检测多键同时按下
- 无消抖处理，应用层需根据需要添加消抖逻辑
- 无按键释放检测，仅检测按下状态

### 5.6 使用示例

```c
#include "HRTOS_Basic.h"

void main(void)
{
    unsigned char key;
    
    drv_key_init();
    
    while(1)
    {
        key = drv_key_scan();
        
        if(key != KEY_NONE)
        {
            // 有按键按下
            switch(key)
            {
                case KEY_1:
                    // 处理 key1
                    break;
                case KEY_2:
                    // 处理 key2
                    break;
                case KEY_3:
                    // 处理 key3
                    break;
                case KEY_4:
                    // 处理 key4
                    break;
            }
        }
    }
}
```

### 5.7 注意事项

- 按键为低电平有效
- 驱动不包含消抖功能，应用层需自行实现
- P3.2 和 P3.3 同时也是外部中断引脚，使用时需注意冲突
- 当前不支持多键同时检测
- GPIO_KEY (P1) 宏定义存在但未在实现中使用

---

## 6. KEY_Matrix 矩阵键盘

### 6.1 功能简介

矩阵键盘驱动提供 4x4 矩阵键盘的扫描功能，支持 16 个按键的检测。

### 6.2 文件组成

```
KEY_Matrix/
├── matrixkey_init.c    - 矩阵键盘初始化实现
└── matrixkey_scan.c    - 矩阵键盘扫描实现
```

### 6.3 硬件连接

矩阵键盘使用 `HRTOS_Basic.h` 中定义的 GPIO_KEY 端口：

```c
#define GPIO_KEY P1
```

- **数据端口：** P1
- **键盘布局：** 4x4 矩阵（16键）
- **返回值范围：** 0-15（对应16个按键），0xFF 表示无按键

**扫描原理：**
- 行扫描：P1 低4位输出 0x0F
- 列扫描：P1 高4位输出 0xF0
- 通过行列组合确定按键位置

### 6.4 API 参考

#### drv_matrixkey_init()

**函数原型：**
```c
void drv_matrixkey_init(void);
```

**功能：**
初始化矩阵键盘驱动。

**参数：**
无

**返回值：**
无

**使用说明：**
当前实现为空函数，预留接口。在使用矩阵键盘扫描前可调用此函数。

**示例：**
```c
drv_matrixkey_init();
```

---

#### drv_matrixkey_scan()

**函数原型：**
```c
unsigned char drv_matrixkey_scan(void);
```

**功能：**
扫描矩阵键盘状态，返回当前按下的按键编码。

**参数：**
无

**返回值：**
- `MATRIXKEY_NONE` (0xFF)：无按键按下
- 0-15：对应矩阵键盘的16个按键

**使用说明：**
- 此函数包含按键释放等待（while 循环），会阻塞直到按键释放
- 代码中包含注释掉的消抖延时（delay(10ms)），当前未启用
- 返回值为 0-15 的键码，具体键位映射需根据实际硬件确定

**示例：**
```c
unsigned char key;
key = drv_matrixkey_scan();

if(key != MATRIXKEY_NONE)
{
    // 有按键按下，key 为 0-15
}
```

### 6.5 矩阵扫描说明

**扫描流程：**
1. 行扫描：GPIO_KEY = 0x0F，检测低4位变化
2. 列扫描：GPIO_KEY = 0xF0，检测高4位变化
3. 根据行列组合计算键码（0-15）
4. 等待按键释放（while 循环阻塞）

**键码计算：**
- 行值：0x07→0, 0x0B→1, 0x0D→2, 0x0E→3
- 列值：0x70→+0, 0xB0+4, 0xD0→+8, 0xE0→+12
- 最终键码 = 行值 + 列值

### 6.6 使用示例

```c
#include "HRTOS_Basic.h"

void main(void)
{
    unsigned char key;
    
    drv_matrixkey_init();
    
    while(1)
    {
        key = drv_matrixkey_scan();
        
        if(key != MATRIXKEY_NONE)
        {
            // 有按键按下，key 为 0-15
            // 根据键码执行相应操作
        }
    }
}
```

### 6.7 注意事项

- 矩阵键盘与独立按键共享 GPIO_KEY (P1) 端口，不能同时使用
- 扫描函数会阻塞等待按键释放，不适合需要快速响应的场景
- 消抖延时代码已注释，当前未启用消抖功能
- 具体键位与键码的对应关系需根据实际硬件连接确定
- 返回值 0-15 的具体按键映射需用户自行定义

---

## 7. LED

### 7.1 功能简介

LED 驱动提供 LED 的初始化、单独控制、状态切换和批量写入功能，支持 8 个 LED 的独立控制。

### 7.2 文件组成

```
LED/
├── led_init.c       - LED 初始化实现
├── led_on.c         - LED 开启实现
├── led_off.c        - LED 关闭实现
├── led_toggle.c     - LED 状态切换实现
└── led_write.c      - LED 批量写入实现
```

### 7.3 硬件连接

LED 连接端口由 `HRTOS_Basic.h` 中的宏定义决定：

```c
#define LED_PORT P1
```

- **控制端口：** P1
- **控制位：** P1.0 - P1.7（对应 LED0 - LED7）
- **控制电平：**
  - `LED_ON` (0)：LED 点亮
  - `LED_OFF` (1)：LED 熄灭

### 7.4 API 参考

#### drv_led_init()

**函数原型：**
```c
void drv_led_init(void);
```

**功能：**
初始化 LED 端口，将所有 LED 设置为熄灭状态。

**参数：**
无

**返回值：**
无

**使用说明：**
初始化后 LED_PORT = 0xFF（所有 LED 熄灭）。

**示例：**
```c
drv_led_init();
```

---

#### drv_led_on()

**函数原型：**
```c
void drv_led_on(unsigned char led);
```

**功能：**
点亮指定 LED。

**参数：**
- `led`：LED 编号，范围 0-7

**返回值：**
无

**使用说明：**
调用前应先调用 `drv_led_init()` 进行初始化。参数超出范围时行为未定义。

**示例：**
```c
drv_led_init();
drv_led_on(0);   // 点亮 LED0
drv_led_on(3);   // 点亮 LED3
```

---

#### drv_led_off()

**函数原型：**
```c
void drv_led_off(unsigned char led);
```

**功能：**
熄灭指定 LED。

**参数：**
- `led`：LED 编号，范围 0-7

**返回值：**
无

**使用说明：**
调用前应先调用 `drv_led_init()` 进行初始化。参数超出范围时行为未定义。

**示例：**
```c
drv_led_init();
drv_led_on(0);
drv_led_off(0);  // 熄灭 LED0
```

---

#### drv_led_toggle()

**函数原型：**
```c
void drv_led_toggle(unsigned char led);
```

**功能：**
切换指定 LED 的状态（点亮/熄灭）。

**参数：**
- `led`：LED 编号，范围 0-7

**返回值：**
无

**使用说明：**
调用前应先调用 `drv_led_init()` 进行初始化。参数超出范围时行为未定义。

**示例：**
```c
drv_led_init();
drv_led_toggle(0);  // LED0 点亮
drv_led_toggle(0);  // LED0 熄灭
```

---

#### drv_led_write()

**函数原型：**
```c
void drv_led_write(unsigned char value);
```

**功能：**
批量设置所有 LED 的状态。

**参数：**
- `value`：LED 状态值，8位二进制对应 LED0-LED7

**返回值：**
无

**使用说明：**
- 调用前应先调用 `drv_led_init()` 进行初始化
- 每个位对应一个 LED：0=点亮，1=熄灭
- 例如：value=0x00 表示所有 LED 点亮，value=0xFF 表示所有 LED 熄灭

**示例：**
```c
drv_led_init();
drv_led_write(0x00);      // 所有 LED 点亮
drv_led_write(0xFF);      // 所有 LED 熄灭
drv_led_write(0x55);      // LED0,2,4,6 点亮，LED1,3,5,7 熄灭
```

### 7.5 使用示例

```c
#include "HRTOS_Basic.h"

void main(void)
{
    // 初始化 LED
    drv_led_init();
    
    // 点亮 LED0
    drv_led_on(0);
    
    // 熄灭 LED0
    drv_led_off(0);
    
    // 切换 LED1 状态
    drv_led_toggle(1);
    
    // 批量设置 LED
    drv_led_write(0xAA);  // LED1,3,5,7 点亮
}
```

### 7.6 注意事项

- LED 为低电平有效（LED_ON = 0）
- LED 端口与矩阵键盘共享 P1 端口，不能同时使用
- 单个 LED 控制函数参数范围为 0-7，超出范围时行为未定义
- 批量写入会影响所有 LED，需注意不要意外改变其他 LED 的状态

---

## 8. Timer1

### 8.1 功能简介

Timer1 驱动提供定时器1的初始化功能，支持自定义重装值，配置为模式1（16位定时器）。

### 8.2 文件组成

```
Timer1/
└── timer1_init.c    - 定时器1初始化实现
```

### 8.3 硬件配置

**定时器配置：**
- **定时器：** Timer1
- **工作模式：** 模式1（16位定时器）
- **相关寄存器：**
  - `TMOD`：定时器模式寄存器
  - `TH1`：定时器1高8位
  - `TL1`：定时器1低8位
  - `ET1`：定时器1中断允许位
  - `TR1`：定时器1运行控制位

### 8.4 API 参考

#### drv_timer1_init()

**函数原型：**
```c
void drv_timer1_init(unsigned char reload_h, unsigned char reload_l);
```

**功能：**
初始化定时器1，设置重装值并启动定时器。

**参数：**
- `reload_h`：重装值高8位
- `reload_l`：重装值低8位

**返回值：**
无

**使用说明：**
- 定时器配置为模式1（16位定时器）
- 初始化后自动开启定时器中断（ET1=1）和定时器运行（TR1=1）
- 定时器溢出后需要在中断服务函数中重新装载 TH1 和 TL1
- 重装值计算：根据所需定时时间和系统时钟频率计算

**示例：**
```c
// 假设系统时钟 11.0592MHz，定时 1ms
// 重装值 = 65536 - (11059200 / 12 / 1000) = 65536 - 922 = 64614 = 0xFC66
drv_timer1_init(0xFC, 0x66);
```

### 8.5 使用示例

```c
#include "HRTOS_Basic.h"

// 定时器1中断服务函数（需用户自行实现）
void timer1_isr(void) interrupt 3
{
    // 重新装载重装值
    TH1 = 0xFC;
    TL1 = 0x66;
    
    // 定时器中断处理代码
}

void main(void)
{
    // 初始化定时器1，1ms 定时
    drv_timer1_init(0xFC, 0x66);
    
    // 主循环
    while(1)
    {
        // 主程序代码
    }
}
```

### 8.6 注意事项

- 定时器1中断服务函数需要用户自行实现
- 定时器溢出后必须在中断中重新装载 TH1 和 TL1
- 定时器初始化后自动开启中断和运行，无需额外操作
- 重装值需根据实际系统时钟和所需定时时间计算
- Timer1 的中断向量号为 3

---

## 9. API 汇总

| 模块 | API | 功能 | 参数 | 返回值 |
|------|-----|------|------|--------|
| Buzzer | drv_beep_init | 蜂鸣器初始化 | 无 | 无 |
| Buzzer | drv_beep_on | 开启蜂鸣器 | 无 | 无 |
| Buzzer | drv_beep_off | 关闭蜂鸣器 | 无 | 无 |
| Buzzer | drv_beep_toggle | 切换蜂鸣器状态 | 无 | 无 |
| KEY | drv_key_init | 独立按键初始化 | 无 | 无 |
| KEY | drv_key_scan | 扫描独立按键 | 无 | 按键编码 (0-4) |
| KEY_Matrix | drv_matrixkey_init | 矩阵键盘初始化 | 无 | 无 |
| KEY_Matrix | drv_matrixkey_scan | 扫描矩阵键盘 | 无 | 键码 (0-15, 0xFF) |
| LED | drv_led_init | LED 初始化 | 无 | 无 |
| LED | drv_led_on | 点亮指定 LED | LED 编号 (0-7) | 无 |
| LED | drv_led_off | 熄灭指定 LED | LED 编号 (0-7) | 无 |
| LED | drv_led_toggle | 切换 LED 状态 | LED 编号 (0-7) | 无 |
| LED | drv_led_write | 批量设置 LED | 状态值 (0-255) | 无 |
| Timer1 | drv_timer1_init | 定时器1初始化 | 重装值高8位, 重装值低8位 | 无 |

**公开 API 总数：14 个**

---

## 10. 源码与接口一致性检查

### 10.1 已确认

**头文件与源码一致性：**
- HRTOS_Basic.h 中声明的所有公开 API 均有对应的实现文件
- 所有实现函数的函数名、参数类型、返回值类型与头文件声明一致
- 宏定义（BEEP_ON/OFF, LED_ON/OFF, KEY_* 等）在实现中正确使用

**文档与源码一致性：**
- 文档中所有 API 均真实存在于 HRTOS_Basic.h
- 硬件引脚定义（P3.6 蜂鸣器、P3.2-P3.5 按键、P1 LED/矩阵键盘）与源码一致
- 寄存器操作（EX0/EX1, IT0/IT1, TMOD, TH1/TL1, ET1/TR1）与源码一致
- 示例代码使用的 API 均为真实存在的公开接口

**文件覆盖：**
- Buzzer：4 个文件，4 个 API ✓
- INT0：1 个文件，0 个公开 API（内部实现）✓
- INT1：1 个文件，0 个公开 API（内部实现）✓
- KEY：2 个文件，2 个 API ✓
- KEY_Matrix：2 个文件，2 个 API ✓
- LED：5 个文件，5 个 API ✓
- Timer1：1 个文件，1 个 API ✓

### 10.2 需要人工确认

**接口边界问题：**

1. **INT0 和 INT1 未公开**
   - 问题：INT0 和 INT1 目录包含实现文件（int0_init.c, int1_init.c），实现了 `drv_int0_init()` 和 `drv_int1_init()` 函数
   - 状态：这两个函数在 `HRTOS_Basic.h` 中**未声明**，因此不属于公开 API
   - 影响：用户无法通过标准方式使用外部中断功能
   - 建议：确认是否需要在 HRTOS_Basic.h 中添加这两个函数的声明

2. **GPIO_KEY 宏定义未完全使用**
   - 问题：KEY 模块定义了 `#define GPIO_KEY P1`，但在 key_init.c 和 key_scan.c 的实现中并未使用此宏（直接使用 sbit key1-key4）
   - 状态：KEY_Matrix 模块正确使用了 GPIO_KEY 宏
   - 影响：KEY 模块的 GPIO_KEY 宏定义可能是预留接口或历史遗留
   - 建议：确认 KEY 模块的 GPIO_KEY 宏定义用途

3. **drv_key_init 和 drv_matrixkey_init 为空函数**
   - 问题：这两个初始化函数的实现体为空
   - 状态：函数存在但无实际操作
   - 影响：初始化操作可能不完整
   - 建议：确认是否需要添加实际的初始化代码（如 GPIO 方向配置）

4. **矩阵键盘消抖延时被注释**
   - 问题：matrixkey_scan.c 中的消抖延时代码（delay(10ms)）被注释掉
   - 状态：消抖功能未启用
   - 影响：可能出现按键抖动误检测
   - 建议：确认是否需要启用消抖功能或由应用层实现

5. **矩阵键盘阻塞等待释放**
   - 问题：drv_matrixkey_scan() 包含 `while(GPIO_KEY != 0xf0);` 阻塞等待按键释放
   - 状态：函数会阻塞直到按键释放
   - 影响：在需要快速响应的场景中可能不适用
   - 建议：确认此阻塞行为是否符合设计预期

**硬件连接冲突：**
- P3.2 同时为 key1 和 INT0 中断引脚
- P3.3 同时为 key2 和 INT1 中断引脚
- P1 同时为 LED_PORT 和 GPIO_KEY（矩阵键盘）
- 建议：确认硬件设计是否允许这些功能复用，或需要在文档中明确说明互斥关系

---

## 附录

### A. 依赖头文件

所有驱动实现依赖以下头文件：
```c
#include "hrtos_hal.h"
```

### B. 编译说明

使用本模块时，需将以下文件加入编译：
- HRTOS_Basic.h
- 对应驱动的 .c 实现文件

### C. 版本信息

- 文档版本：1.0
- 基于 HRTOS Driver Library 01_Basic 模块源码生成
