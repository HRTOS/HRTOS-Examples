# 05 控制设备驱动

## 1. 模块概述

05_Control 模块提供 HRTOS 实时操作系统的控制设备驱动支持，包括 PWM 脉宽调制输出和继电器控制功能。

本模块通过 `HRTOS_Control.h` 头文件向用户公开 API 接口，所有驱动函数均以 `drv_` 前缀命名。

**支持的控制设备：**
- PWM：基于 PCA 模块的 PWM 输出（适用于 STC12C5A60S2 等支持 PCA/PWM 的 51 单片机）
- Relay：继电器控制（高电平触发）

**头文件：**
```c
#include "HRTOS_Control.h"
```

**依赖：**
- 所有驱动实现依赖 `hrtos_hal.h` 硬件抽象层
- PWM 驱动使用 STC12C5A60S2 的 PCA（可编程计数器阵列）模块

---

## 2. PWM

### 2.1 功能简介

PWM 驱动提供基于 PCA 模块的 PWM 输出功能，支持占空比设置、读取、启动和停止。适用于 STC12C5A60S2 等支持 PCA/PWM 的 51 单片机。

**主要功能：**
- 初始化 PCA PWM
- 设置 PWM 占空比（0-100%）
- 获取当前 PWM 占空比
- 启动/停止 PWM 输出

### 2.2 文件组成

```
PWM/
├── pwm_init.c       - PWM 初始化实现
├── pwm_set_duty.c   - 占空比设置实现
├── pwm_get_duty.c   - 占空比读取实现
├── pwm_start.c      - PWM 启动实现
└── pwm_stop.c       - PWM 停止实现
```

### 2.3 硬件连接

PWM 输出引脚由 `HRTOS_Control.h` 中的配置决定：

```c
sfr AUXR1   = 0xA2;     /* 辅助寄存器1 */
sfr P1M1    = 0x91;     /* P1口模式寄存器1 */
sfr P1M0    = 0x92;     /* P1口模式寄存器0 */
```

- **默认输出引脚：** P1.3（PCA0/PWM0）
- **可选输出引脚：** P4.2（通过 AUXR1.6 切换）
- **引脚切换：**
  - AUXR1.6 = 0：PCA/PWM 使用 P1 口（默认）
  - AUXR1.6 = 1：PCA/PWM 切换至 P4 口

**PCA 特殊功能寄存器：**
- `CCON` (0xD8)：PCA 控制寄存器
- `CMOD` (0xD9)：PCA 工作模式寄存器
- `CCAPM0` (0xDA)：PCA 模块 0 模式寄存器
- `CL` (0xE9)：PCA 计数器低 8 位
- `CH` (0xF9)：PCA 计数器高 8 位
- `CCAP0L` (0xEA)：PCA 模块 0 捕获/比较寄存器低 8 位
- `CCAP0H` (0xFA)：PCA 模块 0 捕获/比较寄存器高 8 位
- `CR` (CCON.6)：PCA 运行控制位

### 2.4 PWM 工作方式

**PCA 配置：**
- 使用 PCA 模块 0 输出 PWM
- 8 位 PWM 模式
- 时钟源：FOSC/2（系统时钟二分频）
- 初始化后自动启动 PCA

**占空比表示：**
- 用户接口范围：0-100（0% 到 100%）
- 内部寄存器范围：0-255
- 转换公式：`pwm_value = (duty * 255) / 100`
- 寄存器值反转：`CCAP0H/L = 255 - pwm_value`
  - CCAP0H/L = 0x00 → 100% 占空比
  - CCAP0H/L = 0xFF → 0% 占空比

**输出特性：**
- 反转输出逻辑（低电平有效）
- 初始占空比：0%（输出高电平）

### 2.5 API 参考

#### drv_pwm_init()

**函数原型：**
```c
void drv_pwm_init(void);
```

**功能：**
初始化 PCA PWM 模块，配置为 8 位 PWM 模式，时钟源为 FOSC/2，初始占空比为 0%。

**参数：**
无

**返回值：**
无

