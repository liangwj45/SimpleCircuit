#include "door.hpp"

Door::Door(const string &name, int num)
    : output_(false), name_(name), changed_(true), max_input_(num) {}

bool Door::Connect(Door &other) {
  if (other.input_list_.size() == other.max_input_) {
    throw("Max input"); // 输入已连满，抛出异常
  }
  other.input_list_.push_back(this);
  connect_list_.push_back(&other);
  other.change();
  return true;
}

bool Door::DisConnect(Door &other) {
  bool action = false;
  // remove connect in other's input list
  for (auto it = other.input_list_.begin(); it != other.input_list_.end();
       ++it) {
    if ((*it)->name_ == name_) {
      other.input_list_.erase(it);
      action = true;
      break;
    }
  }
  // remove connect in this connect list
  for (auto it = connect_list_.begin(); it != connect_list_.end(); ++it) {
    if ((*it)->name_ == other.name_) {
      connect_list_.erase(it);
      action = true;
      break;
    }
  }
  if (!action) {
    throw("Non-connected");
  }
  return true;
}

void Door::Delete() {
  for (Door *each : input_list_) {
    each->DisConnect(*this);
  }
  for (Door *each : connect_list_) {
    DisConnect(*each);
  }
}

void Door::change() {
  changed_ = true;
  for (Door *each : connect_list_) {
    each->change();
  }
}

const string &Door::GetName() const { return name_; }

const list<Door *> &Door::GetInputList() const { return input_list_; }

const list<Door *> &Door::GetConnectList() const { return connect_list_; }

bool IN::GetState() { return output_; }

bool IN::SetInput() {
  output_ = !output_;
  change();
  return output_;
}

bool OUT::GetState() {
  if (input_list_.empty()) {
    changed_ = false;
    throw("invalid");
  }
  if (changed_) {
    output_ = input_list_.front()->GetState();
    changed_ = false;
  }
  return output_;
}

bool AND::GetState() {
  if (input_list_.empty()) {
    throw("invalid");
    changed_ = false;
  }
  if (changed_) {
    for (Door *each : input_list_) {
      if (!each->GetState()) {
        output_ = false;
        changed_ = false;
        return output_;
      }
    }
    output_ = true;
    changed_ = false;
  }
  return output_;
}

bool OR::GetState() {
  if (input_list_.empty()) {
    changed_ = false;
    throw("invalid");
  }
  if (changed_) {
    for (Door *each : input_list_) {
      if (each->GetState()) {
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

bool NOT::GetState() {
  if (input_list_.empty()) {
    changed_ = false;
    throw("invalid");
  }
  if (changed_) {
    output_ = ~*(input_list_.front());
    changed_ = false;
  }
  return output_;
}

bool NAND::GetState() {
  if (input_list_.empty()) {
    changed_ = false;
    throw("invalid");
  }
  if (changed_) {
    for (Door *each : input_list_) {
      if (!each->GetState()) {
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

bool NOR::GetState() {
  if (input_list_.empty()) {
    changed_ = false;
    throw("invalid");
  }
  if (changed_) {
    for (Door *each : input_list_) {
      if (each->GetState()) {
        output_ = false;
        changed_ = false;
        return output_;
      }
    }
    output_ = true;
    changed_ = false;
  }
  return output_;
}

bool XOR::GetState() {
  if (input_list_.size() < 2) {
    throw("invalid");
    changed_ = false;
  }
  if (changed_) {
    output_ = *(input_list_.front()) ^ *(input_list_.back());
    changed_ = false;
  }
  return output_;
}

bool NXOR::GetState() {
  if (input_list_.size() < 2) {
    changed_ = false;
    throw("invalid");
  }
  if (changed_) {
    output_ = !(*(input_list_.front()) ^ *(input_list_.back()));
    changed_ = false;
  }
  return output_;
}

bool Door::operator&(Door &other) { return GetState() & other.GetState(); }

bool Door::operator|(Door &other) { return GetState() | other.GetState(); }

bool Door::operator~() { return !GetState(); }

bool Door::operator^(Door &other) { return GetState() ^ other.GetState(); }