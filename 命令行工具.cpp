#include"basicCommandforFile.h"

int main() {
    std::string dir_path = "D:\Test"; // ����Ŀ¼
    std::string prefix = "doc_";
    std::string suffix = "";
    std::string new_extension = "bak";
    std::string regex_pattern = ""; // ƥ������
    std::string replacement = "";

    // ��ȡ�ļ��б�
    auto files = get_files(dir_path);

    // ������ÿ���ļ�
    for (const auto& file : files) {
        std::string original_name = file.filename().string();
        std::string new_name = generate_new_name(
            original_name,
            prefix,
            suffix,
            new_extension,
            regex_pattern,
            replacement
        );

        if (rename_file(file, new_name)) {
            std::cout << "Renamed: " << original_name << " �� " << new_name << "\n";
        }
    }
    return 0;

}