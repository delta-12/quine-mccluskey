#include "FileHandler.h"

/*==========================================================================================================*/
/* FileReader Function Definitions */
/*==========================================================================================================*/

FileReader::FileReader(std::string filename)
{
    readFile(filename);
}

bool FileReader::readFile(std::string filename)
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

std::size_t FileReader::linesSize() const
{
    return lines.size();
}

size_t FileReader::variablesSize() const
{
    return variables.size();
}

size_t FileReader::mintermsSize() const
{
    return minterms.size();
}

std::vector<char> FileReader::getVariables() const
{
    return variables;
}

std::vector<std::vector<unsigned int>> FileReader::getMinterms() const
{
    return minterms;
}

/*==========================================================================================================*/

/*==========================================================================================================*/
/* FileWriter Function Definitions */
/*==========================================================================================================*/

FileWriter::FileWriter(std::string filename)
{
    setFilename(filename);
}

FileWriter::FileWriter(std::vector<Minterm> minterms, std::vector<char> variables, std::string filename)
{
    setFilename(filename);
    write(minterms, variables);
}

FileWriter::~FileWriter()
{
    close();
}

void FileWriter::setFilename(std::string name)
{
    filename = name;
}

void FileWriter::write(std::vector<Minterm> minterms, std::vector<char> variables)
{
    if (!outFile.is_open())
    {
        outFile.open(filename);
    }
    outFile << "fxn = ";
    for (unsigned int i = minterms.size(); i > 0; i--)
    {
        minterms[i - 1].printTerm(variables, outFile);
        if (i - 1 != 0)
        {
            outFile << " + ";
        }
    }
    outFile << std::endl;
}

void FileWriter::close()
{
    if (outFile.is_open())
    {
        outFile.close();
    }
}

/*==========================================================================================================*/
