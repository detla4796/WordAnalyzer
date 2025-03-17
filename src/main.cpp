#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <cstdlib>

using namespace std;

unique_ptr<vector<string>> readFile(const string& fname) {
    auto lines = make_unique<vector<string>>();
    ifstream f(fname);
    if (!f.is_open()) {
        cout << "Error opening file: " << fname << endl;
        return lines;
    }
    
    string line;
    while (getline(f, line)) {
        lines->push_back(line);
    }
    return lines;
}

int* countWords(const string& str) {
    int* count = (int*)malloc(sizeof(int));
    if (!count) return nullptr;
    
    *count = 0;
    bool inWord = false;
    
    for (char c : str) {
        if (c == ' ' || c == '\t') {
            inWord = false;
        } else if (!inWord) {
            (*count)++;
            inWord = true;
        }
    }
    return count;
}

string* findLongestWord(const string& str) {
    string* longest = (string*)malloc(sizeof(string));
    if (!longest) return nullptr;
    
    *longest = "";
    string word = "";
    
    for (char c : str) {
        if (c == ' ' || c == '\t') {
            if (word.length() > longest->length()) {
                *longest = word;
            }
            word = "";
        } else {
            word += c;
        }
    }
    if (word.length() > longest->length()) {
        *longest = word;
    }
    return longest;
}

unique_ptr<string> replaceVowels(const string& str) {
    auto r = make_unique<string>(str);
    string vowels = "aeiouAEIOU";
    for (char& c : *r) {
        if (vowels.find(c) != string::npos) {
            c = '*';
        }
    }
    return r;
}

void results(const string& fname, const vector<string>& olines, const vector<int*>& wcount, const vector<string*>& lword, const vector<unique_ptr<string>>& vowels) {
    ofstream f(fname);
    if (!f.is_open()) {
        cout << "Error opening output file: " << fname << endl;
        return;
    }

    f << "Original lines:\n";
    for (const auto& l : olines) {
        f << l << endl;
    }

    f << "\nWord counts:\n";
    for (size_t i = 0; i < wcount.size(); i++) {
        f << "Line " << i + 1 << ": " << *wcount[i] << " words" << endl;
    }

    f << "\nLongest word:\n";
    for (size_t i = 0; i < lword.size(); i++) {
        f << "Line " << i + 1 << ": " << *lword[i] << endl;
    }

    f << "\nVowels replaced:\n";
    for (size_t i = 0; i < vowels.size(); i++) {
        f << "Line " << i + 1 << ": " << *vowels[i] << endl;
    }
}

int main() {
    string inputFile = "input.txt";
    string outputFile = "output.txt";
    
    auto lines = readFile(inputFile);
    vector<int*> wordCounts;
    vector<string*> longestWords;
    vector<unique_ptr<string>> replacedVowels;

    for (const auto& line : *lines) {
        wordCounts.push_back(countWords(line));
        longestWords.push_back(findLongestWord(line));
        replacedVowels.push_back(replaceVowels(line));
    }

    results(outputFile, *lines, wordCounts, longestWords, replacedVowels);
    
    for (auto ptr : wordCounts) free(ptr);
    for (auto ptr : longestWords) free(ptr);
    
    return 0;
}
