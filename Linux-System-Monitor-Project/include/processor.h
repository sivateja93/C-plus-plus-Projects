#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  

  // Declare necessary private members
 private:
  long int prevIdle, prevActive;

};

#endif