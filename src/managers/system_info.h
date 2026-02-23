//
// Created by jf on 20/03/2023.
//

#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#include <string>

namespace SystemInfo {

    struct SystemInfo {
        std::string name;
        std::string version;
        std::string architecture;
        std::string os;
    };

    SystemInfo getSystemInfo();
}

#endif //SYSTEM_INFO_H