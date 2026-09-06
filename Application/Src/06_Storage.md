# 06 存储设备驱动

## 1. 模块概述

06_Storage 模块提供 HRTOS 实时操作系统的存储设备驱动支持，包括 24C02 EEPROM 驱动和底层 I²C 通信实现。

本模块通过 `HRTOS_Storage.h` 头文件向用户公开 API 接口。

**支持的存储设备：**
- 24C02 EEPROM（2Kbit = 256 字节）

**头文件：**
```c
#include "HRTOS_Storage.h"
```

**依赖：**
- 所有驱动实现依赖 `hrtos_hal.h` 硬件抽象层
- I²C 延时依赖 `<intrins.h>`（_nop_ 函数）

---

## 2. 24C02 EEPROM

### 2.1 功能简介

24C02 驱动提供 24C02 EEPROM 的单字节读写功能，通过软件 I²C 总线通信。24C02 是 2Kbit（256 字节）串行 EEPROM。

**主要功能：**
- 单字节写入
- 单字节读取
- I²C 通信底层接口

### 2.2 文件组成

```
EEPROM/EEPROM-IIC/
├── 24c02_read.c       - EEPROM 读取实现
├── 24c02_write.c      - EEPROM 写入实现
├── i2c.c              - I²C 头文件引用（空实现）
├── i2c_delay.c        - I²C 延时实现
├── i2c_read_byte.c    - I²C 字节读取实现
├── i2c_start.c        - I²C 起始条件实现
├── i2c_stop.c         - I²C 停止条件实现
└── i2c_write_byte.c   - I²C 字节写入实现
```

### 2.3 硬件连接

24C02 EEPROM 连接引脚由 `HRTOS_Storage.h` 中的宏定义决定：

```c
sbit EEPROM_SCL = P2^1;
sbit EEPROM_SDA = P2^0;
```

- **EEPROM_SDA** (P2.0)：I²C 数据线
- **EEPROM_SCL** (P2.1)：I²C 时钟线

**硬件要求：**
- SDA 和 SCL 需要外部上拉电阻（典型值 4.7kΩ-10kΩ）
- 驱动未实现内部上拉，依赖外部电路

### 2.4 存储空间

**24C02 容量：**
- 总容量：2Kbit = 256 字节
- 地址范围：0x00-0xFF（0-255）
- 地址类型：8 位（unsigned char）

### 2.5 通信方式

**I²C 通信：**
- 软件模拟 I²C
- 标准模式（100kHz）
- 7 位从机地址 + 读写位

**24C02 器件地址：**
- 写地址：0xA0（1010 0000）
- 读地址：0xA1（1010 0001）
- 固定地址，不可配置

### 2.6 API 参考

#### eeprom_24c02_write()

**函数原型：**
```c
void eeprom_24c02_write(unsigned char addr, unsigned char dat);
```

**功能：**
向 24C02 EEPROM 指定地址写入一个字节数据。

**参数：**
- `addr`：EEPROM 存储地址（0x00-0xFF）
- `dat`：要写入的数据（0x00-0xFF）

**返回值：**
无

**使用说明：**
- 发送 I²C 起始条件
- 发送器件写地址（0xA0）
- 发送存储地址
- 发送数据
- 发送 I²C 停止条件
- 等待 4 个 eeprom_delay() 延时（约 8 个 _nop_）以确保写周期完成
- 当前实现仅支持单字节写入
- 写入后需要等待写周期完成（约 5ms，但当前延时较短）

**示例：**
```c
#include "HRTOS_Storage.h"

void main(void)
{
    // 向地址 0x10 写入数据 0x55
    eeprom_24c02_write(0x10, 0x55);
    
    // 向地址 0x20 写入数据 0xAA
    eeprom_24c02_write(0x20, 0xAA);
}
```

---

#### eeprom_24c02_read()

**函数原型：**
```c
unsigned char eeprom_24c02_read(unsigned char addr);
```

**功能：**
从 24C02 EEPROM 指定地址读取一个字节数据。

**参数：**
- `addr`：EEPROM 存储地址（0x00-0xFF）

**返回值：**
- 读取的数据（0x00-0xFF）

**使用说明：**
- 发送 I²C 起始条件
- 发送器件写地址（0xA0）
- 发送存储地址
- 发送 I²C 起始条件（重复起始）
- 发送器件读地址（0xA1）
- 读取一个字节数据
- 发送 NACK（不发送 ACK）
- 发送 I²C 停止条件
- 当前实现仅支持单字节读取
- 读取操作无需等待写周期

**示例：**
```c
#include "HRTOS_Storage.h"

void main(void)
{
    unsigned char data;
    
    // 从地址 0x10 读取数据
    data = eeprom_24c02_read(0x10);
    
    // 从地址 0x20 读取数据
    data = eeprom_24c02_read(0x20);
}
```

### 2.7 数据读取

调用 `eeprom_24c02_read(addr)` 读取数据：
- 地址范围：0x00-0xFF
- 返回值：读取的字节数据
- 使用随机读取方式（先发送地址再读取）

### 2.8 数据写入

