#ifndef _DOOR_
#define _DOOR_
#include <list>
using std::list;

// 支持与、或、非、与非、或非、异或、同或功能逻辑门
// 各类门均为单输出门，输出可连接至多个输入端
// 非门为单输入，异或门、同或门为双输入
// 其他门均支持多输入
// 输入IN、输出OUT，也是其中一种特殊的门
class Door
{
public:
  Door(int num = 2);
  bool Connect(Door &other);
  bool DisConnect(Door &other);
  void Delete();
  virtual bool GetState() = 0;
  virtual bool SetInput() = 0;
  list<Door *> GetInputList();
  list<Door *> GetConnectList();
  // 重载 &（与） |（或） ~（非） ^（异或）运算符
  bool operator&(Door &other);
  bool operator|(Door &other);
  bool operator~();
  bool operator^(Door &other);

protected:
  void change();

protected:
  list<Door *> input_list_;
  list<Door *> connect_list_;
  bool output_;
  bool changed_;
  int max_input_;
};

class IN : public Door
{
public:
  IN() : Door(1) {}
  bool SetInput();
  bool GetState();
};

class OUT : public Door
{
public:
  OUT() : Door(1) {}
  bool SetInput() {}
  bool GetState();
};

class AND : public Door
{
public:
  AND(int num = 2) : Door(num) {}
  bool SetInput() {}
  bool GetState();
};

class OR : public Door
{
public:
  OR(int num = 2) : Door(num) {}
  bool SetInput() {}
  bool GetState();
};

class NOT : public Door
{
public:
  NOT() : Door(1) {}
  bool SetInput() {}
  bool GetState();
};

class NAND : public Door
{
public:
  NAND(int num = 2) : Door(num) {}
  bool SetInput() {}
  bool GetState();
};

class NOR : public Door
{
public:
  NOR(int num = 2) : Door(num) {}
  bool SetInput() {}
  bool GetState();
};

class XOR : public Door
{
public:
  XOR(int num = 2) : Door(num) {}
  bool SetInput() {}
  bool GetState();
};

class NXOR : public Door
{
public:
  NXOR(int num = 2) : Door(num) {}
  bool SetInput() {}
  bool GetState();
};

#endif