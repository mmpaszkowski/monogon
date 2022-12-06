#include "../include/cpp_static_lib_template/library.h"

#include <fmt/color.h>
#include <monogon/config.h>

namespace monogon
{
    void version()
    {
        fmt::print(fg(fmt::color::white), project_name);
        fmt::print(fg(fmt::color::green), " v");
        fmt::print(fg(fmt::color::green), project_version);
        fmt::print(fg(fmt::color::green), "\n");
    }
}

