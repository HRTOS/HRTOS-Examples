# 03 传感器驱动

## 1. 模块概述

03_Sensor 模块提供 HRTOS 实时操作系统的传感器驱动支持，包括 ADC 模数转换、DHT11 温湿度、DS1302 实时时钟、DS18B20 温度传感器、HC-SR04 超声波测距、HC-SR501 人体红外、土壤湿度、红外避障和雨滴传感器驱动。

本模块通过 `HRTOS_Sensor.h` 头文件向用户公开 API 接口，所有驱动函数均以 `drv_` 前缀命名。

**支持的传感器：**
- ADC：STC12C5A60S2 内置 10 位 ADC
- DHT11：数字温湿度传感器
- DS1302：实时时钟芯片
- DS18B20：数字温度传感器（1-Wire）
- HC-SR04：超声波测距模块
- HC-SR501：人体红外感应模块
- 土壤湿度：模拟/数字输出土壤湿度传感器
- 红外避障：红外避障传感器
- 雨滴 YL-83：雨滴检测传感器

**头文件：**
```c
#include "HRTOS_Sensor.h"
```

**依赖：**
- 所有驱动实现依赖 `hrtos_hal.h` 硬件抽象层
- ADC 驱动使用 STC12C5A60S2 内置 ADC
- DS18B20 驱动依赖 `<intrins.h>`（_nop_ 函数）
- DS1302 驱动依赖 `<intrins.H>`（_nop_ 函数）
- HC-SR04 驱动占用 Timer1

---

## 2. ADC

### 2.1 功能简介

ADC 驱动提供 STC12C5A60S2 内置 10 位 ADC 的读取功能，支持 8 个通道（P1.0-P1.7），适用于模拟信号采集。

**主要功能：**
- 读取指定 ADC 通道的 10 位转换结果
- 数据范围：0-1023

### 2.2 文件组成

```
ADC/
├── adc_init.c       - ADC 初始化实现
├── adc_get_value.c  - ADC 值读取实现
└── adc_isr.c        - ADC 中断服务程序（内部实现）
```

### 2.3 硬件资源

**ADC 特殊功能寄存器：**
```c
sfr ADC_CONTR = 0xBC;  // ADC 控制寄存器
sfr ADC_RES   = 0xBD;  // ADC 结果高 8 位
sfr ADC_LOW2  = 0xBE;  // ADC 结果低 2 位
sfr P1ASF     = 0x9D;  // P1 模拟功能选择寄存器
```

**ADC 通道定义：**
- ADC_CH0：P1.0
- ADC_CH1：P1.1
- ADC_CH2：P1.2
- ADC_CH3：P1.3
- ADC_CH4：P1.4
- ADC_CH5：P1.5
- ADC_CH6：P1.6
- ADC_CH7：P1.7

**ADC 控制位：**
- DRV_ADC_POWER (0x80)：ADC 电源控制
- DRV_ADC_FLAG (0x10)：ADC 转换完成标志
- DRV_ADC_START (0x08)：ADC 启动转换
- DRV_ADC_SPEEDLL (0x00)：540 时钟周期
- DRV_ADC_SPEEDL (0x20)：360 时钟周期
- DRV_ADC_SPEEDH (0x40)：180 时钟周期
- DRV_ADC_SPEEDHH (0x60)：90 时钟周期

### 2.4 API 参考

#### drv_adc_get_value()

**函数原型：**
```c
u16 drv_adc_get_value(u8 channel);
```

**功能：**
读取指定 ADC 通道的 10 位转换结果。

**参数：**
- `channel`：ADC 通道号（0-7）

**返回值：**
- 10 位 ADC 转换结果，范围 0-1023
- 通道号大于 7 时返回 0

**使用说明：**
- 设置 P1ASF 为 0xFF（启用所有 P1 口的 ADC 功能）
- 清零 ADC_RES 和 ADC_RESL
- 配置 ADC_CONTR：电源 + 启动 + 速度 (0x20) + 通道
- 等待转换完成（ADC_CONTR & 0x10）
- 清除启动位
- 计算 10 位结果：`value = ADC_RES * 4 + ADC_RESL`

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    u16 adc_value;
    
    // 读取 P1.0 通道
    adc_value = drv_adc_get_value(ADC_CH0);
    
    // 读取 P1.1 通道
    adc_value = drv_adc_get_value(ADC_CH1);
}
```

### 2.5 ADC 数据获取

调用 `drv_adc_get_value(channel)` 读取 ADC 值：
- 通道范围：0-7
- 返回值范围：0-1023
- 每次调用完成一次完整的 ADC 转换

### 2.6 中断相关说明

**adc_isr()** 为内部中断服务程序（未在公开 API 中声明）：
- 功能：处理 ADC 中断，读取转换结果并自动启动下一次转换
- 使用：需要用户自行配置中断向量
- 外部变量：
  - `drv_adc_channel`：当前 ADC 通道
  - `drv_adc_value`：ADC 转换结果

### 2.7 使用示例

```c
#include "HRTOS_Sensor.h"

