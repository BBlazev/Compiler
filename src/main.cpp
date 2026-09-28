#include <cstdlib>

#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "asm_ast.hpp"
#include "codegen.hpp"
#include "emit.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "utils.hpp"

namespace fs = std::filesystem;

namespace {

enum class Stage { Lex, Parse, Codegen, Full };

struct Options {
    Stage stage = Stage::Full;
    bool dump_tokens = false;
    bool dump_ast = false;
    fs::path input;
};

void print_usage() {
    std::cerr << "usage: mycc [--lex | --parse | --codegen] [--dump-tokens] [--dump-ast] file.c\n";
}

bool parse_args(int argc, char *argv[], Options &opts) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "--lex") {
            opts.stage = Stage::Lex;
        } else if (arg == "--parse") {
            opts.stage = Stage::Parse;
        } else if (arg == "--codegen") {
            opts.stage = Stage::Codegen;
        } else if (arg == "--dump-tokens") {
            opts.dump_tokens = true;
        } else if (arg == "--dump-ast") {
            opts.dump_ast = true;
        } else if (arg.starts_with("-")) {
            std::cerr << "unknown option: " << arg << "\n";
            return false;
        } else if (!opts.input.empty()) {
            std::cerr << "only one input file allowed\n";
            return false;
        } else {
            opts.input = arg;
        }
    }

    if (opts.input.empty()) {
        std::cerr << "no input file\n";
        return false;
    }
    return true;
}

bool preprocess(const fs::path &input, std::string &out) {
    fs::path preprocessed = input;
    preprocessed.replace_extension(".i");

    std::string cmd = "gcc -E -P \"" + input.string() + "\" -o \"" + preprocessed.string() + "\"";
    if (std::system(cmd.c_str()) != 0)
        return false;

    std::ifstream file(preprocessed);
    out.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    file.close();

    fs::remove(preprocessed);
    return true;
}

} // namespace

int main(int argc, char *argv[]) {

    Options opts;
    if (!parse_args(argc, argv, opts)) {
        print_usage();
        return 1;
    }

    std::string source;
    if (!preprocess(opts.input, source)) {
        std::cerr << "preprocessing failed\n";
        return 1;
    }

    try {
        Lexer lexer(source);
        std::vector<Token> tokens = lexer.lex();

        if (opts.dump_tokens) {
            for (const auto &t : tokens)
                std::cout << t.line << ":" << t.column_number << "  " << t.type << "  '" << t.value
                          << "'\n";
        }
        if (opts.stage == Stage::Lex)
            return 0;

        Parser parser(tokens);
        Program program = parser.parse_program();

        if (opts.dump_ast) {
            std::cerr << "--dump-ast: AST printer not written yet\n";
        }
        if (opts.stage == Stage::Parse)
            return 0;

        x86::Program prog = gen_program(program);

        emit(std::cout, prog);
        if (opts.stage == Stage::Codegen)
            return 0;

        emit(std::cout, prog);

        return 1;

    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
}

// TODO: AST TREE PRINTER
