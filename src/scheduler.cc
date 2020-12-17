#include "scheduler.h"
#include <iostream>

Scheduler::Scheduler(
    const int _max_priority,
    const std::initializer_list<std::pair<const std::string, int>> &_resources)
    : max_priority(_max_priority),
      resource_names(get_resource_names(_resources)),
      max_resources(_resources) {
  resources = max_resources;
  ready_list.resize(max_priority + 1);
} // construct scheduler

std::vector<std::string> Scheduler::get_resource_names(
    const std::vector<std::pair<const std::string, int>> &_resources) {
  std::vector<std::string> resource_names;
  for (auto &p : _resources)
    resource_names.emplace_back(p.first);
  return resource_names;
} // convert to string vector, for the order of resources

int Scheduler::init() {
  if (process_index.count("init")) {
    std::cerr << "Error: There is already init process. Run \"de init\" to "
                 "destory it. ";
    return -1;
  }
  running = std::make_shared<Process>();
  process_index["init"] = running;
  std::cout << "Init process is running. ";
  return 0;
} // init

int Scheduler::cr(const std::string &name, const int priority) {
  if (!process_index.count("init")) {
    std::cerr << "Error: No init process. Run \"init\" first! ";
    return -1;
  }
  if (process_index.count(name)) {
    std::cerr << "Error: Here is already process " << name << ". ";
    return -1;
  }
  auto ptr = std::make_shared<Process>(name, priority);
  ptr->parent = running;
  running->children.emplace(ptr);
  process_index[name] = ptr;
  auto old_name = running->pid;
  if (priority > running->priority) { // hight priority, standby
    running->status.type = READY;
    ready_list[running->priority].emplace_back(running);
    running = ptr;
  } else {
    ready_list[ptr->priority].emplace_back(ptr);
    ptr->status.type = READY; // default is RUNNING
  }
  std::cout << "Process " << running->pid << " is running. ";
  if (old_name != running->pid)
    std::cout << "Process " << old_name << " is ready. ";
  return 0;
} // create process

int Scheduler::de(const std::string &name) {
  if (!process_index.count(name)) {
    std::cerr << "Error: There is no process " << name << ". ";
    return -1;
  }
  auto ptr = process_index[name];
  if (ptr->parent) // only the root node of deleted subtree needs this
    ptr->parent->children.erase(ptr);
  tree_del(ptr); // delete tree
  if (!running)
    Schedule(); // find new running
  return 0;
}

int Scheduler::tree_del(std::shared_ptr<Process> ptr) {
  for (auto p : ptr->children)
    tree_del(p);
  std::cout << "Process " << ptr->pid << " is killed. ";
  process_index.erase(ptr->pid);
  for (auto &p : ptr->resources)
    Release(p.first, p.second);
  switch (ptr->status.type) {
  case RUNNING:
    running = nullptr;
    break;
  case READY: // ready_list uses priority (int) as key
    ready_list[std::get<int>(ptr->status.list)].remove(ptr);
    break;
  case BLOCKED: // name (string)
    block_list[std::get<std::string>(ptr->status.list)].remove(ptr);
    break;
  }
  return 0;
}

int Scheduler::Release(const std::string &res,
                       int cnt) { // modify ptr -> resources first
  std::cout << "Release " << cnt << ' ' << res << ". ";
  resources[res] += cnt;
  while (!block_list[res].empty() && block_list[res].front()->wait_resource <=
                                         resources[res]) { // weak up processes
    auto p = block_list[res].front();
    p->resources[res] += p->wait_resource;
    resources[res] -= p->wait_resource;
    p->wait_resource = 0;
    p->status = {READY, p->priority};
    ready_list[p->priority].emplace_back(p);
    block_list[res].pop_front();
    std::cout << "Weak up process " << p->pid << ". ";
  }
  return 0;
}

