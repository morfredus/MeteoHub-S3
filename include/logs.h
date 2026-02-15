#pragma once
#include <Arduino.h>

#define LOG_BUFFER_SIZE 10

void addLog(const String& msg);
String getLog(int index);
int getLogCount();