void main(void)
{
    u16 value;
    
    // 读取 P1.0 的模拟值
    value = drv_adc_get_value(0);
    
    // 读取 P1.7 的模拟值
    value = drv_adc_get_value(7);
    
    while(1)
    {
        // 周期性读取
        value = drv_adc_get_value(0);
        // 处理数据...
    }
}
```

### 2.8 注意事项

- ADC 仅适用于 STC12C5A60S2 等支持内置 ADC 的 STC 单片机
- ADC 通道对应 P1.0-P1.7
- 转换速度设置为 360 时钟周期（DRV_ADC_SPEEDL）
- 每次读取都会重新配置所有 P1 口为 ADC 功能
- 返回值为 10 位原始 ADC 值，未进行电压换算
- 通道号超出范围时返回 0

---

## 3. DHT11

### 3.1 功能简介

DHT11 驱动提供数字温湿度传感器的读取功能，通过单总线通信获取温度和湿度数据。

**主要功能：**
- 初始化 DHT11
- 读取温湿度数据
- 校验和验证

### 3.2 文件组成

```
DHT11/
├── dht11_init.c       - DHT11 初始化实现
├── dht11_read.c       - 温湿度读取实现
├── dht11_read_byte.c  - 字节读取实现（内部实现）
├── dht11_delay.c      - 延时函数实现（内部实现）
└── dht11_delay_10us.c - 10us 延时实现（内部实现）
```

### 3.3 硬件连接

DHT11 数据引脚由 `HRTOS_Sensor.h` 中的宏定义决定：

```c
sbit DHT11_DATA = P3^7;
```

- **数据引脚：** P3.7（单总线通信）

### 3.4 通信时序

**初始化时序：**
- 主机拉低 DHT11_DATA 至少 18ms
- 主机释放总线
- 等待 DHT11 响应

**数据读取时序：**
- 主机发送起始信号（拉低 18ms，释放）
- DHT11 响应（拉低 80us，拉高 80us）
- 读取 40 位数据（5 字节）：
  - 湿度高 8 位
  - 湿度低 8 位
  - 温度高 8 位
  - 温度低 8 位
  - 校验和

**位读取时序：**
- 等待 DHT11_DATA 变低
- 延时 20us
- 读取 DHT11_DATA 电平（0 或 1）
- 等待 DHT11_DATA 变高

### 3.5 API 参考

#### drv_dht11_init()

**函数原型：**
```c
void drv_dht11_init(void);
```

**功能：**
初始化 DHT11，设置数据引脚为高电平，清零数据缓存。

**参数：**
无

**返回值：**
无

**使用说明：**
- 设置 DHT11_DATA = 1
- 清零所有数据缓存变量

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    drv_dht11_init();
    // DHT11 已初始化
}
```

---

#### drv_dht11_read()

**函数原型：**
```c
unsigned char drv_dht11_read(unsigned char *humidity,
                             unsigned char *temperature);
```

**功能：**
读取 DHT11 温湿度数据。

**参数：**
- `humidity`：湿度指针（输出参数）
- `temperature`：温度指针（输出参数）

**返回值：**
- 0：读取成功
- 数据通过指针参数返回

**使用说明：**
- 发送起始信号（拉低 18ms，释放）
- 关闭中断（EA = 0）
- 等待 DHT11 响应
- 读取 5 字节数据
- 验证校验和：`checksum = rh_high + rh_low + t_high + t_low`
- 校验通过则保存数据到全局变量
- 恢复中断状态
- 返回湿度整数部分和温度整数部分

**数据格式：**
- 湿度：整数部分（0-50%）
- 温度：整数部分（0-50°C）
- 小数部分当前未使用

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    unsigned char humidity, temperature;
    
    drv_dht11_init();
    
    if(drv_dht11_read(&humidity, &temperature) == 0)
    {
        // 读取成功
        // humidity: 湿度
        // temperature: 温度
    }
}
```

### 3.6 数据读取

调用 `drv_dht11_read(&humidity, &temperature)` 读取温湿度：
- 关闭中断进行时序敏感操作
- 读取 40 位数据（5 字节）
- 校验和验证
- 返回整数部分数据

### 3.7 数据格式

**返回数据：**
- `humidity`：湿度整数部分（0-50）
- `temperature`：温度整数部分（0-50）

**全局变量（内部）：**
- `dht11_humidity_high`：湿度整数
- `dht11_humidity_low`：湿度小数
- `dht11_temperature_high`：温度整数
- `dht11_temperature_low`：温度小数
- `dht11_checksum`：校验和

### 3.8 使用示例

```c
#include "HRTOS_Sensor.h"

void main(void)
{
    unsigned char humidity, temperature;
    
    drv_dht11_init();
    
    while(1)
    {
        if(drv_dht11_read(&humidity, &temperature) == 0)
        {
            // 读取成功，处理数据
        }
        
        // 延时 2 秒（DHT11 采样间隔）
        os_delay_ms(2000);
    }
}
```

### 3.9 注意事项

- DHT11 使用单总线通信，时序敏感
- 读取时关闭中断以保证时序准确
- 数据引脚固定为 P3.7
- 采样间隔建议不小于 2 秒
- 返回值为整数部分，小数部分保存在全局变量
- 校验和验证失败时不更新全局变量
- 延时函数为软件实现，精度取决于系统时钟

---

## 4. DS1302

### 4.1 功能简介

DS1302 驱动提供 DS1302 实时时钟芯片的读写功能，支持时间设置和时间读取。

**主要功能：**
- 初始化 DS1302
- 设置时间
- 读取时间

### 4.2 文件组成

```
DS1302/
├── ds1302_init.c       - DS1302 初始化实现
├── ds1302_set_time.c   - 时间设置实现
├── ds1302_read_time.c  - 时间读取实现
├── ds1302_read_byte.c  - 字节读取实现（内部实现）
└── ds1302_write_byte.c - 字节写入实现（内部实现）
```

### 4.3 硬件连接

DS1302 连接引脚由 `HRTOS_Sensor.h` 中的宏定义决定：

```c
sbit DS1302_IO   = P3^0;
sbit DS1302_RST  = P3^1;
sbit DS1302_SCLK = P3^2;
```

- **DS1302_IO** (P3.0)：数据输入/输出
- **DS1302_RST** (P3.1)：片选/复位
- **DS1302_SCLK** (P3.2)：串行时钟

### 4.4 通信方式

**三线串行通信：**
- CE/RST：片选信号
- SCLK：时钟信号
- I/O：双向数据线

**通信时序：**
- 拉低 SCLK
- 拉高 RST
- 写入地址（8 位，低位先传）
- 读取/写入数据（8 位，低位先传）
- 拉高 SCLK
- 拉低 RST

**地址定义：**
- 读地址：0x81, 0x83, 0x85, 0x87, 0x89, 0x8B, 0x8D
- 写地址：0x80, 0x82, 0x84, 0x86, 0x88, 0x8A, 0x8C

### 4.5 API 参考

#### drv_ds1302_init()

**函数原型：**
```c
void drv_ds1302_init(void);
```

**功能：**
初始化 DS1302，解除写保护。

**参数：**
无

**返回值：**
无

**使用说明：**
- 关闭中断
- 写入 0x8E, 0x00（解除写保护）
- 写入 0x8E, 0x80（恢复写保护）
- 恢复中断

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    drv_ds1302_init();
    // DS1302 已初始化
}
```

---

#### drv_ds1302_set_time()

