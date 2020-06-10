#include <vector>

#include "processor.h"
#include "linux_parser.h"

// Returns the aggregate CPU utilization
float Processor::Utilization() {
    long idle = LinuxParser::IdleJiffies();
    long active = LinuxParser::ActiveJiffies();

    long prevTotal = prevIdle_ + prevActive_;
    long total = LinuxParser::Jiffies();

    long totalDelta = total - prevTotal;
    long idleDelta = idle - prevIdle_;

    prevIdle_ = idle;
    prevActive_ = active;

    return ((float)(totalDelta - idleDelta) / (float)totalDelta);
}