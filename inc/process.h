#ifndef PRME_PROCESS_H_
#define PRME_PROCESS_H_
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>

const int RUNNING = 0x100;
const int READY = 0x101;
const int BLOCKED = 0x102;

struct Process {
  std::string pid;                                // Process name
  std::unordered_map<std::string, int> resources; // Requested resources
  struct Status {
    int type;
    std::variant<int, std::string> list;
  } status;
  std::shared_ptr<Process> parent;
  std::unordered_set<std::shared_ptr<Process>> children;
  int priority;
  Process(std::string pid = "init", int priority = 0)
      : pid(pid), priority(priority) {
    status = {RUNNING, std::to_string(priority)}; // "running" by default
  }
};

#endif // PRME_PROCESS_H_