**函数原型：**
```c
void drv_ds1302_set_time(unsigned char *time);
```

**功能：**
设置 DS1302 时间。

**参数：**
- `time`：时间数组指针
  - time[0]：秒（BCD 格式）
  - time[1]：分（BCD 格式）
  - time[2]：时（BCD 格式）
  - time[3]：日（BCD 格式）
  - time[4]：月（BCD 格式）
  - time[5]：星期（BCD 格式）
  - time[6]：年（BCD 格式）

**返回值：**
无

**使用说明：**
- 关闭中断
- 写入 0x8E, 0x00（解除写保护）
- 依次写入 7 个时间寄存器
- 写入 0x8E, 0x80（恢复写保护）
- 恢复中断
- 数据采用 BCD 格式

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    unsigned char time[7];
    
    // 设置时间：2024-01-15 星期一 12:30:45
    time[0] = 0x45;  // 秒
    time[1] = 0x30;  // 分
    time[2] = 0x12;  // 时
    time[3] = 0x15;  // 日
    time[4] = 0x01;  // 月
    time[5] = 0x01;  // 星期
    time[6] = 0x24;  // 年
    
    drv_ds1302_set_time(time);
}
```

---

#### drv_ds1302_read_time()

**函数原型：**
```c
void drv_ds1302_read_time(unsigned char *time);
```

**功能：**
读取 DS1302 时间。

**参数：**
- `time`：时间数组指针
  - time[0]：秒（BCD 格式）
  - time[1]：分（BCD 格式）
  - time[2]：时（BCD 格式）
  - time[3]：日（BCD 格式）
  - time[4]：月（BCD 格式）
  - time[5]：星期（BCD 格式）
  - time[6]：年（BCD 格式）

**返回值：**
无

**使用说明：**
- 关闭中断
- 依次读取 7 个时间寄存器
- 恢复中断
- 数据采用 BCD 格式

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    unsigned char time[7];
    
    drv_ds1302_read_time(time);
    
    // time[0]: 秒 (BCD)
    // time[1]: 分 (BCD)
    // time[2]: 时 (BCD)
    // time[3]: 日 (BCD)
    // time[4]: 月 (BCD)
    // time[5]: 星期 (BCD)
    // time[6]: 年 (BCD)
}
```

### 4.6 时间读取

调用 `drv_ds1302_read_time(time)` 读取时间：
- 读取 7 个时间寄存器
- 数据为 BCD 格式
- 需要自行转换为十进制

### 4.7 时间设置

调用 `drv_ds1302_set_time(time)` 读取时间：
- 写入 7 个时间寄存器
- 数据需为 BCD 格式
- 需要自行将十进制转换为 BCD

### 4.8 数据格式

**BCD 格式：**
- 秒：0x00-0x59
- 分：0x00-0x59
- 时：0x00-0x23
- 日：0x01-0x31
- 月：0x01-0x12
- 星期：0x01-0x07
- 年：0x00-0x99

**BCD 转换：**
- BCD 转十进制：`dec = (bcd >> 4) * 10 + (bcd & 0x0F)`
- 十进制转 BCD：`bcd = ((dec / 10) << 4) + (dec % 10)`

### 4.9 使用示例

```c
#include "HRTOS_Sensor.h"

unsigned char bcd_to_dec(unsigned char bcd)
{
    return (bcd >> 4) * 10 + (bcd & 0x0F);
}

unsigned char dec_to_bcd(unsigned char dec)
{
    return ((dec / 10) << 4) + (dec % 10);
}

void main(void)
{
    unsigned char time[7];
    
    // 初始化
    drv_ds1302_init();
    
    // 设置时间
    time[0] = dec_to_bcd(45);  // 秒
    time[1] = dec_to_bcd(30);  // 分
    time[2] = dec_to_bcd(12);  // 时
    time[3] = dec_to_bcd(15);  // 日
    time[4] = dec_to_bcd(1);   // 月
    time[5] = dec_to_bcd(1);   // 星期
    time[6] = dec_to_bcd(24);  // 年
    drv_ds1302_set_time(time);
    
    // 读取时间
    drv_ds1302_read_time(time);
    
    // 转换为十进制
    unsigned char sec = bcd_to_dec(time[0]);
    unsigned char min = bcd_to_dec(time[1]);
    unsigned char hour = bcd_to_dec(time[2]);
}
```

### 4.10 注意事项

- DS1302 使用三线串行通信
- 通信时关闭中断以保证时序准确
- 引脚固定为 P3.0-P3.2
- 数据采用 BCD 格式
- 需要自行进行 BCD/十进制转换
- 写保护控制寄存器地址为 0x8E
- 写保护解除：0x00，写保护恢复：0x80

---

## 5. DS18B20

### 5.1 功能简介

DS18B20 驱动提供 DS18B20 数字温度传感器的读取功能，通过 1-Wire 总线通信获取温度数据。

**主要功能：**
- 初始化 DS18B20
- 启动温度转换
- 读取温度

### 5.2 文件组成

```
DS18B20/
├── ds18b20_init.c       - DS18B20 初始化实现
├── ds18b20_start.c      - 启动温度转换实现
├── ds18b20_read.c       - 温度读取实现
├── ds18b20_reset.c      - 复位实现（内部实现）
├── ds18b20_read_bit.c   - 位读取实现（内部实现）
├── ds18b20_read_byte.c  - 字节读取实现（内部实现）
└── ds18b20_write_byte.c - 字节写入实现（内部实现）
```

### 5.3 硬件连接

DS18B20 数据引脚由 `HRTOS_Sensor.h` 中的宏定义决定：

```c
sbit DS18B20_DATA = P3^7;
```

- **数据引脚：** P3.7（1-Wire 总线）

### 5.4 1-Wire 通信

**复位时序：**
- 主机拉低 DS18B20_DATA 约 480-960us
- 主机释放总线
- 等待 DS18B20 响应（拉低 60-240us）
- 等待 DS18B20 释放总线

**写 0 时序：**
- 拉低总线 60us
- 释放总线
- 恢复时间至少 1us

**写 1 时序：**
- 拉低总线 1-15us
- 释放总线
- 保持总线高电平至少 45us

**读时序：**
- 拉低总线 1us
- 释放总线
- 延时 15us
- 读取总线电平
- 恢复时间至少 45us

