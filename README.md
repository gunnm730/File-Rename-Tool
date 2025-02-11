# 命令行工具
    options.add_options()
        ("d,dir", "目标目录", cxxopts::value<std::string>()->default_value("."))
        ("p,prefix", "文件名前缀", cxxopts::value<std::string>()->default_value(""))
        ("s,suffix", "文件名后缀", cxxopts::value<std::string>()->default_value(""))
        ("e,ext", "新扩展名", cxxopts::value<std::string>()->default_value(""))
        ("r,regex", "正则表达式模式", cxxopts::value<std::string>()->default_value(""))
        ("R,replace", "正则替换内容", cxxopts::value<std::string>()->default_value(""))
        ("h,help", "显示帮助信息");
