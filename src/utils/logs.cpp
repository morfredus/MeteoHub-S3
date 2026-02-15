#include "logs.h"

static std::string logBuffer[LOG_BUFFER_SIZE];
static int logIndex = 0;
static int logCount = 0;

void addLog(const std::string& msg) {
    logBuffer[logIndex] = msg;
    logIndex = (logIndex + 1) % LOG_BUFFER_SIZE;
    if (logCount < LOG_BUFFER_SIZE) logCount++;
}

std::string getLog(int index) {
    if (index < 0 || index >= logCount) return "";
    int realIndex = (logIndex - logCount + index + LOG_BUFFER_SIZE) % LOG_BUFFER_SIZE;
    return logBuffer[realIndex];
}

int getLogCount() {
    return logCount;
}