### 5.5 API 参考

#### drv_ds18b20_init()

**函数原型：**
```c
void drv_ds18b20_init(void);
```

**功能：**
初始化 DS18B20，设置数据引脚为高电平。

**参数：**
无

**返回值：**
无

**使用说明：**
- 设置 DS18B20_DATA = 1

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    drv_ds18b20_init();
    // DS18B20 已初始化
}
```

---

#### drv_ds18b20_start()

**函数原型：**
```c
void drv_ds18b20_start(void);
```

**功能：**
启动一次温度转换。

**参数：**
无

**返回值：**
无

**使用说明：**
- 关闭中断
- 复位 DS18B20
- 发送 Skip ROM 命令（0xCC）
- 发送 Convert T 命令（0x44）
- 恢复中断
- 温度转换需要时间（约 750ms @ 12 位分辨率）

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    drv_ds18b20_init();
    
    // 启动温度转换
    drv_ds18b20_start();
    
    // 等待转换完成
    os_delay_ms(750);
}
```

---

#### drv_ds18b20_read()

**函数原型：**
```c
signed int drv_ds18b20_read(void);
```

**功能：**
读取温度值。

**参数：**
无

**返回值：**
- 温度值，单位 0.01°C
- 例如：2525 = 25.25°C，-1050 = -10.50°C

**使用说明：**
- 关闭中断
- 复位 DS18B20
- 发送 Skip ROM 命令（0xCC）
- 发送 Read Scratchpad 命令（0xBE）
- 读取温度低字节
- 读取温度高字节
- 恢复中断
- 组合 16 位数据：`raw_data = high << 8 | low`
- 转换为温度：`temperature = raw_data * 0.0625`（12 位分辨率）
- 转换为 0.01°C 单位：`value = temperature * 100 + (value > 0 ? 0.5 : -0.5)`

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    signed int temperature;
    
    drv_ds18b20_init();
    
    // 启动转换
    drv_ds18b20_start();
    
    // 等待转换完成
    os_delay_ms(750);
    
    // 读取温度
    temperature = drv_ds18b20_read();
    
    // temperature = 2525 表示 25.25°C
}
```

### 5.6 复位

`drv_ds18b20_reset()` 为内部实现：
- 拉低 DS18B20_DATA 约 70us
- 释放总线
- 等待 DS18B20 响应（超时 5ms）

### 5.7 温度转换

调用 `drv_ds18b20_start()` 启动温度转换：
- 发送 Skip ROM 命令（0xCC）
- 发送 Convert T 命令（0x44）
- 转换时间约 750ms（12 位分辨率）

### 5.8 温度读取

调用 `drv_ds18b20_read()` 读取温度：
- 发送 Skip ROM 命令（0xCC）
- 发送 Read Scratchpad 命令（0xBE）
- 读取 2 字节温度数据
- 返回值为 0.01°C 单位

### 5.9 数据格式

**返回值格式：**
- 单位：0.01°C
- 正数：2525 = 25.25°C
- 负数：-1050 = -10.50°C
- 分辨率：0.0625°C（12 位）

**原始数据格式：**
- 16 位有符号数
- 高 5 位为符号位
- 低 11 位为温度值
- LSB = 0.0625°C

### 5.10 使用示例

```c
#include "HRTOS_Sensor.h"

void main(void)
{
    signed int temperature;
    
    drv_ds18b20_init();
    
    while(1)
    {
        // 启动温度转换
        drv_ds18b20_start();
        
        // 等待转换完成
        os_delay_ms(750);
        
        // 读取温度
        temperature = drv_ds18b20_read();
        
        // 转换为实际温度值
        float temp = temperature / 100.0;
        
        // 延时
        os_delay_ms(1000);
    }
}
```

### 5.11 注意事项

- DS18B20 使用 1-Wire 总线通信
- 通信时关闭中断以保证时序准确
- 数据引脚固定为 P3.7
- 温度转换需要约 750ms
- 返回值为 0.01°C 单位
- 分辨率为 0.0625°C（12 位）
- 延时函数为软件实现，精度取决于系统时钟
- 复位超时为 5ms

---

## 6. HC-SR04

### 6.1 功能简介

HC-SR04 驱动提供超声波测距模块的测距功能，通过测量超声波往返时间计算距离。

**主要功能：**
- 初始化超声波模块
- 测量距离
- 多次采样与滤波

### 6.2 文件组成

```
HC-SR04/
├── ultrasonic_init.c      - 超声波初始化实现
├── ultrasonic_read.c      - 距离读取实现
├── ultrasonic_measure.c   - 单次测量实现（内部实现）
└── ultrasonic_sort.c      - 数据排序实现（内部实现）
```

### 6.3 硬件连接

HC-SR04 连接引脚由 `HRTOS_Sensor.h` 中的宏定义决定：

```c
sbit ULTRASONIC_RX = P1^1;
sbit ULTRASONIC_TX = P1^2;
```

- **ULTRASONIC_TX** (P1.2)：触发信号输出（TRIG）
- **ULTRASONIC_RX** (P1.1)：回波信号输入（ECHO）

### 6.4 测距原理

**测距流程：**
1. 发送触发脉冲（TRIG 拉高至少 10us）
2. 模块自动发送 8 个 40kHz 超声波脉冲
3. 检测回波信号（ECHO）
4. 测量 ECHO 高电平持续时间
5. 计算距离：`距离 = 时间 × 声速 / 2`

**距离计算：**
- 声速：340 m/s
- 11.0592MHz 12T 51 单片机：1 个机器周期约 1.085us
- 距离公式：`distance = count × 0.1953925`（单位：mm）

### 6.5 API 参考

#### drv_ultrasonic_init()

**函数原型：**
```c
void drv_ultrasonic_init(void);
```

**功能：**
初始化超声波模块，配置 Timer1。

**参数：**
无

**返回值：**
无

**使用说明：**
- 配置 Timer1 为模式 1（16 位定时器）
- 清零 Timer1 计数器
- 停止 Timer1

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    drv_ultrasonic_init();
    // 超声波已初始化
}
```

---

#### drv_ultrasonic_read()

**函数原型：**
```c
unsigned int drv_ultrasonic_read(void);
```

