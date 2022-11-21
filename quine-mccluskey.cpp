#include "Minterms.h"
#include "FileHandler.h"

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

int main(int argc, char **argv)
{
    FileReader reader(argv[1]);
    const unsigned int bits = reader.variablesSize();

    std::vector<std::vector<unsigned int>> inMinterms = reader.getMinterms();
    for (unsigned int i = 0; i < inMinterms.size(); i++)
    {
        std::vector<unsigned int> minterms = inMinterms[i];
        std::size_t size = minterms.size();
        std::vector<MintermGroup> mintermGroups;
        std::vector<Minterm> uncombinedTerms;

        // Create groups for each possible numbers of ones present in a term
        for (unsigned int i = 0; i <= bits; i++)
        {
            mintermGroups.insert(mintermGroups.end(), MintermGroup(i, bits));
        }

        // Sort minterms into groups correspoding to number of ones present
        for (unsigned int i = 0; i < size; i++)
        {
            unsigned int n = minterms[i];
            Minterm minterm(n, bits);
            mintermGroups[minterm.getOnes()].addTerm(minterm);
        }

        // Run algorithm
        algo(mintermGroups, uncombinedTerms);

        // Write result to file
        std::string outputFxn = "fxn_" + std::to_string(i);
        FileWriter writer(uncombinedTerms, reader.getVariables(), outputFxn + "_output.txt");
        writer.close();

        // Display result as SOP
        std::cout << outputFxn << " = ";
        for (unsigned int i = uncombinedTerms.size(); i > 0; i--)
        {
            uncombinedTerms[i - 1].printTerm(reader.getVariables());
            if (i - 1 != 0)
            {
                std::cout << " + ";
            }
        }
        std::cout << std::endl;
    }

    return 0;
}
