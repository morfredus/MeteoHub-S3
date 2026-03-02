#include "logs.h"
#include <array>
#include <string>

namespace {
// Correction 1: buffer circulaire statique pour éviter les allocations/erasures
// répétées qui fragmentent le tas et peuvent provoquer des freezes intermittents.
std::array<std::string, LOG_BUFFER_SIZE> logs;
int log_count = 0;
int log_head = 0;

int physicalIndexFromLogical(int index) {
    if (log_count == 0) {
        return 0;
    }
    return (log_head + index) % LOG_BUFFER_SIZE;
}
}

void addLog(const std::string& msg) {
    if (log_count < LOG_BUFFER_SIZE) {
        logs[physicalIndexFromLogical(log_count)] = msg;
        log_count++;
        return;
    }

    logs[log_head] = msg;
    log_head = (log_head + 1) % LOG_BUFFER_SIZE;
}

std::string getLog(int index) {
    if (index >= 0 && index < log_count) {
        return logs[physicalIndexFromLogical(index)];
    }
    return "";
}

int getLogCount() {
    return log_count;
}

void clearLogs() {
    for (int i = 0; i < log_count; i++) {
        logs[physicalIndexFromLogical(i)].clear();
    }
    log_count = 0;
    log_head = 0;
}