**功能：**
测量距离并返回平均值。

**参数：**
无

**返回值：**
- 距离，单位：mm

**使用说明：**
- 进行 8 次测量（DRV_ULTRASONIC_SAMPLE_MAX）
- 对测量结果排序
- 去除两端各 0 个数据（DRV_ULTRASONIC_FILTER_COUNT = 0）
- 计算平均值
- 转换为距离：`distance = average × 0.1953925`

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    unsigned int distance;
    
    drv_ultrasonic_init();
    
    while(1)
    {
        distance = drv_ultrasonic_read();
        
        // distance 单位为 mm
        // 例如：distance = 150 表示 150mm
        
        os_delay_ms(100);
    }
}
```

### 6.6 测量流程

`drv_ultrasonic_measure()` 为内部实现：
- 关闭中断
- 清零 Timer1
- 发送触发脉冲（拉低 10us，拉高 2us，拉低）
- 等待 ECHO 变高
- 启动 Timer1
- 等待 ECHO 变低
- 停止 Timer1
- 读取 Timer1 计数值
- 恢复中断

### 6.7 数据处理与排序

**采样配置：**
- 采样次数：8 次（DRV_ULTRASONIC_SAMPLE_MAX）
- 滤波次数：0 次（DRV_ULTRASONIC_FILTER_COUNT）

**排序算法：**
- 冒泡排序
- 升序排列

**滤波：**
- 去除两端各 DRV_ULTRASONIC_FILTER_COUNT 个数据
- 当前设置为 0，即不去除任何数据
- 计算剩余数据的平均值

### 6.8 使用示例

```c
#include "HRTOS_Sensor.h"

void main(void)
{
    unsigned int distance;
    
    drv_ultrasonic_init();
    
    while(1)
    {
        distance = drv_ultrasonic_read();
        
        // 转换为 cm
        unsigned int distance_cm = distance / 10;
        
        os_delay_ms(100);
    }
}
```

### 6.9 注意事项

- HC-SR04 占用 Timer1 资源
- 引脚固定为 P1.1 (RX) 和 P1.2 (TX)
- 测量时关闭中断以保证时序准确
- 返回值单位为 mm
- 距离计算基于 11.0592MHz 系统时钟
- 采样次数为 8 次
- 当前滤波次数为 0（不进行滤波）
- 测量超时未处理，可能导致长时间阻塞

---

## 7. HC-SR501 人体红外传感器

### 7.1 功能简介

HC-SR501 驱动提供人体红外感应模块的读取功能，检测人体移动。

**主要功能：**
- 初始化人体红外传感器
- 读取感应状态

### 7.2 文件组成

```
人体感应HC-SR501/
├── human_init.c      - 人体红外初始化实现
├── human_read.c      - 状态读取实现
```

### 7.3 硬件连接

HC-SR501 连接引脚由 `HRTOS_Sensor.h` 中的宏定义决定：

```c
sbit DRV_HUMAN_IN = P3^3;
```

- **DRV_HUMAN_IN** (P3.3)：人体感应输出

### 7.4 API 参考

#### drv_human_init()

**函数原型：**
```c
void drv_human_init(void);
```

**功能：**
初始化人体红外传感器，设置输入引脚为高电平。

**参数：**
无

**返回值：**
无

**使用说明：**
- 设置 DRV_HUMAN_IN = 1

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    drv_human_init();
    // 人体红外传感器已初始化
}
```

---

#### drv_human_read()

**函数原型：**
```c
unsigned char drv_human_read(void);
```

**功能：**
读取人体感应状态。

**参数：**
无

**返回值：**
- 1：检测到人体
- 0：未检测到人体

**使用说明：**
- 直接返回 DRV_HUMAN_IN 引脚电平
- 高电平表示检测到人体
- 低电平表示未检测到人体

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    unsigned char state;
    
    drv_human_init();
    
    while(1)
    {
        state = drv_human_read();
        
        if(state == 1)
        {
            // 检测到人体
        }
        else
        {
            // 未检测到人体
        }
        
        os_delay_ms(100);
    }
}
```

### 7.5 使用示例

```c
#include "HRTOS_Sensor.h"

void main(void)
{
    drv_human_init();
    
    while(1)
    {
        if(drv_human_read() == 1)
        {
            // 人体移动检测
        }
        
        os_delay_ms(100);
    }
}
```

### 7.6 注意事项

- 引脚固定为 P3.3
- 高电平有效（检测到人体）
- 低电平表示未检测到人体
- 模块本身有延时和触发灵敏度调节
- 检测范围和延时由模块硬件决定

---

## 8. 土壤湿度传感器

### 8.1 功能简介

土壤湿度传感器提供模拟输出（AO）和数字输出（DO）两种读取方式，用于检测土壤湿度。

**主要功能：**
- 初始化土壤湿度传感器
- 读取模拟量（ADC）
- 读取数字量（阈值比较）

### 8.2 文件组成

```
土壤湿度/
├── soil_init.c      - 土壤湿度初始化实现
├── soil_read_ao.c   - 模拟量读取实现
└── soil_read_do.c   - 数字量读取实现
```

### 8.3 硬件连接

土壤湿度传感器连接引脚由 `HRTOS_Sensor.h` 中的宏定义决定：

```c
sbit DRV_SOIL_DO = P1^1;
```

- **AO**：模拟输出，连接到 P1.0（ADC 通道 0）
- **DO**：数字输出，连接到 P1.1（LM393 比较器输出）

### 8.4 AO 模拟输出

**ADC 配置：**
- 使用 STC12C5A60S2 内置 ADC
- 固定使用通道 0（P1.0）
- 10 位分辨率
- 数据范围：0-1023

**数据含义：**
- ADC 原始值，未进行湿度百分比转换
- 值越大表示湿度越高（需根据实际传感器特性确认）

### 8.5 DO 数字输出

**数字输出：**
- LM393 比较器输出
- 阈值由模块电位器调节
- 高电平：湿度低于阈值
- 低电平：湿度高于阈值

### 8.6 API 参考

#### drv_soil_init()

**函数原型：**
```c
void drv_soil_init(void);
```

**功能：**
初始化土壤湿度传感器，配置 ADC。

**参数：**
无

**返回值：**
无

**使用说明：**
- 设置 P1ASF |= 0x01（启用 P1.0 的 ADC 功能）
- 清零 ADC_RES
- 启动 ADC（通道 0）

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    drv_soil_init();
    // 土壤湿度传感器已初始化
}
```

