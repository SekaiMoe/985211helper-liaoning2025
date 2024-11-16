#include <version.h>
#include <base.h>

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

namespace version {
    void version() {
        std::cout <<version::prog_name<<" version "<<version::prog_version<<std::endl;
        std::cout << "Build date: " << BUILD_DATE <<" "<< BUILD_TIME <<std::endl;
        std::cout <<"Compiler version: "<<__VERSION__<< std::endl;
        std::cout << "Copyright (C) 2024 dabao1955\n"
        << "License: Apache-2.0\n";
        exit(0);
    }
}

