#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { 
    return cpu_;
}

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  vector<int> pids = LinuxParser::Pids();
  // clear the vector to populate with a new batch of processes
  processes_.clear();

  // Add processes with pids read from linux parser
  for (int pid : pids) {
    processes_.push_back(Process(pid));
  }

  // Sort by the 10 most cpu usage processes
  std::sort(processes_.begin(), processes_.end());
  std::reverse(processes_.begin(), processes_.end());

  return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() const{ 
    return LinuxParser::Kernel(); 
}

// Return the system's memory utilization
float System::MemoryUtilization() const{ 
    return LinuxParser::MemoryUtilization(); 
}

// Return the operating system name
std::string System::OperatingSystem() const{ 
    return LinuxParser::OperatingSystem(); 
}

// Return the number of processes actively running on the system
int System::RunningProcesses() const{ 
    return LinuxParser::RunningProcesses(); 
}

// Return the total number of processes on the system
int System::TotalProcesses() const{ 
    return LinuxParser::TotalProcesses(); 
}

// Return the number of seconds since the system started running
long int System::UpTime() const{ 
    return LinuxParser::UpTime(); 
}