调用 `eeprom_24c02_write(addr, dat)` 写入数据：
- 地址范围：0x00-0xFF
- 数据范围：0x00-0xFF
- 写入后等待 4 个 eeprom_delay() 延时
- 当前延时较短，可能不足以完成完整写周期

### 2.9 地址说明

**地址类型：**
- EEPROM 内部地址：8 位（unsigned char）
- 地址范围：0x00-0xFF（0-255）
- 对应 256 字节存储空间

**I²C 器件地址：**
- 固定为 0xA0（写）/ 0xA1（读）
- 由芯片硬件决定，不可配置

### 2.10 使用示例

```c
#include "HRTOS_Storage.h"

void main(void)
{
    unsigned char data;
    
    // 写入数据
    eeprom_24c02_write(0x00, 0x12);
    eeprom_24c02_write(0x01, 0x34);
    eeprom_24c02_write(0x02, 0x56);
    eeprom_24c02_write(0x03, 0x78);
    
    // 延时等待写周期完成（建议增加延时）
    os_delay_ms(10);
    
    // 读取数据
    data = eeprom_24c02_read(0x00);  // 应返回 0x12
    data = eeprom_24c02_read(0x01);  // 应返回 0x34
    data = eeprom_24c02_read(0x02);  // 应返回 0x56
    data = eeprom_24c02_read(0x03);  // 应返回 0x78
}
```

### 2.11 注意事项

- 当前实现仅支持单字节读写
- 写入后延时较短（约 8 个 _nop_），建议增加延时至 5-10ms
- SDA 和 SCL 需要外部上拉电阻
- 引脚固定为 P2.0 (SDA) 和 P2.1 (SCL)
- 器件地址固定为 0xA0/0xA1
- 未实现 ACK Polling 机制
- 未实现页写功能
- 未实现连续读写功能
- I²C 通信为软件模拟，时序精度取决于系统时钟

---

## 3. I²C 底层实现

### 3.1 功能简介

I²C 底层实现提供软件模拟 I²C 总线通信的基本功能，包括起始条件、停止条件、字节写入和字节读取。

### 3.2 文件组成

```
EEPROM/EEPROM-IIC/
├── i2c.c              - 头文件引用（内部实现文件）
├── i2c_delay.c        - 延时实现（内部实现文件）
├── i2c_read_byte.c    - 字节读取实现（内部实现文件）
├── i2c_start.c        - 起始条件实现（内部实现文件）
├── i2c_stop.c         - 停止条件实现（内部实现文件）
└── i2c_write_byte.c   - 字节写入实现（内部实现文件）
```

### 3.3 I²C 时序

**时钟特性：**
- 软件模拟 I²C
- 时钟频率取决于 eeprom_delay() 延时和系统时钟
- 延时函数：2 个 _nop_() 调用

### 3.4 起始条件

**eeprom_i2c_start()** 为内部实现：
- SDA = 1
- 延时
- SCL = 1
- 延时
- SDA = 0
- 延时
- SCL = 0
- 延时

### 3.5 停止条件

**eeprom_i2c_stop()** 为内部实现：
- SDA = 0
- 延时
- SCL = 1
- 延时
- SDA = 1
- 延时

### 3.6 字节写入

**eeprom_i2c_write_byte()** 为内部实现：
- 逐位发送 8 位数据（MSB 先传）
- 每位：SDA = 数据位，延时，SCL = 1，延时，SCL = 0，延时
- 发送完成后释放 SDA（SDA = 1）
- 等待 ACK（SDA 变低）
- 超时检测：200 次循环
- 返回值：1 = 收到 ACK，0 = 未收到 ACK（超时）

### 3.7 字节读取

**eeprom_i2c_read_byte()** 为内部实现：
- 逐位读取 8 位数据（MSB 先传）
- 每位：SCL = 1，延时，读取 SDA，数据左移，SCL = 0，延时
- 返回读取的字节数据
- 不发送 ACK/NACK（由调用者处理）

### 3.8 ACK 处理

**ACK 检测：**
- 在字节写入后检测 SDA 电平
- SDA = 0 表示收到 ACK
- SDA = 1 表示未收到 ACK（NACK）
- 超时时间：200 次循环

**NACK 发送：**
- 在字节读取后由调用者发送 NACK
- 24c02_read.c 中手动发送 NACK（SDA = 1，SCL = 1，延时，SCL = 0）

---

## 4. API 汇总

| 模块   | API                    | 功能           | 参数                    | 返回值               |
|--------|------------------------|----------------|-------------------------|----------------------|
| 24C02  | eeprom_24c02_write     | 写入一个字节   | addr, dat               | 无                   |
| 24C02  | eeprom_24c02_read      | 读取一个字节   | addr                    | unsigned char        |
| I²C    | eeprom_i2c_start       | I²C 起始条件   | 无                      | 无                   |
| I²C    | eeprom_i2c_stop        | I²C 停止条件   | 无                      | 无                   |
| I²C    | eeprom_i2c_write_byte  | I²C 写入字节   | dat                     | unsigned char (0/1)  |
| I²C    | eeprom_i2c_read_byte   | I²C 读取字节   | 无                      | unsigned char        |
| I²C    | eeprom_delay           | I²C 延时       | 无                      | 无                   |

