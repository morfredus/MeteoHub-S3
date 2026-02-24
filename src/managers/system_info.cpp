//
// Created by jf on 20/03/2023.
//

#include "system_info.h"

#include <array>

namespace system_info {

std::string get_system_info() {
    std::array<char const*, 3> const names{{
        "system_info",
        "system_info",
        "system_info"
    }};
    std::array<char const*, 3> const values{{
        "system_info",
        "system_info",
        "system_info"
    }};
    return std::string(names[0]) + std::string(values[0]);
}

}