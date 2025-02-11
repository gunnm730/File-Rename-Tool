#include"basicCommandforFile.h"
#include"cxxopts.hpp"


int main(int argc, char* argv[]) {

    // ========================================================================
    // 1. ���岢���������в���
    // ========================================================================
    cxxopts::Options options("FileRenamer", "�������������� - ֧��ǰ׺/��׺/��չ��/�����滻");
    options.add_options()
        ("d,dir", "Ŀ��Ŀ¼", cxxopts::value<std::string>()->default_value("."))
        ("p,prefix", "�ļ���ǰ׺", cxxopts::value<std::string>()->default_value(""))
        ("s,suffix", "�ļ�����׺", cxxopts::value<std::string>()->default_value(""))
        ("e,ext", "����չ��", cxxopts::value<std::string>()->default_value(""))
        ("r,regex", "������ʽģʽ", cxxopts::value<std::string>()->default_value(""))
        ("R,replace", "�����滻����", cxxopts::value<std::string>()->default_value(""))
        ("h,help", "��ʾ������Ϣ");

    try {
        auto result = options.parse(argc, argv);

        // ����û������� --help����ʾ�������˳�
        if (result.count("help")) {
            std::cout << options.help() << "\n";
            std::cout << "ʾ��:\n"
                << "  renamer.exe -d ./docs -p img_ -e png\n"
                << "  renamer.exe --dir=./logs --suffix=_backup --regex=\"\\d+\" --replace=NUM\n";
            return 0;
        }

        // ====================================================================
        // 2. ��ȡ����ֵ
        // ====================================================================
        std::string dir_path = result["dir"].as<std::string>();
        std::string prefix = result["prefix"].as<std::string>();
        std::string suffix = result["suffix"].as<std::string>();
        std::string new_ext = result["ext"].as<std::string>();
        std::string regex_pat = result["regex"].as<std::string>();
        std::string regex_rep = result["replace"].as<std::string>();

        // ====================================================================
        // 3. ���Ŀ¼�Ƿ����
        // ====================================================================
        if (!fs::exists(dir_path) || !fs::is_directory(dir_path)) {
            std::cerr << "����Ŀ¼�����ڻ��޷����� - " << dir_path << "\n";
            return 1;
        }

        // ====================================================================
        // 4. ��ȡ�ļ��б�������
        // ====================================================================
        auto files = get_files(dir_path);
        if (files.empty()) {
            std::cout << "Ŀ¼��û�пɴ�����ļ���\n";
            return 0;
        }

        std::cout << "��ʼ����Ŀ¼: " << dir_path << "\n";
        std::cout << "�ҵ� " << files.size() << " ���ļ�\n\n";

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

            // ��ӡ�����Ϣ
            std::cout << "������: " << original_name << " �� " << new_name;

            if (rename_file(file, new_name)) {
                std::cout << " [�ɹ�]\n";
            }
            else {
                std::cout << " [ʧ��]\n";
            }
        }

    }
    catch (const cxxopts::exceptions::exception& e) {
        // ��������������������Ͳ�ƥ�䣩
        std::cerr << "��������: " << e.what() << "\n";
        return 1;
    }
    catch (const std::exception& e) {
        // ���������쳣�����ļ�ϵͳ����
        std::cerr << "����ʱ����: " << e.what() << "\n";
        return 1;
    }

    return 0;
}