// #include <vector>
// #include <algorithm>
// #include <iostream>
// #include <fstream>
// #include <string>
// #include <sstream>
// #include <memory>
// typedef std::string str;
// typedef std::stringstream sst;
// using std::cin;
// using std::cout;
// using std::endl;

// // 模拟 AS-ATC 编译器逐行转换 ATC 代码为 C++代码
// str convertLine(const std::string& line) {
//     std::istringstream iss(line);
//     std::string keyword;
//     iss >> keyword;

//     if (keyword == "import") {
//         keyword="";
//         iss >> keyword;
//         return "#include";
//     } else if (keyword == "func") {
//         std::string funcDef;
//         std::getline(iss, funcDef);
//         if (funcDef.find("->")!= std::string::npos) {
//             // 处理类似于 #define 的函数
//             std::istringstream funcIss(funcDef);
//             std::string varA, varB, arrow, expression;
//             funcIss >> varA >> varB >> arrow >> expression;
//             return "auto " + varA + varB + "{" + expression + "}";
//         } else {
//             // 处理普通函数
//             return funcDef.substr(1) + "{" + funcDef.substr(funcDef.find("->") + 2) + "}";
//         }
//     } else if (keyword == "maindef(ARG)") {
//         return "int main(int argc, char** argv){";
//     } else if (keyword == "maindef()") {
//         return "int main(){";
//     } else if (keyword == "if") {
//         std::string condition;
//         std::getline(iss, condition);
//         return "if (" + condition.substr(condition.find("(") + 1, condition.find(")") - condition.find("(") - 1) + "){";
//     } else if (keyword == "fout") {
//         std::string output;
//         std::getline(iss, output);
//         return "std::cout << " + output.substr(output.find("(") + 1, output.find(")") - output.find("(") - 1) + ";";
//     } else {
//         return line;
//     }
// }


// int main(int argc, char** argv) {
//     if (argc < 3 ) {
//         std::cerr << "Usage: as-atc <YourATCFile> -o <OutPutFileName>" << std::endl;
//         return 1;
//     }

//     std::ifstream inputFile(argv[1]);
//     if (!inputFile) {
//         std::cerr << "Error opening input file." << std::endl;
//         return 1;
//     }
//     std::vector<std::string> lines;
//     std::string line;
//     while (std::getline(inputFile, line)) {
//         lines.push_back(line);
//     }
//     inputFile.close();

//     std::ofstream outputFile(argv[3]);
//     if (!outputFile) {
//         std::cerr << "Error opening output file." << std::endl;
//         return 1;
//     }

//     for (const std::string& l : lines) {
//         std::string convertedLine = convertLine(l);
//         outputFile << convertedLine << std::endl;
//     }

//     outputFile.close();

//     return 0;
// }

#include <iostream>

#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <exception>
#include <cstdio>

class FileOpenException : public std::exception {
private:
    std::string fileName;
public:
    FileOpenException(const std::string& fileName) : fileName(fileName) {}

    const char* what() const noexcept override {
        std::string message = "Error opening file: " + fileName;
        return message.c_str();
    }

    const std::string& getFileName() const {
        return fileName;
    }
};

int main(int argc, char* argv[]) {
    // 检查命令行参数是否少于 2 个
    if (argc < 2) {
        // 抛出一个运行时错误，提示用户如何使用该程序
        throw std::runtime_error("Usage: " + std::string(argv[0]) + " <filename>");
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    // 尝试打开输入文件
    std::ifstream inputFile(argv[1]);
    try{
       // 如果文件打开失败
       if (!inputFile) {
        // 抛出一个文件打开异常，异常信息为输入文件名
        throw FileOpenException("inputfile");
        // 检查文件是否打开
        inputFile.rdbuf()->is_open();
        return 1;
        }
    // 捕获文件打开异常
    }catch(FileOpenException& e){
        // 打印错误信息
        std::cerr << "Error opening file: " << e.getFileName() << std::endl;
    }
    // 定义一个哈希表，用于存储关键字及其替换词
    std::unordered_map<std::string, std::string> keywordMap;
    // 读取关键字文件
    std::string keywordFileName;
    // 如果命令行参数个数大于 2，则使用第三个参数作为关键字文件名
    if(argc > 2){
        keywordFileName = argv[2];
    }
    // 否则，使用默认文件名 "./res/keyword.txt"
    else{
        keywordFileName = "./res/keyword.txt";
    }
    // 尝试打开关键字文件
    std::ifstream keywordFile(keywordFileName);
    try{
        // 如果文件打开成功
        if (keywordFile) {
            // 读取文件中的每一行
            std::string line;
            while (std::getline(keywordFile, line)) {
                // 将每一行分割成关键字和替换词
                std::istringstream iss(line);
                std::string keyword, replacement, replacement2;
                // 如果分割成功
                if (iss >> keyword >> replacement) {
                    // 如果有多个替换词，将它们合并成一个字符串
                    while (iss >> replacement2) {
                        replacement = replacement + " " + replacement2;
                    }
                    std::cout<<replacement<<std::endl;
                    // 将关键字和替换词存入哈希表
                    keywordMap[keyword] = replacement;
                }
            }
            // 关闭关键字文件
            keywordFile.close();
        } 
        // 如果文件打开失败
        else {
            // 抛出一个文件打开异常，异常信息为关键字文件名
            throw FileOpenException(keywordFileName);
            return 1;
        }
    // 捕获文件打开异常
    }catch(FileOpenException& e){
        // 打印错误信息
        std::cerr << "Error opening file: " << e.getFileName() << std::endl;
    }
    keywordFile.close();

    std::string outputFileName;
    // 如果命令行参数个数大于 3，则使用第三个参数作为关键字文件名
    if(argc > 3){
        outputFileName = argv[3];
    }
    // 否则，使用默认文件名 "./res/keyword.txt"
    else{
        outputFileName = "./res/keyword.txt";
    }
    // 打开输出文件
    std::ofstream outputFile(outputFileName);
    // 检查输出文件是否打开
    try{
        // 如果文件打开失败
        if (!outputFile) {
            // 抛出一个文件打开异常，异常信息为输入文件名
            throw FileOpenException(outputFileName);
            return 1;
        }
    }catch(FileOpenException& e){
        // 打印错误信息
        std::cerr << "Error opening file: " << e.getFileName() << std::endl;
    }
    

    // 读取输入文件中的每一行
    std::string line;
    while (std::getline(inputFile, line)) {
        // 如果单词的第一个字符不是字母或者下划线，并且前面没有两个斜杠，则进行替换
        if (line.find("//")!= 0) {
            // 将非字母和非下划线字符替换为空格
            std::replace_if(line.begin(), line.end(), [](char c) { return!isalpha(c) && c!= '_'; }, ' ');
        }
        // 将每一行分割成单词
        std::istringstream iss(line);
        std::string word;
        // 读取每一个单词
        while (iss >> word) {
                // 如果关键字在哈希表中存在
                if (keywordMap.find(word)!= keywordMap.end()) {
                    // 获取关键字的替换词
                    std::string replacement = keywordMap[word];
                    // 将单词替换为关键字和替换词的组合
                    word = replacement;
                }
            
            // 输出处理后的单词
            std::cout << word << " ";
        }
        // 输出换行符
        std::cout << std::endl;
    }

    // 关闭输入文件
    inputFile.close();
    return 0;
}