---

#### drv_soil_read_ao()

**函数原型：**
```c
unsigned int drv_soil_read_ao(void);
```

**功能：**
读取土壤湿度模拟量。

**参数：**
无

**返回值：**
- 10 位 ADC 值，范围 0-1023

**使用说明：**
- 配置 ADC_CONTR：电源 + 速度 + 启动 + 通道 0
- 等待转换完成
- 清除完成标志
- 计算 10 位结果：`value = (ADC_RES << 2) | (ADC_LOW2 & 0x03)`

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    unsigned int adc_value;
    
    drv_soil_init();
    
    adc_value = drv_soil_read_ao();
    
    // adc_value 范围 0-1023
}
```

---

#### drv_soil_read_do()

**函数原型：**
```c
unsigned char drv_soil_read_do(void);
```

**功能：**
读取土壤湿度数字量。

**参数：**
无

**返回值：**
- 0 或 1（LM393 比较器输出）

**使用说明：**
- 直接返回 DRV_SOIL_DO 引脚电平
- 阈值由模块电位器调节

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    unsigned char state;
    
    drv_soil_init();
    
    state = drv_soil_read_do();
    
    // state: 0 或 1
}
```

### 8.7 使用示例

```c
#include "HRTOS_Sensor.h"

void main(void)
{
    unsigned int adc_value;
    unsigned char digital_state;
    
    drv_soil_init();
    
    while(1)
    {
        // 读取模拟量
        adc_value = drv_soil_read_ao();
        
        // 读取数字量
        digital_state = drv_soil_read_do();
        
        os_delay_ms(1000);
    }
}
```

### 8.8 注意事项

- AO 固定使用 P1.0（ADC 通道 0）
- DO 固定使用 P1.1
- ADC 值为原始数据，未进行湿度百分比转换
- DO 阈值由模块电位器调节
- 需要根据实际传感器特性确定 ADC 值与湿度的对应关系

---

## 9. 红外传感器

### 9.1 功能简介

红外避障传感器提供障碍物检测功能，通过红外反射检测前方障碍物。

**主要功能：**
- 初始化红外传感器
- 读取避障状态

### 9.2 文件组成

```
红外传感器/
├── ir_init.c      - 红外传感器初始化实现
└── ir_read.c      - 状态读取实现
```

### 9.3 硬件连接

红外传感器连接引脚由 `HRTOS_Sensor.h` 中的宏定义决定：

```c
sbit DRV_IR_IN = P3^2;
```

- **DRV_IR_IN** (P3.2)：红外避障输出

### 9.4 API 参考

#### drv_ir_init()

**函数原型：**
```c
void drv_ir_init(void);
```

**功能：**
初始化红外传感器，设置输入引脚为高电平。

**参数：**
无

**返回值：**
无

**使用说明：**
- 设置 DRV_IR_IN = 1

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    drv_ir_init();
    // 红外传感器已初始化
}
```

---

#### drv_ir_read()

**函数原型：**
```c
unsigned char drv_ir_read(void);
```

**功能：**
读取红外避障状态。

**参数：**
无

**返回值：**
- 0：检测到障碍物
- 1：未检测到障碍物

**使用说明：**
- 直接返回 DRV_IR_IN 引脚电平
- 低电平表示检测到障碍物
- 高电平表示未检测到障碍物

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    unsigned char state;
    
    drv_ir_init();
    
    while(1)
    {
        state = drv_ir_read();
        
        if(state == 0)
        {
            // 检测到障碍物
        }
        else
        {
            // 未检测到障碍物
        }
        
        os_delay_ms(100);
    }
}
```

### 9.5 使用示例

```c
#include "HRTOS_Sensor.h"

void main(void)
{
    drv_ir_init();
    
    while(1)
    {
        if(drv_ir_read() == 0)
        {
            // 障碍物检测
        }
        
        os_delay_ms(100);
    }
}
```

### 9.6 注意事项

- 引脚固定为 P3.2
- 低电平有效（检测到障碍物）
- 高电平表示未检测到障碍物
- 检测距离和灵敏度由模块硬件决定

---

## 10. 雨滴传感器 YL-83

### 10.1 功能简介

雨滴传感器提供模拟输出（AO）和数字输出（DO）两种读取方式，用于检测雨滴或水滴。

**主要功能：**
- 初始化雨滴传感器
- 读取模拟量（ADC）
- 读取数字量（阈值比较）

### 10.2 文件组成

```
雨滴YL-83/
├── raindrop_init.c      - 雨滴传感器初始化实现
├── raindrop_read_ao.c   - 模拟量读取实现
└── raindrop_read_do.c   - 数字量读取实现
```

### 10.3 硬件连接

雨滴传感器连接引脚由 `HRTOS_Sensor.h` 中的宏定义决定：

```c
sbit DRV_RAINDROP_DO = P3^2;
```

- **AO**：模拟输出，连接到 P1.0-P1.7（ADC 通道 0-7）
- **DO**：数字输出，连接到 P3.2

### 10.4 AO 模拟输出

**ADC 配置：**
- 使用 STC12C5A60S2 内置 ADC
- 支持通道 0-7（P1.0-P1.7）
- 10 位分辨率
- 数据范围：0-1023

**数据含义：**
- ADC 原始值，未进行雨量转换
- 值越大表示雨滴越多（需根据实际传感器特性确认）

### 10.5 DO 数字输出

**数字输出：**
- 比较器输出
- 阈值由模块电位器调节
- 高电平：未检测到雨滴
- 低电平：检测到雨滴

### 10.6 API 参考

#### drv_raindrop_init()

**函数原型：**
```c
void drv_raindrop_init(void);
```

**功能：**
初始化雨滴传感器，配置 ADC。

**参数：**
无

**返回值：**
无

