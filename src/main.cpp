#include "circuit.hpp"
#include <exception>
#include <iostream>
#include <string>
using namespace std;

int main() {
  bool running_ = false;
  Circuit cir;
  string oper;
  cout << "$ ";
  while (cin >> oper) {
    try {
      // 添加门
      if (oper == "add") {
        string name;
        cin >> name;
        string num;
        cin >> num;
        int n = stoi(num);
        while (n--) {
          cir.AddDoor(name);
        }
        cout << "Add door success." << endl;
        cout << endl << "$ ";
        continue;
      }
      // 添加多输入门
      if (oper == "addn") {
        string name;
        cin >> name;
        string num1, num2;
        cin >> num1 >> num2;
        int n = stoi(num1);
        int num = stoi(num2);
        while (num--) {
          cir.AddDoor(name, n);
        }
        cout << "Add n_in_door success." << endl;
        cout << endl << "$ ";
        continue;
      }
      // 删除门
      if (oper == "del") {
        string name;
        cin >> name;
        cir.DeleteDoor(name);
        cout << endl << "$ ";
        continue;
      }
      // 连接门
      if (oper == "con") {
        string name;
        cin >> name;
        string num;
        cin >> num;
        int n = stoi(num);
        while (n--) {
          string sname;
          cin >> sname;
          cir.Connect(name, sname);
        }
        cout << "Connect door success." << endl;
        cout << endl << "$ ";
        continue;
      }
      // 删除连接
      if (oper == "dc") {
        string a, b;
        cin >> a >> b;
        cir.DisConnect(a, b);
        cout << "Disconnect door success." << endl;
        cout << endl << "$ ";
        continue;
      }
      // 运行电路
      if (oper == "run") {
        running_ = true;
        cout << "Circuit run:" << endl;
        cir.Run();
        cout << endl << "$ ";
        continue;
      }
      // 停止运行电路
      if (oper == "off") {
        cout << "Circuit off." << endl;
        running_ = false;
        cout << endl << "$ ";
        continue;
      }
      // 改变输入端电平
      if (oper == "set") {
        string name;
        cin >> name;
        cir.SetInput(name);
        if (running_) {
          cout << "Circuit state:" << endl;
          cir.Run();
        }
        cout << endl << "$ ";
        continue;
      }
      // 展示电路连接
      if (oper == "ds") {
        cir.Display();
        cout << endl << "$ ";
        continue;
      }
      // 打印所有的门
      if (oper == "ls") {
        cir.List();
        cout << endl << "$ ";
        continue;
      }
      // 退出程序
      if (oper == "q") {
        break;
      }
      // 无效输入
      else {
        throw("Invalid input");
      }
    }
    // 捕获circuit中抛出的异常或无效输入异常
    catch (const char *e) {
      cout << e << endl;
    }
    // 捕获输入数字时得到非数字的异常及其他异常
    catch (...) {
      cout << "Input error" << endl;
    }
    cout << endl << "$ ";
  }
}