#ifndef _CIRCUIT_
#define _CIRCUIT_
#include "door.hpp"
#include <iostream>
#include <iomanip>
#include <list>
#include <string>
#include <map>
#include <queue>
using namespace std;

class Circuit
{
public:
  struct CDoor
  {
    Door *door_;
    string name_;
  };
  Circuit();
  ~Circuit();
  bool AddDoor(const string &name, int num = 2);
  bool Connect(const string &a, const string &b);
  bool DisConnect(const string &a, const string &b);
  bool SetInput(const string &name);
  bool DeleteDoor(const string &name);
  void Display();
  bool Run();
  void List();
  Door *GetDoor(const string &name);
  Door *GetInput(const string &name);
  Door *GetOutput(const string &name);

private:
  bool Test(Door *a, Door *b);
  list<CDoor> door_list_;
  list<CDoor> input_list_;
  list<CDoor> output_list_;
  map<string, int> door_count_;
};

#endif