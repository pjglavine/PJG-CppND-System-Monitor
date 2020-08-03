#include <vector>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <dirent.h>
#include "processor.h"
#include "linux_parser.h"

using std::stol;
using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Constructor for Processor class, initilizes some private memeber variables
Processor::Processor(){
  
  vector<string> CpuInfo = LinuxParser::CpuUtilization();
  // CpuInfo Structure: {cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guestnice};
  initialization_ = true;
  CpuPercentage_ = Processor::Utilization();
  
} 

// Return the aggregate CPU utilization
float Processor::Utilization() { 

  vector<string> CpuInfo = LinuxParser::CpuUtilization();
  
  user_ = stol(CpuInfo[1]);
  nice_ = stol(CpuInfo[2]);
  system_ = stol(CpuInfo[3]);
  idle_ = stol(CpuInfo[4]);
  iowait_ = stol(CpuInfo[5]);
  irq_ = stol(CpuInfo[6]);
  softirq_ = stol(CpuInfo[7]);
  steal_ = stol(CpuInfo[8]);
  guest_ = stol(CpuInfo[9]);
  guestnice_ = stol(CpuInfo[10]);
  
  if (initialization_ == false) {
    
    Idle_ = idle_ + iowait_; // Total time idled
    NonIdle_ = user_ + nice_ + system_ + irq_ + softirq_ + steal_; // Total time not idle
    Total_ = Idle_ + NonIdle_; // Total time

    if ((Total_ - prevTotal_) != 0){ // avoid division by zero
      CpuPercentage_ = ((float(Total_) - float(prevTotal_)) - (float(Idle_) - float(prevIdle_)))/ (float(Total_) - float(prevTotal_));
    } else {
      CpuPercentage_ = 0.0;
    }

    return CpuPercentage_;
    
  } else { 
    // if we are initilizing the processor, we set the prevIdle_ and prevTotal_ to be the initial values calculated by the constructor. Currently the program calculates the cpu usage with respect to the usage at processor start time. Left TODO update the prevIdle_ and prevTotal_ values over small time interval to calculate a more accurate value for current cpu usage.

    Idle_ = idle_ + iowait_;
    NonIdle_ = user_ + nice_ + system_ + irq_ + softirq_ + steal_;
    Total_ = Idle_ + NonIdle_;
  
    prevIdle_ = Idle_;
    prevTotal_ = Total_;
    
    if ((Total_ - prevTotal_) != 0){
      CpuPercentage_ = ((float(Total_) - float(prevTotal_)) - (float(Idle_) - float(prevIdle_)))/ (float(Total_) - float(prevTotal_));
    } else {
      CpuPercentage_ = 0.0;
    }
    initialization_ = false;
    return CpuPercentage_;
  }
}