int Scheduler::Schedule() { // find new running process
  for (int i = max_priority; i >= 0; --i)
    if (!ready_list[i].empty()) {
      running = ready_list[i].front();
      ready_list[i].pop_front();
      running->status.type = RUNNING;
      std::cout << "Process " << running->pid << " is running. ";
      return 0;
    }
  std::cout << "Error: Schedule failed. ";
  return -1;
}

int Scheduler::req(const std::string &name, const int cnt) {
  if (running->pid == "init") {
    std::cerr << "Error: Do not request resources for init process. ";
    return -1;
  }
  if (!max_resources.count(name) || cnt > max_resources.at(name)) {
    std::cerr << "Error: There is no " << cnt << ' ' << name << " at all. ";
    return -1;
  }
  auto pid = running->pid;
  if (cnt > resources[name]) { // block
    running->wait_resource += cnt;
    running->status = {BLOCKED, name};
    block_list[name].emplace_back(running);
    running = nullptr;
    Schedule();
    std::cout << "Process " << pid << " is blocked. ";
  } else { // success
    running->resources[name] += cnt;
    resources[name] -= cnt;
    std::cout << "Process " << pid << " requests " << cnt << ' ' << name
              << ". ";
  }
  return 0;
}

int Scheduler::rel(const std::string &name, const int cnt) {
  if (running->resources[name] < cnt) {
    std::cerr << "Error: process " << name << " hasn't request " << cnt << ' '
              << name << ". ";
    return -1;
  }
  return Release(name, cnt);
}

int Scheduler::list_ready() {
  for (int i = max_priority; i >= 0; --i) {
    std::cout << i << ":";
    if (running->priority == i)
      std::cout << running->pid;
    if (!ready_list[i].empty()) {
      auto p = ready_list[i].begin();
      if (running->priority != i) // first item, no '-'
        std::cout << (*p++)->pid;
      while (p != ready_list[i].end())
        std::cout << '-' << (*p++)->pid;
    }
    if (i)
      std::cout << '\n'; // last '\n' is output in test shell
  }
  return 0;
}

int Scheduler::list_res() {
  int size = resource_names.size();
  for (auto &p : resource_names) {
    std::cout << p << ' ' << resources[p];
    if (p != resource_names.back())
      std::cout << '\n';
  }
  return 0;
}

int Scheduler::list_block() {
  int size = resource_names.size();
  for (auto &p : resource_names) {
    std::cout << p << ' ';
    if (!block_list[p].empty()) {
      auto q = block_list[p].begin();
      std::cout << (*q)->pid;
      while (++q != block_list[p].end())
        std::cout << '-' << (*q)->pid;
    }
    if (p != resource_names.back())
      std::cout << '\n';
  }
  return 0;
}

std::string get_type(int type) { // get status string
  return type == RUNNING   ? "running"
         : type == BLOCKED ? "blocked"
         : type == READY   ? "ready"
                           : "unknown";
}

int Scheduler::pr(std::string name) {
  if (!process_index.count(name)) {
    std::cerr << "Error: There is no process " << name << ". ";
    return -1;
  }
  auto ptr = process_index[name];
  std::cout << "Pid: " << ptr->pid << ' ' << "Priority: " << ptr->priority
            << '\n'
            << "Status: "
            << "type: " << get_type(ptr->status.type) << " list: ";
  std::visit([](const auto &p) { std::cout << p << '\n'; }, ptr->status.list);
  std::cout << "Parent: " << (ptr->parent ? ptr->parent->pid : "null") << '\n'
            << "Children:";
  for (auto p : ptr->children)
    std::cout << ' ' << p->pid;
  return 0;
}

int Scheduler::to() {
  auto p = running;
  running = nullptr;
  p->status.type = READY;
  if (p->priority > 1)
    p->status.list = --p->priority; // decrease priority, keep not less then 1
  ready_list[p->priority].emplace_back(p);
  Schedule();
  if (p != running) // switched to another process
    std::cout << "Process " << p->pid << " is ready. ";
  return 0;
}
