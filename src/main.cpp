#include <cstdlib>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <string>
#include <system_error>

int main(int argc, char *argv[]) {

    std::string stage;
    std::filesystem::path input;

    for (int i = 1; i < argc; i++) {

        std::string arg = argv[i];
        if (arg.starts_with("--") == 0)
            stage = arg;
        else
            input = arg;
    }

    if (input.empty()) {

        std::cerr << "usage: mycc [--stage] file.c\n";
        return 1;
    }

    std::filesystem::path preprocessed = input;
    preprocessed.replace_extension(".i");

    std::string cmd = "gcc -E -P " + input.string() + " -o " + preprocessed.string();

    if (std::system(cmd.c_str()) != 0) {

        std::cerr << "preprocessing failed\n";
        return 1;
    }

    std::ifstream file(preprocessed);
    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // auto tokens =  lex(source);

    std::filesystem::path asm_file = input;
    asm_file.replace_extension(".s");
    std::filesystem::remove(asm_file);
    std::filesystem::remove(preprocessed);

    return 0;
}
