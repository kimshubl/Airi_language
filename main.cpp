#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "Semantic/SemanticAnalyzer.h"
#include "CodeGen/CodeGenerator.h"

std::string readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void compileContext(const std::string& filepath, const std::string& outpath, bool expectFailure = false) {
    try {
        std::cout << "\n--- Compiling: " << filepath << " ---" << std::endl;
        std::string source = readFile(filepath);
        
        // 1. Lexing
        Lexer lexer(source);
        std::vector<Token> tokens;
        Token t = lexer.next_token();
        while (t.type != TokenType::END_OF_FILE) {
            tokens.push_back(t);
            t = lexer.next_token();
        }
        tokens.push_back(t); // add EOF Token
        
        // 2. Parsing
        Parser parser(tokens);
        auto astRoot = parser.parseProgram();
        
        // 3. Semantic Analysis
        SemanticAnalyzer analyzer;
        analyzer.analyze(dynamic_cast<ProgramNode*>(astRoot.get()));
        
        // 4. Code Generation
        CodeGenerator generator;
        std::string cppCode = generator.generate(dynamic_cast<ProgramNode*>(astRoot.get()));
        
        std::ofstream outFile(outpath);
        outFile << cppCode;
        outFile.close();

        std::cout << "Compilation pipeline success! output generated mapping to " << outpath << "." << std::endl;

    } catch (const std::exception& e) {
        if (expectFailure) {
            std::cout << "Expected Failure Caught: " << e.what() << std::endl;
        } else {
            std::cerr << "Unexpected Compilation Failed: " << e.what() << std::endl;
            exit(1);
        }
    }
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./main_integrated <filename.airi> [output.cpp]" << std::endl;
        return 1;
    }

    std::string filepath = argv[1];
    std::string outpath = (argc >= 3) ? argv[2] : "output.cpp";

    compileContext(filepath, outpath);

    std::string binPath = outpath.substr(0, outpath.find_last_of('.')) + "_bin";
    std::string buildCmd = "g++ -std=c++14 " + outpath + " -o " + binPath;
    
    std::cout << "\n[Builder] compiling generated file: " << buildCmd << std::endl;
    int ret = std::system(buildCmd.c_str());
    if (ret == 0) {
        std::cout << "\n[Runner] Executing: ./" << binPath << std::endl;
        std::system(("./" + binPath).c_str());
    } else {
        std::cerr << "[Builder] C++ Compilation failed! Please ensure your Airi script contains a main() function." << std::endl;
    }

    return 0;
}
