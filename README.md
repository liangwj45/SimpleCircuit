#[Project] Simple Circuit

> 程序设计 Ⅱ 期末项目 梁文杰 16303050 数据科学与计算机学院软件工程 17 级教务二班

###运行环境

- Windows/Linux

###编译运行

- 编译
  在项目根目录下，在 Terminal 执行
  `make`
- 运行
  `make run`

###项目说明

命令行版的门电路实验程序的简单实现

- 支持与门`AND`、或门`OR`、非门`NOT`、与非门`NAND`、或非门`NOR`、异或门`XOR`、同或门`NXOR`以及输入端`IN`和输出端`OUT`等基本器件
- 与门、或门、与非门、或非门支持**多输入**，单输出
- 各逻辑门输出由多个输入共同控制，若无输入，则输出为**无效电平**
- 与门、或门、与非门、或非门只要有输入，输出即有效，其他输入**可悬空**
- 异或门、同或门输入少于两个时，输出为**无效电平**
