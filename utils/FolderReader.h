#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h>

class IReader {
public:
    virtual int walkReadFile(const std::string& folder_name, const std::string& filename, const std::string_view &sv) = 0;
    virtual ~IReader() {}
};

class FolderReader {
public:
    int runFolderFileData(std::string& folder, IReader& callback, bool recursive, int max_levels) {
        int result = 0;
        int cur_level = 0;
        std::vector<std::pair<std::string, bool>> dirs_to_process;
        dirs_to_process.push_back(std::make_pair(folder, recursive));
        while (!dirs_to_process.empty()) {
            auto cur_dir_pair = dirs_to_process.back();
            dirs_to_process.pop_back();
            auto cur_dir = cur_dir_pair.first;
            auto cur_recursive = cur_dir_pair.second;
            DIR* dir = opendir(cur_dir.c_str());
            if (dir != NULL) {
                dirent* entry;
                while ((entry = readdir(dir)) != NULL) {
                    if (entry->d_type == DT_DIR) {
                        if (cur_recursive && (max_levels == 0 || cur_level + 1 < max_levels)) {
                            if (std::string(entry->d_name) != "." && std::string(entry->d_name) != "..") {
                                dirs_to_process.push_back(std::make_pair(cur_dir + "/" + std::string(entry->d_name), cur_recursive));
                            }
                        }
                    }
                    else if (entry->d_type == DT_REG) {
                        std::string file_name = std::string(entry->d_name);
                        std::string file_path = cur_dir + "/" + file_name;
                        std::ifstream file(file_path, std::ios::binary);
                        if (file) {
                            file.seekg(0, std::ios::end);
                            size_t file_size = file.tellg();
                            file.seekg(0, std::ios::beg);
                            char* file_data = new char[file_size];
                            file.read(file_data, file_size);
                            file.close();
                            result = callback.walkReadFile(cur_dir, file_name, std::string_view(file_data, file_size));
                            delete[] file_data;
                            if (result != 0) {
                                closedir(dir);
                                return result;
                            }
                        }
                    }
                }
                closedir(dir);
            }
            cur_level++;
        }
        return result;
    }
};

