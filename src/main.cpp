#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <cstdlib>

using namespace std;

vector<string>* readFile(const string& fname)
{
    vector<string>* lines = new vector<string>();
    ifstream f(fname);
    if (!f.is_open())
    {
        cout << "Error opening file: " << fname << endl;
        return lines;
    }
    
    string line;
    while (getline(f, line))
    {
        lines->push_back(line);
    }
    return lines;
}

int* countWords(const string& str)
{
    int* count = new int;
    *count = 0;
    bool inWord = false;
    
    for (char c : str)
    {
        if (c == ' ' || c == '\t')
        {
            inWord = false;
        }
        else if (!inWord)
        {
            (*count)++;
            inWord = true;
        }
    }
    return count;
}

string* findLongestWord(const string& str)
{
    string* longest = new string("");
    string word = "";
    
    for (char c : str)
    {
        if (c == ' ' || c == '\t')
        {
            if (word.length() > longest->length())
            {
                *longest = word;
            }
            word = "";
        }
        else
        {
            word += c;
        }
    }
    
    if (word.length() > longest->length())
    {
        *longest = word;
    }
    return longest;
}

string* replaceVowels(const string& str)
{
    string* r = new string(str);
    string vowels = "aeiouAEIOU";
    for (char& c : *r)
    {
        if (vowels.find(c) != string::npos)
        {
            c = '*';
        }
    }
    return r;
}

bool results(const string& fname, const vector<string>& olines, const vector<int*>& wcount, const vector<string*>& lword, const vector<string*>& vowels)
{
    ofstream f(fname);
    if (!f.is_open())
    {
        cout << "Error opening output file: " << fname << endl;
        return false;
    }

    f << "Original lines:\n";
    for (const auto& l : olines)
    {
        f << l << endl;
    }

    f << "\nWord counts:\n";
    for (size_t i = 0; i < wcount.size(); i++)
    {
        f << "Line " << i + 1 << ": " << *wcount[i] << " words" << endl;
    }

    f << "\nLongest word:\n";
    for (size_t i = 0; i < lword.size(); i++)
    {
        f << "Line " << i + 1 << ": " << *lword[i] << endl;
    }

    f << "\nVowels replaced:\n";
    for (size_t i = 0; i < vowels.size(); i++)
    {
        f << "Line " << i + 1 << ": " << *vowels[i] << endl;
    }
    
    f.close();
    return true;
}

int main()
{
    string inputFile = "input.txt";
    string outputFile = "output.txt";
    
    vector<string>* lines = readFile(inputFile);
    if (lines->empty())
    {
        cout << "No data to process, exiting." << endl;
        delete lines;
        return 1;
    }
    
    vector<int*> wordCounts;
    vector<string*> longestWords;
    vector<string*> replacedVowels;

    for (const auto& line : *lines)
    {
        wordCounts.push_back(countWords(line));
        longestWords.push_back(findLongestWord(line));
        replacedVowels.push_back(replaceVowels(line));
    }

    if (!results(outputFile, *lines, wordCounts, longestWords, replacedVowels))
    {
        cout << "Failed to write results to file." << endl;
        
        // Освобождаем память
        delete lines;
        for (auto ptr : wordCounts) delete ptr;
        for (auto ptr : longestWords) delete ptr;
        for (auto ptr : replacedVowels) delete ptr;
        
        return 1;
    }
    else
    {
        cout << "Results successfully written to " << outputFile << endl;
    }
    
    delete lines;
    for (auto ptr : wordCounts) delete ptr;
    for (auto ptr : longestWords) delete ptr;
    for (auto ptr : replacedVowels) delete ptr;
    
    return 0;
}