**使用说明：**
- 配置 P1.3 为推挽输出模式
- 设置 PCA 使用 P1 口（不切换到 P4）
- 清除 PCA 控制寄存器
- 清零 PCA 计数器
- 设置 PCA 时钟为 FOSC/2
- 配置 PCA 模块 0 为 PWM 模式
- 设置初始占空比为 0%（CCAP0H/L = 0xFF）
- 自动启动 PCA（CR = 1）

**示例：**
```c
#include "HRTOS_Control.h"

void main(void)
{
    drv_pwm_init();
    // PWM 已初始化并启动，占空比 0%
}
```

---

#### drv_pwm_set_duty()

**函数原型：**
```c
void drv_pwm_set_duty(u8 duty);
```

**功能：**
设置 PWM 占空比。

**参数：**
- `duty`：PWM 占空比，范围 0-100
  - 0：0% 占空比
  - 100：100% 占空比

**返回值：**
无

**使用说明：**
- 参数超过 100 时自动限制为 100
- 占空比保存到全局变量 `pwm_duty`
- 转换公式：`pwm_value = (duty * 255) / 100`
- 寄存器值反转：`CCAP0H/L = 255 - pwm_value`
- 实时生效，无需重启 PWM

**示例：**
```c
drv_pwm_set_duty(50);  // 设置 50% 占空比
drv_pwm_set_duty(100); // 设置 100% 占空比
```

---

#### drv_pwm_get_duty()

**函数原型：**
```c
u8 drv_pwm_get_duty(void);
```

**功能：**
获取当前 PWM 占空比。

**参数：**
无

**返回值：**
- 当前 PWM 占空比，范围 0-100

**使用说明：**
- 返回全局变量 `pwm_duty` 的值
- 返回的是通过 `drv_pwm_set_duty()` 设置的值

**示例：**
```c
u8 current_duty;
current_duty = drv_pwm_get_duty();  // 读取当前占空比
```

---

#### drv_pwm_start()

**函数原型：**
```c
void drv_pwm_start(void);
```

**功能：**
启动 PCA PWM 输出。

**参数：**
无

**返回值：**
无

**使用说明：**
- 设置 CR = 1 启动 PCA 计数器
- 如果 PCA 已经运行，此函数无副作用

**示例：**
```c
drv_pwm_stop();
// ... 其他操作
drv_pwm_start();  // 重新启动 PWM
```

---

#### drv_pwm_stop()

**函数原型：**
```c
void drv_pwm_stop(void);
```

**功能：**
停止 PCA PWM 输出。

**参数：**
无

**返回值：**
无

**使用说明：**
- 设置 CR = 0 停止 PCA 计数器
- 停止后 PWM 输出保持当前状态
- 占空比设置保持不变

**示例：**
```c
drv_pwm_stop();  // 停止 PWM 输出
```

### 2.6 初始化

PWM 初始化流程：
1. 设置初始占空比为 0
2. 配置 P1.3 为推挽输出模式
3. 设置 PCA 使用 P1 口（AUXR1.6 = 0）
4. 清除 PCA 控制寄存器（CCON = 0x00）
5. 清零 PCA 计数器（CL = 0, CH = 0）
6. 设置 PCA 时钟为 FOSC/2（CMOD = 0x02）
7. 配置 PCA 模块 0 为 PWM 模式（CCAPM0 = 0x42）
8. 设置初始占空比为 0%（CCAP0H = 0xFF, CCAP0L = 0xFF）
9. 启动 PCA（CR = 1）

### 2.7 启动与停止

- **启动：** 调用 `drv_pwm_start()` 设置 CR = 1
- **停止：** 调用 `drv_pwm_stop()` 设置 CR = 0
- 初始化后自动启动，无需手动调用 `drv_pwm_start()`

### 2.8 占空比读取与设置

- **设置：** 调用 `drv_pwm_set_duty(duty)` 设置占空比
- **读取：** 调用 `drv_pwm_get_duty()` 读取当前占空比
- 占空比范围：0-100
- 实时生效，无需重启

### 2.9 使用示例

