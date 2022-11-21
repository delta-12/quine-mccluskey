#ifndef __FILEHANDLER_H__
#define __FILEHANDLER_H__

#include "Minterms.h"

// Class to read write minterms from files
class FileReader
{
private:
    std::vector<std::string> lines;
    std::vector<char> variables;
    std::vector<std::vector<unsigned int>> minterms;

public:
    FileReader(std::string);
    bool readFile(std::string);
    std::size_t linesSize() const;
    std::size_t variablesSize() const;
    std::size_t mintermsSize() const;
    std::vector<char> getVariables() const;
    std::vector<std::vector<unsigned int>> getMinterms() const;
};

// Class to write expressions to files
class FileWriter
{
private:
    std::string filename;
    std::ofstream outFile;

public:
    FileWriter(std::string filename = "fxn_output.txt");
    FileWriter(std::vector<Minterm>, std::vector<char>, std::string filename = "fxn_output.txt");
    ~FileWriter();
    void setFilename(std::string);
    void write(std::vector<Minterm>, std::vector<char>);
    void close();
};

#endif