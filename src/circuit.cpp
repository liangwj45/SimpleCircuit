#include "circuit.hpp"

Circuit::Circuit() {}

Circuit::~Circuit()
{
    // 删除所有门
    for (auto each : door_list_)
    {
        delete each;
        each = nullptr;
    }
    // 删除所有输入端
    for (auto each : input_list_)
    {
        delete each;
        each = nullptr;
    }
    // 删除所有输出端
    for (auto each : output_list_)
    {
        delete each;
        each = nullptr;
    }
}

// 添加门
bool Circuit::AddDoor(const string &name, int n)
{
    string num = to_string(++door_count_[name]); // 给新增的门添加序号
    // 添加输入端
    if (name == "in")
    {
        input_list_.push_back(new IN(name + num));
        return true;
    }
    // 添加输出端
    if (name == "out")
    {
        output_list_.push_back(new OUT(name + num));
        return true;
    }
    // 添加非门
    if (name == "not")
    {
        door_list_.push_back(new NOT(name + num));
        return true;
    }
    // 添加 n 输入或门
    if (name == "or")
    {
        door_list_.push_back(new OR(name + num, n));
        return true;
    }
    // 添加 n 输入与门
    if (name == "and")
    {
        door_list_.push_back(new AND(name + num, n));
        return true;
    }
    // 添加 n 输入与非门
    if (name == "nand")
    {
        door_list_.push_back(new NAND(name + num, n));
        return true;
    }
    // 添加 n 输入或非门
    if (name == "nor")
    {
        door_list_.push_back(new NOR(name + num, n));
        return true;
    }
    // 添加抑或门
    if (name == "xor")
    {
        door_list_.push_back(new XOR(name + num));
        return true;
    }
    // 添加同或门
    if (name == "nxor")
    {
        door_list_.push_back(new NXOR(name + num));
        return true;
    }
    // 指定的门不存在
    throw("Nonexist door");
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
            cout << (*it)->GetName() << " =? " << name << endl;
            if ((*it)->GetName() == name)
            {
                input_list_.erase(it);
                (*it)->Delete();
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
            if ((*it)->GetName() == name)
            {
                output_list_.erase(it);
                (*it)->Delete();
                return true;
            }
        }
        throw("Nonexist door");
    }
    // 删除门
    for (auto it = door_list_.begin(); it != door_list_.end(); ++it)
    {
        if ((*it)->GetName() == name)
        {
            door_list_.erase(it);
            (*it)->Delete();
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
    // 检查是否成环
    if (!Test(*door1, *door2))
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
    // 尝试删除连接（仍存在连接不匹配导致删除失败的可能）
    return door1->DisConnect(*door2);
}

// 改变输入端的电平
bool Circuit::SetInput(const string &name)
{
    Door *door = GetInput(name);
    door->SetInput();
    return true;
}

// 根据门的名称获取相应的门
Door *Circuit::GetDoor(const string &name)
{
    for (Door *each : door_list_)
    {
        if (each->GetName() == name)
        {
            return each;
        }
    }
    throw("Nonexist door");
}

// 根据输入端的名称获取相应的输入端
Door *Circuit::GetInput(const string &name)
{
    for (Door *each : input_list_)
    {
        if (each->GetName() == name)
        {
            return each;
        }
    }
    throw("Nonexist door");
}

// 根据输出端的名称获取相应的输出端
Door *Circuit::GetOutput(const string &name)
{
    for (Door *each : output_list_)
    {
        if (each->GetName() == name)
        {
            return each;
        }
    }
    throw("Nonexist door");
}

// 运行电路
bool Circuit::Run()
{
    // 打印输入端的电平
    for (Door *each : input_list_)
    {
        cout << left << setw(8) << each->GetName()
             << (each->GetState() ? "H" : "L")
             << endl;
    }
    // 打印输出端的电平
    for (Door *each : output_list_)
    {
        try
        {
            cout << left << setw(8) << each->GetName();
            cout << (each->GetState() ? "H" : "L") << endl;
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
    // 打印输入端的连接情况
    for (const Door *each : input_list_)
    {
        if (each->GetConnectList().size())
        {
            cout << left << setw(5) << each->GetName() << " -> ";
            for (const Door *door : each->GetConnectList())
            {
                cout << " " << door->GetName();
            }
            cout << endl;
        }
    }
    // 打印逻辑门的连接情况
    for (const Door *each : door_list_)
    {
        if (each->GetConnectList().size())
        {
            cout << left << setw(5) << each->GetName() << " -> ";
            for (const Door *door : each->GetConnectList())
            {
                cout << " " << door->GetName();
            }
            cout << endl;
        }
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
bool Circuit::Test(const Door &a, const Door &b) const
{
    queue<const Door *> que;
    que.push(&b);
    while (!que.empty())
    {
        for (const Door *each : que.front()->GetConnectList())
        {
            if (each->GetName() == a.GetName())
            {
                return false;
            }
            que.push(each);
        }
        que.pop();
    }
    return true;
}