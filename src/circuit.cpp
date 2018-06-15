#include "circuit.hpp"

Circuit::Circuit() {}

Circuit::~Circuit()
{
    // 删除所有门
    for (auto each : door_list_)
    {
        delete each.door_;
        each.door_ = nullptr;
    }
    // 删除所有输入端
    for (auto each : input_list_)
    {
        delete each.door_;
        each.door_ = nullptr;
    }
    // 删除所有输出端
    for (auto each : output_list_)
    {
        delete each.door_;
        each.door_ = nullptr;
    }
}

// 添加门
bool Circuit::AddDoor(const string &name, int n)
{
    Door *door = nullptr;
    string num = to_string(++door_count_[name]); // 给新增的门添加序号
    // 添加输入端
    if (name == "in")
    {
        input_list_.push_back(CDoor{new IN, name + num});
        return true;
    }
    // 添加输出端
    else if (name == "out")
    {
        output_list_.push_back(CDoor{new OUT, name + num});
        return true;
    }
    // 添加非门
    else if (name == "not")
    {
        door = new NOT;
    }
    // 添加 n 输入或门
    else if (name == "or")
    {
        door = new OR(n);
    }
    // 添加 n 输入与门
    else if (name == "and")
    {
        door = new AND(n);
    }
    // 添加 n 输入与非门
    else if (name == "nand")
    {
        door = new NAND(n);
    }
    // 添加 n 输入或非门
    else if (name == "nor")
    {
        door = new NOR(n);
    }
    // 添加抑或门
    else if (name == "xor")
    {
        door = new XOR();
    }
    // 添加同或门
    else if (name == "nxor")
    {
        door = new NXOR();
    }
    // 指定的门不存在
    else
    {
        throw("Nonexist door");
    }
    door_list_.push_back(CDoor{door, name + num});
    return true;
}

// 删除门
bool Circuit::DeleteDoor(const string &name)
{
    string sname = name.substr(0, 2);
    // 删除输入端
    if (sname == "in")
    {
        for (auto it = input_list_.begin(); it != input_list_.end(); ++it)
        {
            if (it->name_ == name)
            {
                input_list_.erase(it);
                it->door_->Delete();
                return true;
            }
        }
        throw("Nonexist door");
    }
    // 删除输出端
    if (sname == "ou")
    {
        for (auto it = output_list_.begin(); it != output_list_.end(); ++it)
        {
            if (it->name_ == name)
            {
                output_list_.erase(it);
                it->door_->Delete();
                return true;
            }
        }
        throw("Nonexist door");
    }
    // 删除门
    for (auto it = door_list_.begin(); it != door_list_.end(); ++it)
    {
        if (it->name_ == name)
        {
            door_list_.erase(it);
            it->door_->Delete();
            return true;
        }
    }
    throw("Nonexist door");
    return false;
}

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
    if (!door1 || !door2)
    {
        throw("Nonexist door");
    }
    // 检查是否成环
    if (!Test(door1, door2))
    {
        throw("illegal connect");
    }
    // 尝试连接（仍存在输入端已连满导致连接失败的可能）
    return door1->Connect(*door2);
}

// 删除连接
bool Circuit::DisConnect(const string &a, const string &b)
{
    // 有向连接，规定为删除 a 到 b 的连接
    // 不存在输出端当输入，输入端当输出，并检查门是否存在
    string name1 = a.substr(0, 2);
    string name2 = b.substr(0, 2);
    if (name1 == "ou" || name2 == "in")
    {
        throw("No match");
    }
    Door *door1 = (name1 == "in") ? GetInput(a) : GetDoor(a);
    Door *door2 = (name2 == "ou") ? GetOutput(b) : GetDoor(b);
    if (!door1 || !door2)
    {
        throw("Nonexist door");
    }
    // 尝试删除连接（仍存在连接不匹配导致删除失败的可能）
    return door1->DisConnect(*door2);
}

// 改变输入端的电平
bool Circuit::SetInput(const string &name)
{
    Door *door = GetInput(name);
    if (!door)
    {
        throw("Nonexist door");
    }
    // 将输入端电平置反
    door->SetInput();
    return true;
}

// 根据门的名称获取相应的门
Door *Circuit::GetDoor(const string &name)
{
    for (auto each : door_list_)
    {
        if (each.name_ == name)
        {
            return each.door_;
        }
    }
    return nullptr;
}

// 根据输入端的名称获取相应的输入端
Door *Circuit::GetInput(const string &name)
{
    for (auto each : input_list_)
    {
        if (each.name_ == name)
        {
            return each.door_;
        }
    }
    return nullptr;
}

// 根据输出端的名称获取相应的输出端
Door *Circuit::GetOutput(const string &name)
{
    for (auto each : output_list_)
    {
        if (each.name_ == name)
        {
            return each.door_;
        }
    }
    return nullptr;
}

// 运行电路
bool Circuit::Run()
{
    // 打印输入端的电平
    for (auto each : input_list_)
    {
        cout << left << setw(8) << each.name_
             << (each.door_->GetState() ? "H" : "L")
             << endl;
    }
    // 打印输出端的电平
    for (auto each : output_list_)
    {
        try
        {
            cout << left << setw(8) << each.name_;
            cout << (each.door_->GetState() ? "H" : "L") << endl;
        }
        catch (const char *e)
        {
            cout << e << endl;
        }
    }
}

// 打印连接的情况
void Circuit::Display()
{
    for (auto each : input_list_)
    {
        cout << left << setw(5) << each.name_ << "-> " << endl;
    }
}

// 打印所有的门
void Circuit::List()
{
    for (auto each : door_count_)
    {
        for (int i = 1; i <= each.second; ++i)
        {
            string name(each.first);
            //name += "_" + to_string(i);
            name += to_string(i);
            cout << left << setw(8) << name;
        }
        cout << endl;
    }
}

// 检查连接是否合理
bool Circuit::Test(Door *a, Door *b)
{
    queue<Door *> que;
    que.push(b);
    while (!que.empty())
    {
        for (auto each : que.front()->GetConnectList())
        {
            if (each == a)
            {
                return false;
            }
            que.push(each);
        }
        que.pop();
    }
    return true;
}