**使用说明：**
- 设置 DRV_RAINDROP_DO = 1
- 启动 ADC 电源
- 清零 ADC 寄存器

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    drv_raindrop_init();
    // 雨滴传感器已初始化
}
```

---

#### drv_raindrop_read_ao()

**函数原型：**
```c
unsigned int drv_raindrop_read_ao(unsigned char channel);
```

**功能：**
读取雨滴传感器模拟量。

**参数：**
- `channel`：ADC 通道号（0-7）

**返回值：**
- 10 位 ADC 值，范围 0-1023

**使用说明：**
- 限制通道号为 0-7
- 设置 P1ASF 启用对应通道的 ADC 功能
- 配置 ADC_CONTR：电源 + 速度 + 启动 + 通道
- 等待转换完成
- 清除完成标志
- 计算 10 位结果：`adc_value = (ADC_RES << 2) | (ADC_LOW2 & 0x03)`

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    unsigned int adc_value;
    
    drv_raindrop_init();
    
    // 读取 P1.0 通道
    adc_value = drv_raindrop_read_ao(0);
    
    // 读取 P1.1 通道
    adc_value = drv_raindrop_read_ao(1);
}
```

---

#### drv_raindrop_read_do()

**函数原型：**
```c
unsigned char drv_raindrop_read_do(void);
```

**功能：**
读取雨滴传感器数字量。

**参数：**
无

**返回值：**
- 0：检测到雨滴
- 1：未检测到雨滴

**使用说明：**
- 直接返回 DRV_RAINDROP_DO 引脚电平
- 阈值由模块电位器调节

**示例：**
```c
#include "HRTOS_Sensor.h"

void main(void)
{
    unsigned char state;
    
    drv_raindrop_init();
    
    state = drv_raindrop_read_do();
    
    // state: 0 或 1
}
```

### 10.7 使用示例

```c
#include "HRTOS_Sensor.h"

void main(void)
{
    unsigned int adc_value;
    unsigned char digital_state;
    
    drv_raindrop_init();
    
    while(1)
    {
        // 读取模拟量（P1.0）
        adc_value = drv_raindrop_read_ao(0);
        
        // 读取数字量
        digital_state = drv_raindrop_read_do();
        
        os_delay_ms(1000);
    }
}
```

### 10.8 注意事项

- AO 支持通道 0-7（P1.0-P1.7）
- DO 固定使用 P3.2
- ADC 值为原始数据，未进行雨量转换
- DO 阈值由模块电位器调节
- 需要根据实际传感器特性确定 ADC 值与雨量的对应关系

---

## 11. API 汇总

| 模块       | API                    | 功能           | 参数                    | 返回值               |
|------------|------------------------|----------------|-------------------------|----------------------|
| ADC        | drv_adc_get_value      | 读取 ADC 值    | channel (0-7)           | u16 (0-1023)         |
| DHT11      | drv_dht11_init         | 初始化 DHT11   | 无                      | 无                   |
| DHT11      | drv_dht11_read         | 读取温湿度     | humidity, temperature   | unsigned char        |
| DS1302     | drv_ds1302_init        | 初始化 DS1302  | 无                      | 无                   |
| DS1302     | drv_ds1302_set_time    | 设置时间       | time[7] (BCD)           | 无                   |
| DS1302     | drv_ds1302_read_time   | 读取时间       | time[7] (BCD)           | 无                   |
| DS18B20    | drv_ds18b20_init       | 初始化 DS18B20 | 无                      | 无                   |
| DS18B20    | drv_ds18b20_start      | 启动温度转换   | 无                      | 无                   |
| DS18B20    | drv_ds18b20_read       | 读取温度       | 无                      | signed int (0.01°C)  |
| HC-SR04    | drv_ultrasonic_init    | 初始化超声波   | 无                      | 无                   |
| HC-SR04    | drv_ultrasonic_read    | 读取距离       | 无                      | unsigned int (mm)    |
| HC-SR501   | drv_human_init         | 初始化人体红外 | 无                      | 无                   |
| HC-SR501   | drv_human_read         | 读取人体状态   | 无                      | unsigned char (0/1)  |
| 土壤湿度   | drv_soil_init          | 初始化土壤湿度 | 无                      | 无                   |
| 土壤湿度   | drv_soil_read_ao       | 读取模拟量     | 无                      | unsigned int (0-1023)|
| 土壤湿度   | drv_soil_read_do       | 读取数字量     | 无                      | unsigned char (0/1)  |
| 红外传感器 | drv_ir_init            | 初始化红外     | 无                      | 无                   |
| 红外传感器 | drv_ir_read            | 读取避障状态   | 无                      | unsigned char (0/1)  |
| 雨滴 YL-83 | drv_raindrop_init      | 初始化雨滴     | 无                      | 无                   |
| 雨滴 YL-83 | drv_raindrop_read_ao   | 读取模拟量     | channel (0-7)           | unsigned int (0-1023)|
| 雨滴 YL-83 | drv_raindrop_read_do   | 读取数字量     | 无                      | unsigned char (0/1)  |

**公开 API 总数：21 个**

**内部实现 API：**
- ADC: adc_isr
- DHT11: drv_dht11_delay, drv_dht11_delay_10us, drv_dht11_read_byte
- DS1302: drv_ds1302_read_byte, drv_ds1302_write_byte
- DS18B20: drv_ds18b20_reset, drv_ds18b20_read_bit, drv_ds18b20_read_byte, drv_ds18b20_write_byte
- HC-SR04: drv_ultrasonic_measure, drv_ultrasonic_sort

**外部变量：**
- ADC: drv_adc_channel, drv_adc_value
- DHT11: dht11_humidity_high, dht11_humidity_low, dht11_temperature_high, dht11_temperature_low, dht11_checksum
- HC-SR04: ultrasonic_buffer[8]

---

## 12. 源码与接口一致性检查

### 12.1 已确认

**头文件与源码一致性：**
- HRTOS_Sensor.h 中声明的所有公开 API 均有对应的实现文件
- 所有实现函数的函数名、参数类型、返回值类型与头文件声明一致
- 宏定义（引脚、寄存器、控制位）在实现中正确使用
- 特殊功能寄存器定义与实现一致
- 外部变量在实现文件中正确定义

**文档与源码一致性：**
- 文档中所有 API 均真实存在于 HRTOS_Sensor.h
- 硬件引脚定义与源码一致
- 寄存器和端口操作与源码一致
- 示例代码使用的 API 均为真实存在的公开接口
- 内部实现函数已正确标记

