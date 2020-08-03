#include <string>
#include <math.h>
#include "format.h"
#include <sstream>
#include <iomanip>
using std::string;
using std::to_string;

// This function processes the system up time in seconds and converts it to an appropriate format for displaying.
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
  
  float hour, minute, second;
  hour = floor(seconds/3600);
  second = seconds - hour*3600;
  minute = floor(second/60);
  second = second - minute*60;
  char buffer[9]; //Create character string for storing the formatted result.
  sprintf( buffer, "%02d:%02d:%02d", (int(hour)), (int(minute)), (int(second)));

  return buffer; 
}