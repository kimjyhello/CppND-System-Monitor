#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float prevIdle_{0}, prevActive_{0};
  
  /*prevuser_{0}, prevnice_{0}, prevsys_{0}, previdle_{0}, 
  previowait_{0}, previrq_{0}, prevsoftirq_{0}, prevsteal_{0},
  prevguest_{0}, prevguestnice_{0};*/
};

#endif