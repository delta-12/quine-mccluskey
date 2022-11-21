#ifndef __FILEHANDLER_H__
#define __FILEHANDLER_H__

#include <fstream>
#include <iostream>
#include <vector>

// Class to read and write minterms and expressions to files
class FileHandler
{
private:
    std::vector<std::string> lines;
    std::vector<char> variables;
    std::vector<std::vector<unsigned int>> minterms;

public:
    FileHandler(std::string);
    bool readFile(std::string);
    std::size_t linesSize() const;
    std::size_t variablesSize() const;
    std::size_t mintermsSize() const;
    std::vector<char> getVariables() const;
    std::vector<std::vector<unsigned int>> getMinterms() const;
};

#endif