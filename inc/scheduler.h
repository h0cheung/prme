#ifndef PRME_SCHEDULER_H_
#define PRME_SCHEDULER_H_
#include "process.h"
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <initializer_list>

class Scheduler { // Resources and processes
  std::shared_ptr<Process> running;
  std::unordered_map<std::string, std::shared_ptr<Process>> process_index;
  std::vector<std::list<std::shared_ptr<Process>>> ready_list;
  std::unordered_map<std::string, std::list<std::shared_ptr<Process>>>
      block_list;
  std::unordered_map<std::string, int> resources;
  const std::vector<std::string> resource_names;
  const std::unordered_map<std::string, int> max_resources;

  std::vector<std::string>
  get_resource_names(const std::vector<std::pair<const std::string, int>> &_resources);
  int tree_del(std::shared_ptr<Process> ptr);
  int Schedule();
  int Unblock(const std::string& res);
  int Release(const std::string& res, int cnt);

public:
  const int max_priority;
  Scheduler(const int _max_priority,
            const std::initializer_list<std::pair<const std::string, int>> &_resources);
  int init(); // init
  int cr(const std::string &name, const int priority);
  int de(const std::string &name);
  int req(const std::string &name, const int cnt);
  int rel(const std::string &name, const int cnt);
  int to();
  int list_ready();
  int list_block();
  int list_res();
  int pr(std::string name);
};

#endif // PRME_SCHEDULER_H_
