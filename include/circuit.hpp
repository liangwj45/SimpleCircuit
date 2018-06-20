#ifndef _CIRCUIT_
#define _CIRCUIT_
#include "door.hpp"
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <string>
using namespace std;

class Circuit {
public:
  Circuit();
  ~Circuit();
  bool AddDoor(const string &name, int num = 2);
  bool DeleteDoor(const string &name);
  bool Connect(const string &a, const string &b);
  bool DisConnect(const string &a, const string &b);
  bool SetInput(const string &name);
  void Display();
  bool Run();
  void List();

private:
  bool Test(const Door &a, const Door &b) const;
  Door *GetDoor(const string &name);
  Door *GetInput(const string &name);
  Door *GetOutput(const string &name);

private:
  list<Door *> door_list_;
  list<Door *> input_list_;
  list<Door *> output_list_;
  map<string, int> door_count_;
};

#endif