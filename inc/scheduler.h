#ifndef PRME_SCHEDULER_H_
#define PRME_SCHEDULER_H_
#include "process.h"
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Scheduler { // Resources and processes
  std::shared_ptr<Process> running;
  std::vector<std::list<std::shared_ptr<Process>>> ready_list;
  std::unordered_map<std::string, std::list<std::shared_ptr<Process>>> block_list;
  std::unordered_map<std::string, int> resources;

public:
  const int max_priority;
  const std::vector<std::string> resource_names;
  Scheduler (int _max_priority, std::vector<std::pair<std::string, int>>);
  int init(); // init
  int cr(std::string name, int priority);
  int de(std::string name);
  int req(std::string name, int cnt);
  int rel(std::string name, int cnt);
  int to();
  int list_ready();
  int list_block();
  int list_res();
  int pr(std::string name);
};

#endif // PRME_SCHEDULER_H_
