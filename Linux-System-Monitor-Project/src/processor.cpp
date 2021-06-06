#include <string>
#include <vector>

#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() { 
  long int currIdle = LinuxParser::IdleJiffies();
  long int currActive = LinuxParser::ActiveJiffies();


  long int prevTotal = prevIdle + prevActive;
  long int currTotal = currIdle + currActive;

  long int totalDiff = currTotal - prevTotal;
  long int idleDiff = currIdle - prevIdle;

  /* update the previous times to be used in the next iteration */
  prevIdle = currIdle;
  prevActive = currActive;

  return (double)(totalDiff - idleDiff) / (double)totalDiff;
}
