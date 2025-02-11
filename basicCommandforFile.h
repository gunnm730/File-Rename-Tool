#pragma once
#include<iostream>
#include<filesystem>
#include<vector>
#include<stdexcept>
#include<regex>

namespace fs = std::filesystem;

/**
 * 获取目录下所有文件的路径
 * 
 * @param dir_path 目录的路径
 * @return 包含目录下所有文件路径的vector
 * 
 * 此函数通过遍历指定目录，收集所有文件的路径，并将这些路径存储在一个vector中返回
 * 它忽略了目录中的子目录和其他非文件条目
 */
std::vector<fs::path> get_files(const std::string& dir_path) {
    // 存储文件路径的容器
    std::vector<fs::path> files;

    // 遍历目录中的每个条目
    for (const auto& entry : fs::directory_iterator(dir_path)) {
        // 检查条目是否为常规文件
        if (entry.is_regular_file())
            // 如果是文件，则将其路径添加到容器中
            files.push_back(entry.path());
    }

    // 返回包含所有文件路径的容器
    return files;
}

/**
 * 尝试重命名文件。
 * 
 * 本函数使用C++标准库中的filesystem API来重命名文件。它首先构造一个新的路径，
 * 然后尝试将旧路径的文件重命名为新路径。如果操作成功，返回true；如果操作失败，
 * 捕获异常并输出错误信息，然后返回false。
 * 
 * @param old_path 旧文件的路径。
 * @param new_name 文件的新名称。
 * @return 成功重命名返回true，否则返回false。
 */
bool rename_file(const fs::path & old_path, const std::string& new_name) {
    try {
        fs::path new_path = old_path.parent_path() / new_name;

        // 如果新文件名已存在，添加序号 (例如：file.txt → file_1.txt)
        int counter = 1;
        while (fs::exists(new_path)) {
            std::string stem = new_path.stem().string();
            std::string ext = new_path.extension().string();

            // 处理无扩展名的情况
            if (ext.empty()) {
                new_path = new_path.parent_path() / (stem + "_" + std::to_string(counter++));
            }
            else {
                new_path = new_path.parent_path() / (stem + "_" + std::to_string(counter++) + ext);
            }
        }

        fs::rename(old_path, new_path);
        return true;
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << " 错误详情: " << e.what();
        return false;
    }
}

/**
 * 生成新的名称基于原始名称。
 *
 * 该函数通过应用前缀、后缀、新的扩展名，以及基于正则表达式的文本替换来生成新的名称。
 *
 * @param original_name 原始文件名或名称。
 * @param prefix 添加到名称开始处的前缀。
 * @param suffix 添加到名称结束处的后缀。
 * @param new_extension 替换原有扩展名的新扩展名。
 * @param regex_pattern 用于匹配名称中部分的正则表达式模式。
 * @param replacement 用于替换正则表达式匹配到的部分的新字符串。
 * @return 返回生成的新名称。
 */
std::string generate_new_name(
    const std::string& original_name,
    const std::string& prefix = "",
    const std::string& suffix = "",
    const std::string& new_extension = "",
    const std::string& regex_pattern = "",
    const std::string& replacement = ""
) {
    // 初始化新名称为原始名称
    std::string new_name = original_name;

    // 如果提供了正则表达式模式，则应用文本替换
    if (!regex_pattern.empty()) {
        std::regex re(regex_pattern);
        new_name = std::regex_replace(new_name, re, replacement);
    }

    // 添加前缀和后缀到名称中
    new_name = prefix + new_name + suffix;

    // 如果提供了新的扩展名，则替换原有的扩展名
    if (!new_extension.empty()) {
        fs::path new_path = new_name;
        new_path.replace_extension(new_extension);
        new_name = new_path.string();
    }

    // 返回生成的新名称
    return new_name;
}

