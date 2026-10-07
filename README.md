# HRTOS Examples

HRTOS 官方示例仓库。

本仓库提供基于 **51 单片机 HRTOS** 的示例源码、任务机制示例以及常用硬件驱动，用于展示 HRTOS 的基本使用方法、内核机制和实际硬件应用能力。

## 示例内容

### Basic

基础入门示例，用于快速了解 HRTOS 的基本运行方式和主要内核功能。

包含：

* HRTOS 基础启动
* 事件机制
* 中断处理
* 任务删除
* 任务抢占
* 时间片调度
* 信号量
* 互斥锁
* 消息队列
* 邮箱
* 快速任务
* 任务切换控制
* 中断嵌套

### Task

内核机制示例，用于深入理解 HRTOS 的任务模型、调度机制和实时行为。

包含：

* 任务创建与执行
* 上下文切换
* 任务状态管理
* 优先级调度
* 优先级继承
* 实时调度
* 时间片调度
* 任务通信
* 时间控制
* 系统资源管理
* 中断驱动
* 内核行为分析

Task 中的示例更侧重于 **HRTOS 内核机制、调度过程以及实时行为的分析和验证**。

### Driver

HRTOS 硬件驱动组件，为 51 单片机常用外设和模块提供驱动支持。

按照功能划分为：

* **Basic**：蜂鸣器、LED、按键、矩阵键盘、外部中断、定时器等
* **Display**：LCD1602、OLED、数码管等显示设备
* **Sensor**：ADC、DHT11、DS1302、DS18B20、HC-SR04、人体感应、红外、土壤湿度、雨滴等传感器
* **Control**：PWM、继电器等控制设备
* **Storage**：24C02 EEPROM、I²C 等存储相关组件

Driver 采用模块化组织方式，并提供相应的头文件、源码和库文件，便于在 HRTOS 工程中直接使用。

## 目录结构

```text
HRTOS-Examples
├── Basic
│   ├── 01_Hello_HRTOS
│   ├── 02_Hello_Event
│   ├── 03_Hello_Interrupt
│   ├── ...
│   └── 13_HRTOS_InterruptNesting
│
├── Driver
│   ├── APP
│   ├── Inc
│   ├── Project
│   └── Src
│
└── Task
    ├── 01-task-execution
    ├── 02-scheduling-engine
    ├── 03-communication-model
    ├── 04-time-control
    ├── 05-system-resource
    ├── 06-interrupt-driven
    └── 07-kernel-behavior
```

## 使用环境

* MCU：51 单片机
* RTOS：HRTOS
* 编译环境：Keil C51

## 关于 HRTOS

HRTOS（Hard Real-Time Operating System）是一款面向 **51 单片机**设计的实时操作系统。

HRTOS 对任务调度、中断处理、上下文切换、系统资源和实时行为进行系统化管理，为 51 单片机提供完整的实时操作系统支持。

官方网站：

https://hrtos.com