```c
#include "HRTOS_Control.h"

void main(void)
{
    // 初始化 PWM
    drv_pwm_init();
    
    // 设置占空比为 50%
    drv_pwm_set_duty(50);
    
    // 运行一段时间
    // ...
    
    // 调整占空比为 75%
    drv_pwm_set_duty(75);
    
    // 停止 PWM
    drv_pwm_stop();
    
    // 重新启动 PWM
    drv_pwm_start();
    
    // 设置占空比为 25%
    drv_pwm_set_duty(25);
    
    while(1)
    {
        // 主循环
    }
}
```

### 2.10 注意事项

- PWM 驱动仅适用于支持 PCA/PWM 的 STC 单片机（如 STC12C5A60S2）
- 默认输出引脚为 P1.3，可通过 AUXR1.6 切换到 P4.2
- PWM 时钟源为 FOSC/2，实际频率取决于系统时钟
- 占空比范围为 0-100，超出范围自动限制
- 输出逻辑为反转（低电平有效）
- 初始化后自动启动 PCA
- 占空比设置实时生效
- 停止 PWM 后输出保持当前状态
- 全局变量 `pwm_duty` 用于保存当前占空比

---

## 3. Relay

### 3.1 功能简介

继电器驱动提供继电器的初始化、开启、关闭、状态设置和状态读取功能。按照高电平触发设计。

**主要功能：**
- 初始化继电器
- 吸合继电器（开启）
- 释放继电器（关闭）
- 设置继电器状态
- 获取继电器状态

### 3.2 文件组成

```
Relay/
├── relay_init.c      - 继电器初始化实现
├── relay_on.c        - 继电器开启实现
├── relay_off.c       - 继电器关闭实现
├── relay_set.c       - 继电器状态设置实现
└── relay_get.c       - 继电器状态读取实现
```

### 3.3 硬件连接

继电器连接引脚由 `HRTOS_Control.h` 中的宏定义决定：

```c
sbit RELAY_IN = P1^4;
sbit RELAY_EN = P0^0;
```

- **RELAY_IN** (P1.4)：继电器输入控制引脚
- **RELAY_EN** (P0.0)：继电器使能控制引脚（GND/使能控制）

**硬件连接说明：**
- 继电器模块 IN → P1.4
- 继电器模块 GND/使能控制 → P0.0

**触发方式：**
- 高电平触发（高电平吸合继电器）

### 3.4 API 参考

#### drv_relay_init()

**函数原型：**
```c
void drv_relay_init(void);
```

**功能：**
初始化继电器，设置初始状态为关闭。

**参数：**
无

**返回值：**
无

**使用说明：**
- 设置初始状态为关闭（relay_state = 0）
- 设置 RELAY_IN = 0
- 设置 RELAY_EN = 1（使能继电器模块）

**示例：**
```c
#include "HRTOS_Control.h"

void main(void)
{
    drv_relay_init();
    // 继电器已初始化并关闭
}
```

---

#### drv_relay_on()

**函数原型：**
```c
void drv_relay_on(void);
```

**功能：**
吸合继电器（开启）。

**参数：**
无

**返回值：**
无

**使用说明：**
- 设置 RELAY_EN = 1（使能）
- 设置 RELAY_IN = 1（高电平触发）
- 更新状态为开启（relay_state = 1）

**示例：**
```c
drv_relay_on();  // 吸合继电器
```

---

#### drv_relay_off()

**函数原型：**
```c
void drv_relay_off(void);
```

**功能：**
释放继电器（关闭）。

**参数：**
无

**返回值：**
无

**使用说明：**
- 设置 RELAY_IN = 0（低电平）
- 设置 RELAY_EN = 1（保持使能）
- 更新状态为关闭（relay_state = 0）

**示例：**
```c
drv_relay_off();  // 释放继电器
```

---

#### drv_relay_set()

**函数原型：**
```c
void drv_relay_set(u8 state);
```

**功能：**
设置继电器状态。

**参数：**
- `state`：继电器状态
  - 0：关闭
  - 1：开启

**返回值：**
无

**使用说明：**
- state = 1 时调用 `drv_relay_on()`
- state = 0 时调用 `drv_relay_off()`
- 非 0 值视为开启

