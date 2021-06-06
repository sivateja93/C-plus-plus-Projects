# Linux-System-Monitor

Starter code for System Monitor Project is adapted from the Object Oriented Programming Course of the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). 

The starter code can create a system monitor like shown in the following image. 
![Starting System Monitor](images/starting_monitor.png)

The goal of the project is to create a linux system monitor that looks like the following image. 
![System Monitor](images/monitor.png)

The final system monitor should be a displaying the key information os the system like OS version, CPU uage, RAM usage, Processes running and their respective usages.

You can develop this project on your local machine.

## ncurses
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in the terminal. This project relies on ncurses for display output.

Install ncurses within your own Linux environment: `sudo apt install libncurses5-dev libncursesw5-dev`

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions

1. Clone the project repository: `git clone https://github.com/sivateja93/C-plus-plus-Projects/edit/main/Linux-System-Monitor-Project.git`

2. Build the project: `make build`

3. Run the resulting executable: `./build/monitor`
![Starting System Monitor](images/starting_monitor.png)

4. Implement the `System`, `Process`, and `Processor` classes, as well as functions within the `LinuxParser` namespace.

## Key learning: Object Oriented Programming

Created classes for process, system and processor. Created objects for each of them. This made the code reusable and scalable for future works. 
