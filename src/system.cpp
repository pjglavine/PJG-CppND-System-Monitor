#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Construct a system using the currently running processes.
System::System() {
  processes_ = System::Processes();
  Processor cpu_;
}

// Return the system's CPU
Processor& System::Cpu() {
  cpu_.Utilization();
  return cpu_;
  
}

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  
  processes_.clear(); //clear the previously stored processes to make room for a new set
  std::vector<int> pids = LinuxParser::Pids();
  for (int pid : pids){
    Process next_process(pid);
    processes_.emplace_back(next_process);
  }
  
  if (!processes_.empty()){ // make sure there are processes to sort.
    std::sort(processes_.begin(), processes_.end(), [](Process& a, Process& b){ return (a < b);});
  }
  
  return processes_; 
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { 
  return LinuxParser::Kernel(); 
}

// Return the system's memory utilization
float System::MemoryUtilization() { 
  return LinuxParser::MemoryUtilization(); 
}

// Return the operating system name
std::string System::OperatingSystem() { 
  return LinuxParser::OperatingSystem(); 
}

// Return the number of processes actively running on the system
int System::RunningProcesses() {
  return LinuxParser::RunningProcesses();
}

// Return the total number of processes on the system
int System::TotalProcesses() { 
  return LinuxParser::TotalProcesses(); 
}

// Return the number of seconds since the system started running
long int System::UpTime() { 
  return LinuxParser::UpTime(); 
}
