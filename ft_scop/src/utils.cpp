#include "utils.hpp"


// @return A sorted list of object files in the directory with the given extension.
std::vector<std::string> get_sorted_file_list(const std::string& directory, const std::string& extension)
{
    namespace fs = std::filesystem;
    std::vector<std::string> obj_list;

    try {
        if (!fs::exists(directory) || !fs::is_directory(directory)) {
            std::cerr << "Directory does not exist or is not a directory." << std::endl;
            return obj_list;
        }

        for (const auto& entry : fs::directory_iterator(directory)) {
            if (fs::is_regular_file(entry)) {
                if (entry.path().extension() == extension)
                {
                    // std::cout << entry.path().filename() << std::endl;
                    obj_list.push_back(entry.path().filename());
                }
            }
        }
        std::sort(obj_list.begin(), obj_list.end());
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return obj_list;
}

std::string getFileExtension(const std::string& fileName) {
    std::filesystem::path filePath(fileName);
    return filePath.extension().string();
}

// @return The position of the file in the sorted list, or 0 by default.
int get_file_index(const std::string& filename, const std::string& directory)
{
    std::vector<std::string> obj_list = get_sorted_file_list(directory, getFileExtension(filename));

    for (size_t i = 0; i < obj_list.size(); i++)
    {
        if(obj_list[i] == filename)
        {
            return i;
        }
    }
    return 0;
}