**示例：**
```c
drv_relay_set(1);  // 开启继电器
drv_relay_set(0);  // 关闭继电器
```

---

#### drv_relay_get()

**函数原型：**
```c
u8 drv_relay_get(void);
```

**功能：**
获取继电器当前状态。

**参数：**
无

**返回值：**
- 0：关闭
- 1：开启

**使用说明：**
- 返回全局变量 `relay_state` 的值
- 返回的是通过 API 设置的逻辑状态，而非实际引脚电平

**示例：**
```c
u8 state;
state = drv_relay_get();  // 读取继电器状态
if(state == 1)
{
    // 继电器开启
}
```

### 3.5 初始化

继电器初始化流程：
1. 设置初始状态为关闭（relay_state = 0）
2. 设置 RELAY_IN = 0
3. 设置 RELAY_EN = 1（使能继电器模块）

### 3.6 开启与关闭

- **开启：** 调用 `drv_relay_on()` 设置 RELAY_IN = 1（高电平触发）
- **关闭：** 调用 `drv_relay_off()` 设置 RELAY_IN = 0
- 两种操作都保持 RELAY_EN = 1（使能状态）

### 3.7 状态读取与设置

- **设置：** 调用 `drv_relay_set(state)` 设置状态
- **读取：** 调用 `drv_relay_get()` 读取状态
- 状态值：0=关闭，1=开启
- 状态保存在全局变量 `relay_state`

### 3.8 使用示例

```c
#include "HRTOS_Control.h"

void main(void)
{
    u8 state;
    
    // 初始化继电器
    drv_relay_init();
    
    // 开启继电器
    drv_relay_on();
    
    // 读取状态
    state = drv_relay_get();
    
    // 关闭继电器
    drv_relay_off();
    
    // 使用 set 函数控制
    drv_relay_set(1);  // 开启
    drv_relay_set(0);  // 关闭
    
    while(1)
    {
        // 主循环
    }
}
```

### 3.9 注意事项

- 继电器为高电平触发设计
- RELAY_IN (P1.4) 为控制引脚
- RELAY_EN (P0.0) 为使能引脚，初始化后保持高电平
- 状态保存在全局变量 `relay_state`
- `drv_relay_get()` 返回的是逻辑状态，而非实际引脚电平
- 初始化后继电器处于关闭状态

---

## 4. API 汇总

| 模块   | API                | 功能           | 参数          | 返回值   |
|--------|--------------------|----------------|---------------|----------|
| PWM    | drv_pwm_init       | PWM 初始化     | 无            | 无       |
| PWM    | drv_pwm_set_duty   | 设置占空比     | duty (0-100)  | 无       |
| PWM    | drv_pwm_get_duty   | 获取占空比     | 无            | u8 (0-100) |
| PWM    | drv_pwm_start      | 启动 PWM       | 无            | 无       |
| PWM    | drv_pwm_stop       | 停止 PWM       | 无            | 无       |
| Relay  | drv_relay_init     | 继电器初始化   | 无            | 无       |
| Relay  | drv_relay_on       | 开启继电器     | 无            | 无       |
| Relay  | drv_relay_off      | 关闭继电器     | 无            | 无       |
| Relay  | drv_relay_set      | 设置继电器状态 | state (0/1)   | 无       |
| Relay  | drv_relay_get      | 获取继电器状态 | 无            | u8 (0/1) |

**公开 API 总数：10 个**

**外部变量：**
- `pwm_duty` (u8)：当前 PWM 占空比
- `relay_state` (u8)：当前继电器状态

---

## 5. 源码与接口一致性检查

### 5.1 已确认

**头文件与源码一致性：**
- HRTOS_Control.h 中声明的所有公开 API 均有对应的实现文件
- 所有实现函数的函数名、参数类型、返回值类型与头文件声明一致
- 宏定义（AUXR1, P1M1, P1M0, RELAY_IN, RELAY_EN）在实现中正确使用
- 特殊功能寄存器（CCON, CMOD, CCAPM0, CL, CH, CCAP0L, CCAP0H）定义与实现一致
- 外部变量（pwm_duty, relay_state）在实现文件中正确定义

