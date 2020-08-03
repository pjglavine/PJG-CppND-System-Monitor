# PJG-CppND-System-Monitor

System Monitor Project in the Object Oriented Programming Course of the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213).

This project involved developing a System Monitor (Task Manager-like) program to display operating information of a Linux system. The main objective of this project was to use object-oriented programming techniques to read, parse and process data from system files, perform system utilization calculations, and display measurements to the user.

![System Monitor](images/monitor.png)

## ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

To install ncurses within your own Linux environment: `sudo apt install libncurses5-dev libncursesw5-dev`

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Basic Build Instructions
1. Clone this repository.
2. Build the project `make build`
3. Run the resulting executable: `./build/monitor`
![Starting System Monitor](images/starting_monitor.png)
