#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace fs = std::filesystem;

// Unescape helper to convert literal escape sequences (\n, \t, etc.) into real character byte sequences
std::string unescape_string(const std::string& input) {
    std::string result;
    result.reserve(input.length());

    for (size_t i = 0; i < input.length(); ++i) {
        if (input[i] == '\\' && i + 1 < input.length()) {
            switch (input[i + 1]) {
                case 'n':  result += '\n'; ++i; break;
                case 't':  result += '\t'; ++i; break;
                case 'r':  result += '\r'; ++i; break;
                case '"':  result += '"';  ++i; break;
                case 'q':  result += '"';  ++i; break; // \q -> " to bypass CMD quote parsing issues
                case '\\': result += '\\'; ++i; break;
                default:   result += input[i]; break;
            }
        } else {
            result += input[i];
        }
    }
    return result;
}

// Function to safely delete target file if it exists
bool remove_existing_file(const fs::path& target_file) {
    std::error_code ec;
    if (fs::exists(target_file, ec)) {
        if (fs::remove(target_file, ec)) {
            std::cout << "[INFO] Removed existing file: " << target_file.string() << std::endl;
            return true;
        } else {
            std::cerr << "[WARNING] Failed to remove existing file: " << ec.message() << std::endl;
            return false;
        }
    }
    return false;
}

void print_usage() {
    std::cout << "file-writer CLI Utility\n"
              << "Usage:\n"
              << "  file-writer.exe -n \"filename.ext\" -p \"C:\\target\\path\" -c \"code content\"\n"
              << "  file-writer.exe -d \"filename.ext\" -p \"C:\\target\\path\" -c \"code content\"\n"
              << "  file-writer.exe -n \"filename.ext\" -p \"C:\\target\\path\" < input.txt\n"
              << "  echo \"code content\" | file-writer.exe -n \"filename.ext\" -p \"C:\\target\\path\"\n\n"
              << "Parameters:\n"
              << "  -n <filename> : Specify output filename.\n"
              << "  -d <filename> : Specify output filename AND force-delete any existing file first.\n"
              << "  -p <path>     : Destination directory path.\n"
              << "  -c <content>  : Raw code content (supports escape sequences like \\n). Use '-' to read from stdin.\n";
}

int main(int argc, char* argv[]) {
    std::string filename;
    std::string folder_path;
    std::string code_content;
    bool force_delete = false;
    bool has_c_flag = false;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-v" || arg == "--version") {
            std::cout << "file-writer version 1.1.0" << std::endl;
            return 0;
        } else if (arg == "-h" || arg == "--help") {
            print_usage();
            return 0;
        } else if ((arg == "-n" || arg == "-d") && i + 1 < argc) {
            filename = argv[++i];
            if (arg == "-d") {
                force_delete = true;
            }
        } else if (arg == "-p" && i + 1 < argc) {
            folder_path = argv[++i];
        } else if (arg == "-c" && i + 1 < argc) {
            code_content = argv[++i];
            has_c_flag = true;
        }
    }

    // Input validation for target destination
    if (filename.empty() || folder_path.empty()) {
        std::cerr << "[ERROR] Missing required arguments.\n\n";
        print_usage();
        return 1;
    }

    // Determine if we should read from standard input
    bool read_from_stdin = false;
    if (has_c_flag && code_content == "-") {
        read_from_stdin = true;
    } else if (!has_c_flag) {
#ifdef _WIN32
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
        DWORD dwType = GetFileType(hStdin);
        if (dwType != FILE_TYPE_CHAR && dwType != FILE_TYPE_UNKNOWN) {
            read_from_stdin = true;
        }
#else
        if (!isatty(0)) {
            read_from_stdin = true;
        }
#endif
    }

    if (read_from_stdin) {
        // Read raw data from stdin
        code_content = std::string((std::istreambuf_iterator<char>(std::cin)),
                                    std::istreambuf_iterator<char>());
    } else if (!has_c_flag) {
        std::cerr << "[ERROR] Missing code content. Provide -c <content> or pipe/redirect input.\n\n";
        print_usage();
        return 1;
    }

    try {
        fs::path dir(folder_path);
        
        // 1. Create target directories if missing (skip root-only paths like C: or C:\)
        if (!dir.empty() && dir.has_relative_path() && !fs::exists(dir)) {
            std::error_code ec;
            fs::create_directories(dir, ec);
            if (ec) {
                std::cerr << "[ERROR] Failed to create directories: " << ec.message() << std::endl;
                return 1;
            }
        }

        fs::path full_file_path = dir / filename;

        // 2. Perform deletion if -d parameter was used
        if (force_delete) {
            remove_existing_file(full_file_path);
        }

        // 3. Process content: unescape command-line input, or use stdin raw
        std::string processed_content;
        if (read_from_stdin) {
            processed_content = code_content;
        } else {
            processed_content = unescape_string(code_content);
        }

        // 4. Write string content to the target file
        std::ofstream out_file(full_file_path, std::ios::out | std::ios::binary);
        if (!out_file.is_open()) {
            std::cerr << "[ERROR] Unable to create file at: " << full_file_path.string() << std::endl;
            return 1;
        }

        out_file.write(processed_content.c_str(), processed_content.size());
        out_file.close();

        std::cout << "SUCCESS: Written to " << full_file_path.string() << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Exception caught: " << e.what() << std::endl;
        return 1;
    }
}