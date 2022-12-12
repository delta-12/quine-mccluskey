#include "Minterms.h"

// Count number of ones in minterm number and fill bits vector
unsigned int countOnes(unsigned int number, unsigned int bitCount, std::vector<unsigned char> &bits)
{
    unsigned int count = 0;
    for (unsigned int i = 0; i < bitCount; i++)
    {
        bool bit = number >> i & 1;
        bits.insert(bits.end(), bit);
        if (bit)
        {
            count++;
        }
    }
    return count;
}

// Count number of ones in minterm bits vector
unsigned int countOnes(std::vector<unsigned char> bits)
{
    unsigned int count = 0;
    for (unsigned int i = 0; i < bits.size(); i++)
    {
        if (bits[i] == 0x01)
        {
            count++;
        }
    }
    return count;
}

/*==========================================================================================================*/
/* Minterm Function Definitions */
/*==========================================================================================================*/

// Construct Minterm from minterm number and number of bits
Minterm::Minterm(unsigned int number, unsigned int bitCount)
{
    this->bitCount = bitCount;
    this->combined = false;
    this->ones = countOnes(number, bitCount, bits); /* count number of ones present in minterm
                                                    and fill bits vector */
}

// Construct Minterm from vector of bits of values 0x00, 0x01, or 0xff
Minterm::Minterm(std::vector<unsigned char> bits)
{
    this->bitCount = bits.size();
    this->bits = bits;
    this->ones = countOnes(bits);
    this->combined = false;
}

// Compare minterms by ones, bitCount, and bits
bool Minterm::operator==(Minterm minterm) const
{
    if (ones != minterm.getOnes() || bitCount != minterm.getBitCount() || bits != minterm.getBits())
    {
        return false;
    }
    return true;
}

// Returns combined term if minterms differ by single bit
Minterm *Minterm::operator+(Minterm minterm) const
{
    // Minterms must have same bit count, but different number of ones and bits
    if (ones == minterm.getOnes() || bitCount != minterm.getBitCount() || bits == minterm.getBits())
    {
        return nullptr;
    }

    // Find difference in bits and replace with 0xff
    std::vector<unsigned char> combinedBits;
    unsigned int diff = 0; // count difference between corresponding bits
    for (unsigned int i = 0; i < bitCount; i++)
    {
        unsigned char cBit = minterm.getBit(i);
        if (bits[i] != cBit)
        {
            if (bits[i] == 0xff || cBit == 0xff)
            {
                return nullptr; // cannot be combined if bits are different and one is 0xff
            }
            combinedBits.insert(combinedBits.end(), 0xff); // mark corresponding bits that differ with 0xff in combinedBits
            diff++;
        }
        else
        {
            combinedBits.insert(combinedBits.end(), cBit);
        }
    }

    // Must only be one difference in bits
    if (diff != 1)
    {
        return nullptr;
    }

    return new Minterm(combinedBits);
}

unsigned int Minterm::getOnes() const
{
    return ones;
}

unsigned int Minterm::getBitCount() const
{
    return bitCount;
}

unsigned char Minterm::getBit(unsigned int index) const
{
    return bits[index];
}

std::vector<unsigned char> Minterm::getBits() const
{
    return bits;
}

bool Minterm::getCombined() const
{
    return combined;
}

void Minterm::setCombined(bool combined)
{
    this->combined = combined;
}

// Print term MSB first
void Minterm::printTerm() const
{
    for (unsigned int i = bits.size(); i > 0; i--)
    {
        std::cout << (bits[i - 1] == 0xff ? "X" : std::to_string(bits[i - 1]));
    }
}

// Print term MSB first with variables
void Minterm::printTerm(std::vector<char> variables) const
{
    if (variables.size() != bitCount)
    {
        std::cout << "Mismatch variables and minterm size!" << std::endl;
        return;
    }
    for (unsigned int i = bits.size(); i > 0; i--)
    {
        unsigned char bit = bits[i - 1];
        if (bit != 0xff)
        {
            std::cout << variables[bitCount - i];
        }
        if (bit == 0x00)
        {
            std::cout << "'";
        }
    }
}

