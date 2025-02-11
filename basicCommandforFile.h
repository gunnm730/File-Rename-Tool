#pragma once
#include<iostream>
#include<filesystem>
#include<vector>
#include<stdexcept>
#include<regex>

namespace fs = std::filesystem;

/**
 * ��ȡĿ¼�������ļ���·��
 * 
 * @param dir_path Ŀ¼��·��
 * @return ����Ŀ¼�������ļ�·����vector
 * 
 * �˺���ͨ������ָ��Ŀ¼���ռ������ļ���·����������Щ·���洢��һ��vector�з���
 * ��������Ŀ¼�е���Ŀ¼���������ļ���Ŀ
 */
std::vector<fs::path> get_files(const std::string& dir_path) {
    // �洢�ļ�·��������
    std::vector<fs::path> files;

    // ����Ŀ¼�е�ÿ����Ŀ
    for (const auto& entry : fs::directory_iterator(dir_path)) {
        // �����Ŀ�Ƿ�Ϊ�����ļ�
        if (entry.is_regular_file())
            // ������ļ�������·����ӵ�������
            files.push_back(entry.path());
    }

    // ���ذ��������ļ�·��������
    return files;
}

/**
 * �����������ļ���
 * 
 * ������ʹ��C++��׼���е�filesystem API���������ļ��������ȹ���һ���µ�·����
 * Ȼ���Խ���·�����ļ�������Ϊ��·������������ɹ�������true���������ʧ�ܣ�
 * �����쳣�����������Ϣ��Ȼ�󷵻�false��
 * 
 * @param old_path ���ļ���·����
 * @param new_name �ļ��������ơ�
 * @return �ɹ�����������true�����򷵻�false��
 */
bool rename_file(const fs::path & old_path, const std::string& new_name) {
    try {
        fs::path new_path = old_path.parent_path() / new_name;

        // ������ļ����Ѵ��ڣ������� (���磺file.txt �� file_1.txt)
        int counter = 1;
        while (fs::exists(new_path)) {
            std::string stem = new_path.stem().string();
            std::string ext = new_path.extension().string();

            // ��������չ�������
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
        std::cerr << " ��������: " << e.what();
        return false;
    }
}

/**
 * �����µ����ƻ���ԭʼ���ơ�
 *
 * �ú���ͨ��Ӧ��ǰ׺����׺���µ���չ�����Լ�����������ʽ���ı��滻�������µ����ơ�
 *
 * @param original_name ԭʼ�ļ��������ơ�
 * @param prefix ��ӵ����ƿ�ʼ����ǰ׺��
 * @param suffix ��ӵ����ƽ������ĺ�׺��
 * @param new_extension �滻ԭ����չ��������չ����
 * @param regex_pattern ����ƥ�������в��ֵ�������ʽģʽ��
 * @param replacement �����滻������ʽƥ�䵽�Ĳ��ֵ����ַ�����
 * @return �������ɵ������ơ�
 */
std::string generate_new_name(
    const std::string& original_name,
    const std::string& prefix = "",
    const std::string& suffix = "",
    const std::string& new_extension = "",
    const std::string& regex_pattern = "",
    const std::string& replacement = ""
) {
    // ��ʼ��������Ϊԭʼ����
    std::string new_name = original_name;

    // ����ṩ��������ʽģʽ����Ӧ���ı��滻
    if (!regex_pattern.empty()) {
        std::regex re(regex_pattern);
        new_name = std::regex_replace(new_name, re, replacement);
    }

    // ���ǰ׺�ͺ�׺��������
    new_name = prefix + new_name + suffix;

    // ����ṩ���µ���չ�������滻ԭ�е���չ��
    if (!new_extension.empty()) {
        fs::path new_path = new_name;
        new_path.replace_extension(new_extension);
        new_name = new_path.string();
    }

    // �������ɵ�������
    return new_name;
}

