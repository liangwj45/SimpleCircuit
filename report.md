# Simple Circuit Report

> 程序设计 Ⅱ 期末项目 梁文杰 16303050 数据科学与计算机学院软件工程 17 级教务二班

### 项目说明

命令行版的门电路实验程序的简单实现

- 支持与门`AND`、或门`OR`、非门`NOT`、与非门`NAND`、或非门`NOR`、异或门`XOR`、同或门`NXOR`以及输入端`IN`和输出端`OUT`等基本器件
- 与门、或门、与非门、或非门支持**多输入**，单输出
- 各逻辑门输出由多个输入共同控制，若无输入，则输出为**无效电平**
- 与门、或门、与非门、或非门只要有输入，输出即有效，其他输入**可悬空**
- 异或门、同或门输入少于两个时，输出为**无效电平**
- 支持简单的环异常检测，根据目前判断标准无法搭建 JK 触发器或 D 触发器
- 输入默认低电平，通过`set`更改（置反）

### 代码实现

#### 运算符重载

- 实现了运算符&、|、~、^的重载，对应逻辑门的与、或、非、异或运算

```c++
class Door
{
  public:
	...
    // 重载 &（与） |（或） ~（非） ^（异或）运算符
	bool operator&(Door &other);
	bool operator|(Door &other);
	bool operator~();
	bool operator^(Door &other);
  
  private:
  	...
}

bool Door::operator&(Door &other)
{
    return GetState() & other.GetState(); // GetState()为递归获取该门的输出电平
  										  // 返回值为布尔型
}

bool Door::operator|(Door &other)
{
    return GetState() | other.GetState();
}

bool Door::operator~()
{
    return !GetState();
}

bool Door::operator^(Door &other)
{
    return GetState() ^ other.GetState();
}
```

####继承和多态

- 构造一个抽象类，各类门对该抽象类公有继承
- 派生类对GetState()等函数进行重写，通过基类指针调用不同版本的函数实现多态

```c++
class Door
{
  public:
	...
	// 派生类对GetState()函数进行重写，通过基类指针调用不同版本的函数实现多态
	virtual bool GetState() = 0;
  
  private:
  	...
}

bool IN::GetState()
{
    return output_;
}

bool OUT::GetState()
{
    if (input_list_.empty())
    {
        changed_ = false;
        throw("invalid"); // 无效电平
    }
    if (changed_)
    {
        output_ = input_list_.front()->GetState();
        changed_ = false;
    }
    return output_;
}

bool OR::GetState()
{
    if (input_list_.empty())
    {
        changed_ = false;
        throw("invalid");
    }
    if (changed_)
    {
        for (Door *each : input_list_)
        {
            if (each->GetState())
            {
                output_ = true;
                changed_ = false;
                return output_;
            }
        }
        output_ = false;
        changed_ = false;
    }
    return output_;
}

...

```

####对异常的处理

- 能对环进行简单的检测，并抛出异常
- UI添加捕获输入时的异常，比如需要输出数字时输入字符，捕获异常避免程序崩溃
- 捕获输入门名称错误的异常，反馈给用户

```c++
// main.cpp
// 捕获circuit中抛出的异常或无效输入异常
catch (const char *e)
{
	cout << e << endl;
}
// 捕获输入数字时得到非数字的异常及其他异常
catch (...)
{
	cout << "Input error" << endl;
}

//circuit.cpp
// 添加连接
bool Circuit::Connect(const string &a, const string &b)
{
    // 有向连接，规定为 a 到 b 的连接
    // 确保输入端不当输出，输出端不当输入，并检查门是否存在
    string name1 = a.substr(0, 2);
    string name2 = b.substr(0, 2);
    if (name1 == "ou" || name2 == "in")
    {
        throw("illegal connect");
    }
    Door *door1 = (name1 == "in") ? GetInput(a) : GetDoor(a);
    Door *door2 = (name2 == "ou") ? GetOutput(b) : GetDoor(b);
    // 检查是否成环
    if (!Test(*door1, *door2))
    {
        throw("illegal connect");
    }
    // 尝试连接（仍存在输入端已连满导致连接失败的可能）
    return door1->Connect(*door2);
}

// door.cpp
bool Door::Connect(Door &other)
{
    if (other.input_list_.size() == other.max_input_)
    {
        throw("Max input"); // 输入已连满，抛出异常
    }
    other.input_list_.push_back(this);
    connect_list_.push_back(&other);
    other.change();
    return true;
}
```

### 拓展

- 增加了与非门、或非门和同或门
- 支持多输入门，多个输入允许部分悬空
- 添加无效电平，输出不满足条件时，输出为无效电平
- UI添加捕获输入时的异常，比如需要输出数字时输入字符，捕获异常避免程序崩溃
- 捕获输入门名称错误的异常，反馈给用户