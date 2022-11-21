#ifndef __MINTERMS_H__
#define __MINTERMS_H__

#include <iostream>
#include <vector>

// Function prototypes
unsigned int countOnes(unsigned int, unsigned int);

// Class to store single minterm
class Minterm
{
private:
    unsigned int ones, bitCount;     // number of ones present, number of bits
    std::vector<unsigned char> bits; // values of each bit in minterm; 0x00, 0x01, or 0xff for combined
    bool combined;                   // whether term has been able to combine with another

public:
    Minterm(unsigned int number = 0, unsigned int bitCount = 1);
    Minterm(std::vector<unsigned char>);
    bool operator==(Minterm) const;
    Minterm *operator+(Minterm) const;
    unsigned int getOnes() const;
    unsigned int getBitCount() const;
    unsigned char getBit(unsigned int) const;
    std::vector<unsigned char> getBits() const;
    bool getCombined() const;
    void setCombined(bool);
    void printTerm() const;
    void printTerm(std::vector<char>) const;
};

// Class to store minterms with same number of ones present
class MintermGroup
{
private:
    unsigned int ones, bitCount; // number of ones present, number of bits
    std::vector<Minterm> terms;  // terms with same number of bits and ones present

public:
    MintermGroup(unsigned int ones = 0, unsigned int bitCount = 1);
    MintermGroup(std::vector<Minterm>);
    MintermGroup *operator+(MintermGroup &);
    bool operator+=(MintermGroup);
    unsigned int getOnes() const;
    unsigned int getBitCount() const;
    unsigned int getTermCount() const;
    std::vector<Minterm> getTerms() const;
    Minterm *getTerm(unsigned int);
    MintermGroup uncombinedTerms() const;
    void setOnes(unsigned int);
    void setBitCount(unsigned int);
    bool addTerm(Minterm);
    bool append(MintermGroup);
};

#endif