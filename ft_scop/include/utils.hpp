#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <vector>
#include <filesystem>
#include <algorithm>

// int load_image(const char *path, int srcDataFormat, int option1);
std::vector<std::string> get_sorted_file_list(const std::string& directory, const std::string& extension);
int get_file_index(const std::string& filename, const std::string& directory);

#endif