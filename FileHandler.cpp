#include "FileHandler.h"

FileHandler::FileHandler(std::string filename)
{
    readFile(filename);
}

bool FileHandler::readFile(std::string filename)
{
    std::ifstream inFile(filename);
    if (inFile.fail())
    {
        std::cout << "Cannot open file" << std::endl;
        return false;
    }

    // Read in file line-by-line
    while (!inFile.eof())
    {
        std::string line;
        getline(inFile, line);
        lines.insert(lines.end(), line);
    }
    inFile.close();

    // Get variables from first line of file
    if (lines.size() > 0)
    {
        std::string varLine = lines[0];
        unsigned int pos = varLine.find(',');
        while (pos < varLine.length())
        {
            variables.insert(variables.end(), char(varLine[pos - 1]));
            pos = varLine.find(',', ++pos);
        }
        variables.insert(variables.end(), char(varLine[varLine.length() - 1]));

        // Get minterms from remaining lines
        for (unsigned int i = 1; i < lines.size(); i++)
        {
            std::vector<unsigned int> terms;
            std::string minLine = lines[i];
            unsigned int begin = 0, end = minLine.find(',', begin);
            while (end < minLine.length())
            {
                int minterm = stoi(minLine.substr(begin, end - begin));
                terms.insert(terms.end(), minterm);
                begin = end + 1;
                end = minLine.find(',', begin);
            }
            terms.insert(terms.end(), stoi(minLine.substr(begin, minLine.length() - begin)));
            minterms.insert(minterms.end(), terms);
        }
    }
    else
    {
        std::cout << "Invalid file" << std::endl;
        return false;
    }

    return true;
}

std::size_t FileHandler::linesSize() const
{
    return lines.size();
}

size_t FileHandler::variablesSize() const
{
    return variables.size();
}

size_t FileHandler::mintermsSize() const
{
    return minterms.size();
}

std::vector<char> FileHandler::getVariables() const
{
    return variables;
}

std::vector<std::vector<unsigned int>> FileHandler::getMinterms() const
{
    return minterms;
}