**公开 API 总数：** 7 个

**内部实现文件：**
- i2c.c：空实现，仅包含头文件引用
- i2c_delay.c：延时实现
- i2c_read_byte.c：字节读取实现
- i2c_start.c：起始条件实现
- i2c_stop.c：停止条件实现
- i2c_write_byte.c：字节写入实现

**注意：**
- I²C 底层函数（eeprom_i2c_*）在 HRTOS_Storage.h 中声明为公开 API
- 应用程序可以直接调用 I²C 底层函数
- 但建议优先使用 24C02 读写接口（eeprom_24c02_*）

---

## 5. 源码与接口一致性检查

### 5.1 已确认

**头文件与源码一致性：**
- HRTOS_Storage.h 中声明的所有公开 API 均有对应的实现文件
- 所有实现函数的函数名、参数类型、返回值类型与头文件声明一致
- 宏定义（引脚）在实现中正确使用
- 示例代码使用的 API 均为真实存在的公开接口

**文档与源码一致性：**
- 文档中所有 API 均真实存在于 HRTOS_Storage.h
- 硬件引脚定义与源码一致
- I²C 器件地址（0xA0/0xA1）与源码一致
- 读写流程与源码一致
- 延时实现与源码一致

**文件覆盖：**
- EEPROM-IIC：8 个文件，7 个公开 API ✓

### 5.2 需要人工确认

**写入延时问题：**

1. **写入延时不足**
   - 问题：eeprom_24c02_write() 写入后仅等待 4 个 eeprom_delay()（约 8 个 _nop_）
   - 状态：延时远小于 24C02 写周期要求（典型 5ms）
   - 影响：写入后立即读取可能读取到错误数据
   - 建议：增加延时至 5-10ms 或实现 ACK Polling 机制

**I²C 底层函数公开问题：**

2. **I²C 底层函数作为公开 API**
   - 问题：eeprom_i2c_* 函数在 HRTOS_Storage.h 中声明为公开 API
   - 状态：底层实现函数对用户公开
   - 影响：用户可能误用底层函数，导致时序错误
   - 建议：确认是否需要将 I²C 底层函数标记为内部实现

**i2c.c 空文件：**

3. **i2c.c 文件为空**
   - 问题：i2c.c 仅包含头文件引用，无实际代码
   - 状态：文件存在但无实现
   - 影响：可能造成混淆
   - 建议：确认是否需要删除该文件或添加实现

**ACK Polling 未实现：**

4. **未实现 ACK Polling**
   - 问题：写入后使用固定延时，未实现 ACK Polling
   - 状态：写周期检测不完善
   - 影响：可能浪费等待时间或写入未完成就继续操作
   - 建议：确认是否需要实现 ACK Polling 机制

**功能限制：**

5. **仅支持单字节读写**
   - 问题：当前实现仅支持单字节读写
   - 状态：功能受限
   - 影响：批量数据读写效率低
   - 建议：确认是否需要实现连续读写功能

6. **未实现页写功能**
   - 问题：24C02 支持页写（8 字节/页），但当前驱动未实现
   - 状态：未利用芯片功能
   - 影响：写入效率较低
   - 建议：确认是否需要实现页写功能

**延时精度：**

7. **I²C 延时精度未明确**
   - 问题：eeprom_delay() 为 2 个 _nop_()，实际延时取决于系统时钟
   - 状态：延时精度不明确
   - 影响：不同系统时钟下 I²C 时序可能不准确
   - 建议：确认系统时钟频率或提供延时校准方法

**错误处理：**

8. **写入函数无返回值**
   - 问题：eeprom_24c02_write() 无返回值，无法判断写入是否成功
   - 状态：错误处理不完善
   - 影响：写入失败时无法感知
   - 建议：确认是否需要返回写入状态

**外部上拉：**

9. **外部上拉电阻要求未明确**
   - 问题：源码未实现内部上拉，依赖外部电路
   - 状态：硬件要求未明确说明
   - 影响：用户可能忘记添加上拉电阻
   - 建议：在文档中明确说明需要外部上拉电阻

**HRTOS 兼容性：**

10. **I²C 操作未关闭中断**
    - 问题：I²C 操作未关闭中断
    - 状态：时序可能受中断影响
    - 影响：在 HRTOS 任务中使用可能影响时序准确性
    - 建议：确认是否需要在 I²C 操作时关闭中断

---

## 附录

### A. 依赖头文件

所有驱动实现依赖以下头文件：
```c
#include "hrtos_hal.h"
```

I²C 延时额外依赖：
```c
#include <intrins.h>  // _nop_ 函数
```

### B. 编译说明

使用本模块时，需将以下文件加入编译：
- HRTOS_Storage.h
- EEPROM/EEPROM-IIC/ 下所有 .c 文件

### C. 版本信息

- 文档版本：1.0
- 基于 HRTOS Driver Library 06_Storage 模块源码生成
- 适用芯片：STC12C5A60S2 等 51 单片机
- 适用器件：24C02 EEPROM（2Kbit）
