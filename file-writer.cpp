#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

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
              << "  file-writer.exe -d \"filename.ext\" -p \"C:\\target\\path\" -c \"code content\"\n\n"
              << "Parameters:\n"
              << "  -n <filename> : Specify output filename.\n"
              << "  -d <filename> : Specify output filename AND force-delete any existing file first.\n"
              << "  -p <path>     : Destination directory path.\n"
              << "  -c <content>  : Raw code content (supports escape sequences like \\n).\n";
}

int main(int argc, char* argv[]) {
    std::string filename;
    std::string folder_path;
    std::string code_content;
    bool force_delete = false;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if ((arg == "-n" || arg == "-d") && i + 1 < argc) {
            filename = argv[++i];
            if (arg == "-d") {
                force_delete = true;
            }
        } else if (arg == "-p" && i + 1 < argc) {
            folder_path = argv[++i];
        } else if (arg == "-c" && i + 1 < argc) {
            code_content = argv[++i];
        }
    }

    // Input validation
    if (filename.empty() || folder_path.empty()) {
        std::cerr << "[ERROR] Missing required arguments.\n\n";
        print_usage();
        return 1;
    }

    try {
        fs::path dir(folder_path);
        
        // 1. Create target directories if missing
        if (!dir.empty() && !fs::exists(dir)) {
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

        // 3. Unescape raw string input (\n -> newline, etc.)
        std::string processed_content = unescape_string(code_content);

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