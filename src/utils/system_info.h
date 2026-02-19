#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#include <Arduino.h>
#include <string>

// Forward declaration to avoid circular dependencies
class SdManager;

// Function to get a JSON string with system information
std::string getSystemInfoJson(SdManager* sd);

#endif // SYSTEM_INFO_H