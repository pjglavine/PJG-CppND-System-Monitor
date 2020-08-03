#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"
using std::string;
using std::to_string;
using std::vector;

// Constructor for process object, initializes object private members
Process::Process(int pid) {
  
  pid_ = pid;
  cpu_utilization_ = Process::CpuUtilization();
  ram_ = stol(Process::Ram());
  
  }

// Return this process's ID
int Process::Pid() { 
  return pid_; 
}

// Return this process's CPU utilization
float Process::CpuUtilization() const { 
  long total_time = 0;
  long seconds = 0;
  
  //total_time is comprised of jiffy information which is accessed by passing the data index into the UpTime function. See LinuxParser::UpTime(int pid, int t_index) for details
  total_time = (LinuxParser::UpTime(pid_ , 13) + LinuxParser::UpTime(pid_ , 14) + LinuxParser::UpTime(pid_ , 15)+LinuxParser::UpTime(pid_ , 16));
  seconds = LinuxParser::UpTime() - (LinuxParser::UpTime(pid_, 21)/sysconf(_SC_CLK_TCK));
  if (seconds > 0){ // avoid division by zero here, this was causing issues.
    return float((float(total_time)/sysconf(_SC_CLK_TCK))/float(seconds));
  } else {
    return 0;
  }
}

// Return the command that generated this process
string Process::Command() { 
  return LinuxParser::Command(pid_); 
}

// Return this process's memory utilization
string Process::Ram() { 
  return LinuxParser::Ram(pid_); 
}

// Return the user (name) that generated this process
string Process::User() { 
  return LinuxParser::User(pid_); 
}

// Return the age of this process (in seconds)
long int Process::UpTime() { 
  return LinuxParser::UpTime(pid_, 21)/sysconf(_SC_CLK_TCK); 
}

// Overload the "less than" comparison operator for Process objects to sort by CPU utlization.
bool Process::operator<(Process const& a) const {
  return a.CpuUtilization() < CpuUtilization();
}