// Print term MSB first with variables to file
void Minterm::printTerm(std::vector<char> variables, std::ofstream &outFile) const
{
    if (variables.size() != bitCount)
    {
        std::cout << "Mismatch variables and minterm size!" << std::endl;
        return;
    }
    for (unsigned int i = bits.size(); i > 0; i--)
    {
        unsigned char bit = bits[i - 1];
        if (bit != 0xff)
        {
            outFile << variables[bitCount - i];
        }
        if (bit == 0x00)
        {
            outFile << "'";
        }
    }
}

/*==========================================================================================================*/

/*==========================================================================================================*/
/* MintermGroup Function Definitions */
/*==========================================================================================================*/

// Default MintermGroup constructor to specify number of ones present and number of bits in each term
MintermGroup::MintermGroup(unsigned int ones, unsigned int bitCount)
{
    this->ones = ones;
    this->bitCount = bitCount;
}

// Construct MintermGroup from vector of Minterms
MintermGroup::MintermGroup(std::vector<Minterm> terms)
{
    this->terms = terms;
    this->ones = terms[0].getOnes();
    this->bitCount = terms[0].getBitCount();
}

// Returns new MintermGroup with results of combining terms that differ by single bit
MintermGroup *MintermGroup::operator+(MintermGroup &mintermGroup)
{
    // Groups must have different number of ones and same bit count
    if (ones == mintermGroup.getOnes() || bitCount != mintermGroup.getBitCount())
    {
        return nullptr;
    }

    std::vector<Minterm> combinedTerms;
    for (unsigned int i = 0; i < terms.size(); i++)
    {
        for (unsigned int j = 0; j < mintermGroup.getTermCount(); j++)
        {
            Minterm *cTerm = mintermGroup.getTerm(j);
            Minterm *combinedTerm = terms[i] + *cTerm;
            if (combinedTerm != nullptr)
            {
                // add term to combined group
                combinedTerms.insert(combinedTerms.end(), *combinedTerm);
                delete combinedTerm;

                // set both terms as combined
                terms[i].setCombined(true);
                cTerm->setCombined(true);
            }
        }
    }

    if (combinedTerms.size() > 0)
    {
        return new MintermGroup(combinedTerms);
    }
    return nullptr;
}

// Append MintermGroup
bool MintermGroup::operator+=(MintermGroup mintermGroup)
{
    return append(mintermGroup);
}

unsigned int MintermGroup::getOnes() const
{
    return ones;
}

unsigned int MintermGroup::getBitCount() const
{
    return bitCount;
}

unsigned int MintermGroup::getTermCount() const
{
    return terms.size();
}

std::vector<Minterm> MintermGroup::getTerms() const
{
    return terms;
}

Minterm *MintermGroup::getTerm(unsigned int index)
{
    return terms.data() + index;
}

// Get all minterms in the group not yet able to combine with another
MintermGroup MintermGroup::uncombinedTerms() const
{
    MintermGroup uncombined(ones, bitCount);
    for (unsigned int i = 0; i < terms.size(); i++)
    {
        if (!terms[i].getCombined())
        {
            uncombined.addTerm(terms[i]);
        }
    }
    return uncombined;
}

void MintermGroup::setOnes(unsigned int ones)
{
    this->ones = ones;
}

void MintermGroup::setBitCount(unsigned int bitCount)
{
    this->bitCount = bitCount;
}

// Add minterm to group if correct number of ones present
bool MintermGroup::addTerm(Minterm minterm)
{
    if (minterm.getBitCount() != bitCount || minterm.getOnes() != ones)
    {
        return false;
    }
    terms.insert(terms.end(), minterm);
    return true;
}

// Append another MintermGroup to group
bool MintermGroup::append(MintermGroup appendGroup)
{
    // Group to be appended must have same number of ones and bit count
    if (ones != appendGroup.getOnes() || bitCount != appendGroup.getBitCount())
    {
        return false;
    }
    std::vector<Minterm> appendTerms = appendGroup.getTerms();
    terms.insert(terms.end(), appendTerms.begin(), appendTerms.end());
    return true;
}

/*==========================================================================================================*/
