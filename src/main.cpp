#include "ncurses_display.h"
#include "system.h"

int main() {
  //Create system object.
  System system;
  
  //Pass system object into NCurses Display function;
  NCursesDisplay::Display(system);
}