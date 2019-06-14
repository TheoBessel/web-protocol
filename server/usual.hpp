//
// Created by Theo Bessel on 2019-06-14.
//

#ifndef HTTPSERVER_USUAL_HPP
#define HTTPSERVER_USUAL_HPP

#include <experimental/filesystem>
#include <fstream>

namespace fs = std::experimental::filesystem;

inline std::string readFile(fs::path path) {
    std::cerr << "base path : " << path << std::endl;
    path = fs::absolute(path);
    std::cerr << "absolute path : " << path << std::endl;
    std::ifstream ifile{path};

    if (!ifile.is_open()) {
        throw std::runtime_error("Failed to open file !");
    }

    const auto fileSize = fs::file_size(path);

    std::string result(fileSize, ' ');

    ifile.read(result.data(), fileSize);
    return result;
}

#endif //HTTPSERVER_USUAL_HPP
