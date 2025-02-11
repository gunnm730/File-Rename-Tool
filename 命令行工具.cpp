#include"basicCommandforFile.h"
#include"cxxopts.hpp"


int main(int argc, char* argv[]) {

    // ========================================================================
    // 1. 定义并解析命令行参数
    // ========================================================================
    cxxopts::Options options("FileRenamer", "批量重命名工具 - 支持前缀/后缀/扩展名/正则替换");
    options.add_options()
        ("d,dir", "目标目录", cxxopts::value<std::string>()->default_value("."))
        ("p,prefix", "文件名前缀", cxxopts::value<std::string>()->default_value(""))
        ("s,suffix", "文件名后缀", cxxopts::value<std::string>()->default_value(""))
        ("e,ext", "新扩展名", cxxopts::value<std::string>()->default_value(""))
        ("r,regex", "正则表达式模式", cxxopts::value<std::string>()->default_value(""))
        ("R,replace", "正则替换内容", cxxopts::value<std::string>()->default_value(""))
        ("h,help", "显示帮助信息");

    try {
        auto result = options.parse(argc, argv);

        // 如果用户输入了 --help，显示帮助并退出
        if (result.count("help")) {
            std::cout << options.help() << "\n";
            std::cout << "示例:\n"
                << "  renamer.exe -d ./docs -p img_ -e png\n"
                << "  renamer.exe --dir=./logs --suffix=_backup --regex=\"\\d+\" --replace=NUM\n";
            return 0;
        }

        // ====================================================================
        // 2. 获取参数值
        // ====================================================================
        std::string dir_path = result["dir"].as<std::string>();
        std::string prefix = result["prefix"].as<std::string>();
        std::string suffix = result["suffix"].as<std::string>();
        std::string new_ext = result["ext"].as<std::string>();
        std::string regex_pat = result["regex"].as<std::string>();
        std::string regex_rep = result["replace"].as<std::string>();

        // ====================================================================
        // 3. 检查目录是否存在
        // ====================================================================
        if (!fs::exists(dir_path) || !fs::is_directory(dir_path)) {
            std::cerr << "错误：目录不存在或无法访问 - " << dir_path << "\n";
            return 1;
        }

        // ====================================================================
        // 4. 获取文件列表并重命名
        // ====================================================================
        auto files = get_files(dir_path);
        if (files.empty()) {
            std::cout << "目录中没有可处理的文件。\n";
            return 0;
        }

        std::cout << "开始处理目录: " << dir_path << "\n";
        std::cout << "找到 " << files.size() << " 个文件\n\n";

        for (const auto& file : files) {
            std::string original_name = file.filename().string();
            std::string new_name = generate_new_name(
                original_name,
                prefix,
                suffix,
                new_ext,
                regex_pat,
                regex_rep
            );

            // 打印变更信息
            std::cout << "重命名: " << original_name << " → " << new_name;

            if (rename_file(file, new_name)) {
                std::cout << " [成功]\n";
            }
            else {
                std::cout << " [失败]\n";
            }
        }

    }
    catch (const cxxopts::exceptions::exception& e) {
        // 捕获参数解析错误（如类型不匹配）
        std::cerr << "参数错误: " << e.what() << "\n";
        return 1;
    }
    catch (const std::exception& e) {
        // 捕获其他异常（如文件系统错误）
        std::cerr << "运行时错误: " << e.what() << "\n";
        return 1;
    }

    return 0;
}