**文件覆盖：**
- ADC：3 个文件，1 个公开 API ✓
- DHT11：5 个文件，2 个公开 API ✓
- DS1302：5 个文件，3 个公开 API ✓
- DS18B20：7 个文件，3 个公开 API ✓
- HC-SR04：4 个文件，2 个公开 API ✓
- HC-SR501：2 个文件，2 个公开 API ✓
- 土壤湿度：3 个文件，3 个公开 API ✓
- 红外传感器：2 个文件，2 个公开 API ✓
- 雨滴 YL-83：3 个文件，3 个公开 API ✓

### 12.2 需要人工确认

**ADC 相关：**

1. **adc_init 未在公开 API 中声明**
   - 问题：adc_init.c 实现了 `drv_adc_init()` 函数，但 HRTOS_Sensor.h 中未声明
   - 状态：函数存在但未公开
   - 影响：用户无法通过头文件调用初始化函数
   - 建议：确认是否需要公开初始化函数或删除该文件

2. **adc_isr 未在公开 API 中声明**
   - 问题：adc_isr.c 实现了中断服务程序，但 HRTOS_Sensor.h 中未声明
   - 状态：内部实现，未公开
   - 影响：用户需要自行配置中断向量
   - 建议：确认是否需要提供中断配置说明

3. **ADC_RESL 寄存器定义不一致**
   - 问题：HRTOS_Sensor.h 定义 ADC_LOW2 = 0xBE，但 adc_get_value.c 定义 ADC_RESL = 0xBE
   - 状态：寄存器名称不一致
   - 影响：可能造成混淆
   - 建议：统一寄存器名称

**DHT11 相关：**

4. **DHT11 校验和验证失败时仍返回数据**
   - 问题：drv_dht11_read() 在校验和失败时仍通过指针返回数据
   - 状态：校验失败处理不完整
   - 影响：用户可能获得错误数据
   - 建议：确认是否需要在校验失败时返回错误码

5. **DHT11 返回值始终为 0**
   - 问题：drv_dht11_read() 函数始终返回 0，未利用返回值指示错误
   - 状态：返回值未使用
   - 影响：无法通过返回值判断读取是否成功
   - 建议：确认是否需要修改返回值含义

6. **DHT11 延时函数精度未明确**
   - 问题：drv_dht11_delay() 和 drv_dht11_delay_10us() 为软件延时，精度取决于系统时钟
   - 状态：延时精度不明确
   - 影响：不同系统时钟下时序可能不准确
   - 建议：确认系统时钟频率或提供延时校准方法

**DS1302 相关：**

7. **DS1302 初始化后未设置时间**
   - 问题：drv_ds1302_init() 仅解除写保护，未设置初始时间
   - 状态：初始化不完整
   - 影响：DS1302 可能包含随机时间数据
   - 建议：确认是否需要在初始化时设置默认时间

**DS18B20 相关：**

8. **DS18B20 复位超时处理不完善**
   - 问题：drv_ds18b20_reset() 超时后调用 os_delay_ms(1)，但超时计数器可能不准确
   - 状态：超时处理可能不可靠
   - 影响：传感器未响应时可能长时间阻塞
   - 建议：确认超时处理逻辑

9. **DS18B20 温度转换未等待完成**
   - 问题：drv_ds18b20_start() 启动转换后立即返回，不等待转换完成
   - 状态：需要用户自行延时
   - 影响：用户可能忘记延时导致读取错误数据
   - 建议：在文档中明确说明需要延时

**HC-SR04 相关：**

10. **HC-SR04 测量超时未处理**
    - 问题：drv_ultrasonic_measure() 等待 ECHO 变高和变低时无超时保护
    - 状态：无超时处理
    - 影响：传感器故障时可能无限阻塞
    - 建议：确认是否需要添加超时保护

11. **HC-SR04 距离计算系数依赖系统时钟**
    - 问题：距离计算系数 0.1953925 基于 11.0592MHz 系统时钟
    - 状态：系数固定
    - 影响：不同系统时钟下距离计算不准确
    - 建议：确认系统时钟频率或提供可配置系数

12. **HC-SR04 滤波功能未启用**
    - 问题：DRV_ULTRASONIC_FILTER_COUNT 设置为 0，未进行滤波
    - 状态：滤波功能存在但未使用
    - 影响：测量结果可能不稳定
    - 建议：确认是否需要启用滤波或删除相关代码

**土壤湿度相关：**

13. **土壤湿度 AO 固定使用通道 0**
    - 问题：drv_soil_read_ao() 固定使用通道 0，不支持参数选择
    - 状态：功能受限
    - 影响：无法灵活选择 ADC 通道
    - 建议：确认是否需要支持通道参数

**雨滴传感器相关：**

14. **雨滴传感器 DO 引脚与红外传感器冲突**
    - 问题：DRV_RAINDROP_DO 和 DRV_IR_IN 都定义为 P3.2
    - 状态：引脚冲突
    - 影响：两个传感器无法同时使用
    - 建议：确认实际硬件连接或修改引脚定义

**通用问题：**

15. **u8/u16 类型未定义**
    - 问题：HRTOS_Sensor.h 中使用了 u8 和 u16 类型，但未定义
    - 状态：依赖外部类型定义
    - 影响：需要确认类型定义位置
    - 建议：确认 u8/u16 类型在何处定义或在头文件中添加定义

16. **多个传感器禁用中断**
    - 问题：DHT11、DS1302、DS18B20、HC-SR04 在操作时关闭中断
    - 状态：中断禁用影响系统实时性
    - 影响：在 HRTOS 任务中使用可能影响其他任务
    - 建议：确认在 HRTOS 环境中的使用方式

---

## 附录

### A. 依赖头文件

所有驱动实现依赖以下头文件：
```c
#include "hrtos_hal.h"
```

DS18B20 和 DS1302 额外依赖：
```c
#include <intrins.h>  // _nop_ 函数
```

### B. 编译说明

使用本模块时，需将以下文件加入编译：
- HRTOS_Sensor.h
- 对应驱动的 .c 实现文件

### C. 版本信息

- 文档版本：1.0
- 基于 HRTOS Driver Library 03_Sensor 模块源码生成
- 适用芯片：STC12C5A60S2 等 51 单片机
