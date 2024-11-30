#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>
typedef std::string str;
typedef std::stringstream sst;
using std::cin;
using std::cout;
using std::endl;

// 模拟 AS-ATC 编译器逐行转换 ATC 代码为 C++代码
str convertLine(const std::string& line) {
    std::istringstream iss(line);
    std::string keyword;
    iss >> keyword;

    if (keyword == "import") {
        keyword="";
        iss >> keyword;
        return "#include";
    } else if (keyword == "func") {
        std::string funcDef;
        std::getline(iss, funcDef);
        if (funcDef.find("->")!= std::string::npos) {
            // 处理类似于 #define 的函数
            std::istringstream funcIss(funcDef);
            std::string varA, varB, arrow, expression;
            funcIss >> varA >> varB >> arrow >> expression;
            return "auto " + varA + varB + "{" + expression + "}";
        } else {
            // 处理普通函数
            return funcDef.substr(1) + "{" + funcDef.substr(funcDef.find("->") + 2) + "}";
        }
    } else if (keyword == "maindef(ARG)") {
        return "int main(int argc, char** argv){";
    } else if (keyword == "maindef()") {
        return "int main(){";
    } else if (keyword == "if") {
        std::string condition;
        std::getline(iss, condition);
        return "if (" + condition.substr(condition.find("(") + 1, condition.find(")") - condition.find("(") - 1) + "){";
    } else if (keyword == "fout") {
        std::string output;
        std::getline(iss, output);
        return "std::cout << " + output.substr(output.find("(") + 1, output.find(")") - output.find("(") - 1) + ";";
    } else {
        return line;
    }
}


int main(int argc, char** argv) {
    if (argc < 3 ) {
        std::cerr << "Usage: as-atc <YourATCFile> -o <OutPutFileName>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();

    std::ofstream outputFile(argv[3]);
    if (!outputFile) {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }

    for (const std::string& l : lines) {
        std::string convertedLine = convertLine(l);
        outputFile << convertedLine << std::endl;
    }

    outputFile.close();

    return 0;
}