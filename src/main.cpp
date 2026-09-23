#include <cstdlib>

#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <string>
#include <system_error>

#include "lexer.hpp"

int main(int argc, char *argv[]) {

    std::string stage;
    std::filesystem::path input;

    for (int i = 1; i < argc; i++) {

        std::string arg = argv[i];
        if (arg.starts_with("--"))
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

    Lexer lexer(source);

    try {
        auto tokens = lexer.lex();
        for (auto &a : tokens)
            std::cout << a.value << "\n";
    } catch (std::exception &e) {
        std::cerr << e.what();

        std::filesystem::path asm_file = input;
        asm_file.replace_extension(".s");
        std::filesystem::remove(asm_file);
        std::filesystem::remove(preprocessed);
        return 1;
    }
    return 0;
}
