#pragma once
#include <string>


#define LOG_BUFFER_SIZE 10

void addLog(const std::string& msg);
std::string getLog(int index);
int getLogCount();
void clearLogs();

#define LOG_DEBUG(msg) addLog(std::string("[DEBUG] ") + std::string(msg))
#define LOG_INFO(msg) addLog(std::string("[INFO] ") + std::string(msg))
#define LOG_WARNING(msg) addLog(std::string("[WARN] ") + std::string(msg))
#define LOG_ERROR(msg) addLog(std::string("[ERROR] ") + std::string(msg))
