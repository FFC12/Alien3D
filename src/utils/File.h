#ifndef ALIEN3D_FILE_H
#define ALIEN3D_FILE_H

#include <string>
#include <iostream>
#include <fstream>

namespace Utils {
    //TODO: Maybe you can change the read file function
    // by C-ish version of reading file.
    static std::string ReadFile(const std::string &path) {
        std::string data;

        std::ifstream file;
        file.open(path.c_str());

        std::string line;
        if (file.is_open()) {
            while (std::getline(file, line)) {
                data += line + "\n";
            }

            file.close();
        }

        return std::move(data);
    }
}

#endif