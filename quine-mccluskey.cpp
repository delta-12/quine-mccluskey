#include "Minterms.h"

#define SIZE 10
#define BITS 4

void algo(std::vector<MintermGroup> mintermGroups, std::vector<Minterm> &uncombinedTerms)
{
    // Combine terms in adjacent groups
    bool combine = false;
    std::vector<MintermGroup> combinedGroups;
    for (unsigned int i = 1; i < mintermGroups.size(); i++)
    {
        MintermGroup *group = mintermGroups[i] + mintermGroups[i - 1];
        // Add result of combining terms in adjacent groups to combinedGroups vector
        if (group != nullptr)
        {
            combinedGroups.insert(combinedGroups.end(), *group);
            delete group;
            combine = true;
        }
    }
    // Add terms not combined in this step to uncombinedTerms vector
    for (unsigned int i = 0; i < mintermGroups.size(); i++)
    {
        std::vector<Minterm> terms = mintermGroups[i].uncombinedTerms().getTerms();
        uncombinedTerms.insert(uncombinedTerms.end(), terms.begin(), terms.end());
    }
    // Recursively call function if terms were combined
    if (combine)
    {
        algo(combinedGroups, uncombinedTerms);
    }
    // Remove duplicate terms
    else
    {
        unsigned int i = 0;
        while (i < uncombinedTerms.size())
        {
            unsigned int j = i + 1;
            while (j < uncombinedTerms.size())
            {
                if (uncombinedTerms[i] == uncombinedTerms[j])
                {
                    uncombinedTerms.erase(uncombinedTerms.begin() + j);
                }
                else
                {
                    j++;
                }
            }
            i++;
        }
    }
}

int main()
{
    unsigned int minterms[SIZE] = {0, 1, 2, 5, 6, 7, 8, 9, 10, 14};
    std::vector<MintermGroup> mintermGroups;
    std::vector<Minterm> uncombinedTerms;

    // Create groups for each possible numbers of ones present in a term
    for (unsigned int i = 0; i <= BITS; i++)
    {
        mintermGroups.insert(mintermGroups.end(), MintermGroup(i, BITS));
    }

    // Sort minterms into groups correspoding to number of ones present
    for (unsigned int i = 0; i < SIZE; i++)
    {
        unsigned int n = minterms[i];
        Minterm minterm(n, BITS);
        mintermGroups[minterm.getOnes()].addTerm(minterm);
    }

    // Run algorithm
    algo(mintermGroups, uncombinedTerms);

    // Display result as SOP
    std::cout << "f = ";
    for (unsigned int i = 0; i < uncombinedTerms.size(); i++)
    {
        uncombinedTerms[i].printTerm();
        if (i + 1 != uncombinedTerms.size())
        {
            std::cout << " + ";
        }
    }
    std::cout << std::endl;

    return 0;
}
