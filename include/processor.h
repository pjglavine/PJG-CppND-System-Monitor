#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
  
 public:
  
  Processor();
  
  float Utilization();  // TODO: See src/processor.cpp

  // Declare any necessary private members
 //private:
  
  bool initialization_ = false; // flag for the constructor to initialize the previous relevant cpu usage variables.
  // This is done when the processor object is created, after that, the previous values are updated at the start of 
  // the Processor::Utilization() function rather than at the end.
  
  long user_ = 0;
  long nice_ = 0;
  long system_ = 0;
  long idle_ = 0;
  long iowait_ = 0;
  long irq_ = 0;
  long softirq_ = 0;
  long steal_ = 0;
  long guest_ = 0;
  long guestnice_ = 0;
  
  long Idle_ = 0;
  long NonIdle_ = 0;
  long Total_ = 0;
  
  long prevIdle_ = 0;
  long prevTotal_ = 0;
  
  float CpuPercentage_ = 0.0;
  
};

#endif