#ifndef ALIEN3D_FILE_H
#define ALIEN3D_FILE_H

#include <string>
#include <iostream>
#include <fstream>
#include "Base.hpp"

namespace Utils {
    //TODO: Maybe you can change the read file function
    // by C-ish version of reading file.
    static std::string ReadFile(const std::string &path) {
        std::string data;

        std::ifstream file;
        if (file.good()) {
            file.open(path.c_str());

            std::string line;
            if (file.is_open()) {
                while (std::getline(file, line)) {
                    data += line + "\n";
                }

                file.close();
            }

            return std::move(data);
        } else {
            ALIEN_ERROR("File does not exist!");
            return "";
        }
    }
}

#endif