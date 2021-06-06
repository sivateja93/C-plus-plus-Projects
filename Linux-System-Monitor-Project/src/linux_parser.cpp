#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

// Read operating sytem data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Read kernel data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// Find the existing process ids
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memTotal, memFree;
  string key;
  string value;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
        if (key == "MemTotal:") 
          memTotal = stof(value);
        if (key == "MemFree:")
          memFree = stof(value);
    }
    return float(1-(memFree/memTotal));
  }
  return 0.0;
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  string value;
  long upTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
    upTime = stol(value);
    return upTime;
  }
  return 0; 
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  vector<string> cpuTimes = CpuUtilization();

  return (std::stol(cpuTimes[0]) + std::stol(cpuTimes[1]) +
          std::stol(cpuTimes[2]) + std::stol(cpuTimes[3]) +
          std::stol(cpuTimes[4]) + std::stol(cpuTimes[5]) +
          std::stol(cpuTimes[6]) + std::stol(cpuTimes[7])); }

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  long int userTime, systemTime, childUserTime, childSystemTime;
  string line, skip;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream lineStream(line);

    // skip reading untill the 14th element
    for (int i = 0; i < 13; i++) {
      lineStream >> skip;
    }

    // read from 14th to the 17th element
    lineStream >> userTime >> systemTime >> childUserTime >> childSystemTime;
  }

  return userTime + systemTime + childUserTime + childSystemTime;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> cpuTimes = CpuUtilization();

  return (std::stol(cpuTimes[0]) + std::stol(cpuTimes[1]) +
          std::stol(cpuTimes[2]) + std::stol(cpuTimes[5]) +
          std::stol(cpuTimes[6]) + std::stol(cpuTimes[7])); 
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> cpuTimes = CpuUtilization();

  return (std::stol(cpuTimes[3]) + std::stol(cpuTimes[4]));
}

// Read and return total CPU utilization of system
vector<string> LinuxParser::CpuUtilization() {
  string key;
  string value;
  vector<string> values(10);
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu"){
        linestream >> values[0] >> values[1] >> values[2] >>
        values[3] >> values[4] >> values[5] >> values[6] >>
        values[7] >> values[8] >> values[9];
      }
    }
  }
  return values;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "processes")
        return stol(value);
    }
  }
  return 0; 
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "procs_running")
        return stol(value);
    }
  }
  return 0; 
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string pid_str = to_string(pid);
  string line, cmdLine;

  std::ifstream stream(LinuxParser::kProcDirectory + "/" + pid_str + 
                       LinuxParser::kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cmdLine;
    return cmdLine;
  }
  return string(); 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string pid_str = to_string(pid);
  string line, key, value;
  float ram_kb;
  std::ifstream stream(LinuxParser::kProcDirectory + "/" + pid_str + 
                       LinuxParser::kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmData:") { //Using VmData instead of VmSize
        ram_kb = stof(value);
        int ram_mb = int(ram_kb / 1024); // KB to MB
        return to_string(ram_mb);
      }
    }
  }
  return string();  
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string pid_str = to_string(pid);
  string line, key, value, uid;
  std::ifstream stream(LinuxParser::kProcDirectory + "/" + pid_str + 
                       LinuxParser::kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:"){
        uid = value;
        break;
      }
    }
    return uid;
  }
  return string(); 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line, key, user, uid, dummy;
  std::ifstream stream(LinuxParser::kPasswordPath);
  if (stream.is_open()) {
    uid = Uid(pid);
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);

      std::getline(linestream, user, ':');
      std::getline(linestream, dummy, ':');
      std::getline(linestream, key, ':');
      if (key == uid)
        return user;
    }
  }
  return string(); 
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string pid_str = to_string(pid);
  std::ifstream stream(LinuxParser::kProcDirectory + "/" + pid_str + 
                       LinuxParser::kStatFilename);
  string line, dum1, dum2, dum3, dum4, dum5, dum6, dum7, dum8, dum9;
  string dum10, dum11, dum12, dum13, dum14, dum15, dum16, dum17, dum18;
  string dum19, dum20, dum21, starttime;

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream>> dum1 >> dum2 >> dum3 >> dum4 >> dum5 >> dum6 >> dum7 
              >> dum8 >> dum9 >> dum10 >> dum11 >> dum12 >> dum13 
              >> dum14 >> dum15 >> dum16 >> dum17 >> dum18 >> dum19 
              >> dum20 >> dum21 >> starttime;
    long upTimePid = UpTime() - stol(starttime)/ sysconf(_SC_CLK_TCK);
    return upTimePid;
  }
  return 0; 
}

// Read and return total CPU utilization of a given process
float LinuxParser::CpuUtilization(int pid) {
  float cpu_usage;
  string pid_s = to_string(pid);
  std::ifstream stream(LinuxParser::kProcDirectory + "/" + pid_s + 
                       LinuxParser::kStatFilename);
  string line, dum1, dum2, dum3, dum4, dum5, dum6, dum7, dum8, dum9;
  string dum10, dum11, dum12, dum13, dum14, dum15, dum16, dum17, dum18;
  string dum19, dum20, dum21, dum22;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream>> dum1 >> dum2 >> dum3 >> dum4 >> dum5 >> dum6 >> dum7 
              >> dum8 >> dum9 >> dum10 >> dum11 >> dum12 >> dum13 
              >> dum14 >> dum15 >> dum16 >> dum17 >> dum18 >> dum19 
              >> dum20 >> dum21 >> dum22;
  // Refer to the given weblink for details of this execution:
  // shorturl.at/eoDEL
  int utime, stime, cutime, cstime, starttime; 
  utime = stol(dum14); stime = stol(dum15); cutime =stol(dum16); 
  cstime =stol(dum17); starttime = stol(dum22);
  long totaltime = utime + stime + cutime + cstime;
  long uptime = UpTime();
  long Hertz = sysconf(_SC_CLK_TCK);
  float seconds = uptime - float(starttime / Hertz);
  cpu_usage = float(totaltime / Hertz) / seconds;
  return cpu_usage;
  }
  return 0.0; 
}