**文档与源码一致性：**
- 文档中所有 API 均真实存在于 HRTOS_Control.h
- PWM 硬件配置（P1.3 输出、PCA 模块 0、8 位 PWM、FOSC/2 时钟）与源码一致
- PWM 占空比转换公式与源码一致
- PWM 反转输出逻辑与源码一致
- 继电器硬件连接（P1.4、P0.0）与源码一致
- 继电器高电平触发与源码一致
- 示例代码使用的 API 均为真实存在的公开接口

**文件覆盖：**
- PWM：5 个文件，5 个 API ✓
- Relay：5 个文件，5 个 API ✓

### 5.2 需要人工确认

**硬件连接说明：**

1. **继电器硬件连接描述不一致**
   - 问题：HRTOS_Control.h 注释中说明 "IN -> P0.0, GND/使能控制 -> P1.4"，但实际宏定义是 RELAY_IN = P1.4, RELAY_EN = P0.0
   - 状态：注释与实际代码定义相反
   - 影响：用户可能混淆引脚连接
   - 建议：确认实际硬件连接方式并更新注释

**PWM 频率说明：**

2. **PWM 频率未明确说明**
   - 问题：源码中设置 PCA 时钟为 FOSC/2，但未说明系统时钟频率
   - 状态：无法确定实际 PWM 频率
   - 影响：用户无法计算实际 PWM 频率
   - 建议：确认系统时钟频率或在文档中说明 PWM 频率计算方法

**引脚切换功能：**

3. **PWM 引脚切换功能未实现**
   - 问题：HRTOS_Control.h 中注释了 PCA_P4 引脚切换功能（AUXR1.6），但代码中该行被注释且未提供切换 API
   - 状态：功能已定义但未实现
   - 影响：用户无法切换 PWM 输出引脚
   - 建议：确认是否需要实现引脚切换功能或删除相关注释

**类型定义：**

4. **u8 类型未定义**
   - 问题：HRTOS_Control.h 中注释了 `typedef unsigned char u8;` 但实际未定义
   - 状态：依赖外部类型定义
   - 影响：需要确认 u8 类型在何处定义
   - 建议：确认 u8 类型定义位置或在头文件中添加定义

**继电器使能引脚作用：**

5. **RELAY_EN 引脚作用不明确**
   - 问题：注释说明为 "GND/使能控制"，但实际使用中始终设置为 1
   - 状态：引脚作用不清晰
   - 影响：用户可能不理解该引脚的实际作用
   - 建议：确认 RELAY_EN 引脚的具体作用和使用场景

**状态变量同步：**

6. **继电器状态变量未同步实际引脚**
   - 问题：`drv_relay_get()` 返回全局变量 `relay_state`，而非读取实际引脚电平
   - 状态：状态变量可能与实际硬件状态不同步
   - 影响：如果直接操作引脚，状态变量可能不准确
   - 建议：确认是否需要从引脚读取实际状态

**PWM 输出逻辑：**

7. **PWM 反转输出逻辑需注意**
   - 问题：PWM 输出为反转逻辑（0x00=100%，0xFF=0%），与常规直觉相反
   - 状态：实现正确但可能造成用户混淆
   - 影响：用户可能误解占空比设置
   - 建议：在文档中明确说明反转逻辑

**资源占用：**

8. **PCA 模块资源占用**
   - 问题：PWM 占用 PCA 模块 0，可能与其他功能冲突
   - 状态：资源占用已明确
   - 影响：用户需注意 PCA 模块资源冲突
   - 建议：在文档中明确说明 PCA 模块资源占用情况

---

## 附录

### A. 依赖头文件

所有驱动实现依赖以下头文件：
```c
#include "hrtos_hal.h"
```

### B. 编译说明

使用本模块时，需将以下文件加入编译：
- HRTOS_Control.h
- 对应驱动的 .c 实现文件

### C. 版本信息

- 文档版本：1.0
- 基于 HRTOS Driver Library 05_Control 模块源码生成
- 适用芯片：STC12C5A60S2 等支持 PCA/PWM 的 51 单片机
