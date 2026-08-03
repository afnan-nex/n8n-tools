#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

void print_help() {
    cout << "Usage: file-writer.exe -n \"filename.ext\" -p \"destination_path\" -c \"code_string\"\n";
    cout << "  -n  Filename with extension (supports spaces inside double quotes)\n";
    cout << "  -p  Destination folder path\n";
    cout << "  -c  Code string to write\n";
}

int main(int argc, char* argv[]) {
    string filename = "";
    string path = "";
    string code = "";

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];

        if (arg == "-n" && i + 1 < argc) {
            filename = argv[++i];
        } else if (arg == "-p" && i + 1 < argc) {
            path = argv[++i];
        } else if (arg == "-c" && i + 1 < argc) {
            code = argv[++i];
        } else if (arg == "-h" || arg == "--help") {
            print_help();
            return 0;
        }
    }

    // Validate inputs
    if (filename.empty() || path.empty() || code.empty()) {
        cerr << "Error: Missing required arguments.\n\n";
        print_help();
        return 1;
    }

    try {
        // Ensure destination directory exists
        fs::path dir(path);
        if (!fs::exists(dir)) {
            fs::create_directories(dir);
        }

        // Combine path and filename cleanly
        fs::path full_path = dir / filename;

        // Write content to the file
        ofstream out_file(full_path);
        if (!out_file.is_open()) {
            cerr << "Error: Could not create file at " << full_path.string() << endl;
            return 1;
        }

        out_file << code;
        out_file.close();

        cout << "Successfully saved: " << full_path.string() << endl;
    } 
    catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}