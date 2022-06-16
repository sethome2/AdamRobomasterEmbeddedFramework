<!--
 * @Author: sethome
 * @Date: 2021-08-18 01:50:37
 * @FilePath: \RMc_Adam_GenralRobotSystem Ver1.0.4.20210818 Alpha\README.md
-->
RMc_adam_GenralRobotSystem
Ver1.0.4.20210818 Alpha

采用MIT开源协议，我没加协议头（不合法。。严格来说也不能用DJI开源的，逃

这是关于RobotMaster的开发板C的框架，遵循各单元高内聚，低耦合的特点，进行开发。并方便移植。
目前，除了串口有框架问题，其他均正常。（应该）
相当于对官方的bsp做了精简，目前正在去DJI化（逃）
希望如果这个对你有帮助，能提到这个框架~我们希望减少无用功，专注在应用程序的开发上
并且，快速糊shit！

所有逻辑层都在 freeRTOS.c 实现（比较也不多，也精简
软件层在Softeware，硬件控制在Hardware

作者：**sethome + 帮忙协助的各位**

___
问题：
1. 命名太混乱啦
2. C语言缺失命名空间的问题开始凸显，应该严格管理全局变量与宏
3. 掉线检测懒得写了

简单描述：
尽可能使用HAL库函数，支持CubeMX二次定义，陀螺仪及PID（待实现）的定时控制
___
写在最后：

1.0.4可能是最后一个迭代了，说实话这玩意算是我一个人在写（笑），如今是打算多人合作重写一份Adam的底层框架（试产）。
这是要废弃了，本来还想加上第三方开源CJSON之类的。
很多地方程序设计考虑不周，不够明了，还请多多担待，期待新的框架吧。

release里有干净的工程和齐鲁工业大学Adam战队的工程机器人比赛代码。
比赛代码里很多是临时加的，就没什么可读性，见谅。

补充：现在由于各种原因，已经跨越所有机器人运行了。
补补充：居然活下来了。
___
# 更新日志

**大版本更新**
更新日志（Ver2.0.0.20220616 Alpha）:
1. 支持了裁判系统的解析与UI图像绘制（字符待测试）
2. 支持了队内的电容
为什么是大版本？ 因为终于是完全体了，适合各种兵种的机器人

**大版本更新**
更新日志（Ver1.1.5.20220325 Alpha）: 
1. 增加了脉塔X系列电机的支持
2. 更好的陀螺仪（利用了官方开源的lib）
3. 自带全局AC机一个
4. 完善的云台和底盘支持（哨兵需要修改Keil，删除chassis_move.c & .h 加入guard_chassic.c & .h）
5. 完善的串口通信+协议，支持USB虚拟串口（CDC半双工）
6. 支持了北醒的激光雷达
7. 支持按键更改队伍
8. 支持电机等掉线检测

更新日志（Ver1.0.4.20210818 Alpha）:
1. 修复长久以来的命名问题

更新日志（Ver1.0.3.20210713 Alpha）:
1. 使用了SPI2_CS引脚为IO8，浮空输出模式，支持状态量控制，不支持PWM

更新日志（Ver1.0.2.20210712 Alpha）:
1. 更改IO口功能，IO1-5不变，IO6改为浮空输入模式，IO7为浮空外部中断
2. 增加了对光电编码器的支持(Drivers/Encoder.c)，但是无统一定时器，未能计算速度
3. 增加了对系统时间的支持(Applications/time.c)，最大49天时间
4. 移植了ADAM步兵的开源代码，为多个文件(referee.c,referee_usart_send_task.c)
5. 增加了CRC8/16(Algorithm/CRC8_CRC16.c),和fifo数据结构(Algorithm/fifo.c)的支持

更新日志（Ver1.0.1.20210330 Alpha）：
1. 修复PWM输出问题

更新日志（Ver1.0.0.20210327 Alpha）：
1. bsp基本工作正常
