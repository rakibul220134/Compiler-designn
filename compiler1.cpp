#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include <set>

using namespace std;

// Check if a word is a keyword
bool isKeyword(const string& str) {
    static set<string> keywords = {
        "for", "while", "do", "int", "float", "char", "double", "static",
        "switch", "case", "if", "else", "then", "endif", "print"
    };
    return keywords.count(str) > 0;
}

// Function to remove comments from input file and write to output file
void removeComments(const string& inputFile, const string& outputFile) {
    ifstream fin(inputFile);
    ofstream fout(outputFile);

    if (!fin.is_open() || !fout.is_open()) {
        cerr << "Error opening files for comment removal." << endl;
        return;
    }

    char ch, next;
    bool in_single_line_comment = false;
    bool in_multi_line_comment = false;

    while (fin.get(ch)) {
        if (in_single_line_comment) {
            if (ch == '\n') {
                in_single_line_comment = false;
                fout << ch; // preserve newline
            }
            // else skip character
        }
        else if (in_multi_line_comment) {
            if (ch == '*') {
                if (fin.peek() == '/') {
                    fin.get(next); // consume '/'
                    in_multi_line_comment = false;
                }
            }
            // else skip character inside multi-line comment
        }
        else {
            if (ch == '/') {
                if (fin.peek() == '/') {
                    fin.get(next); // consume second '/'
                    in_single_line_comment = true;
                }
                else if (fin.peek() == '*') {
                    fin.get(next); // consume '*'
                    in_multi_line_comment = true;
                }
                else {
                    fout << ch;
                }
            }
            else {
                fout << ch;
            }
        }
    }

    fin.close();
    fout.close();
}

int main() {
    // Step 1: Take input and save to input.txt
    ofstream fout("input.txt");
    string line;
    cout << "Enter the C/C++ program (end input with a single '$' on a new line):" << endl;
    while (getline(cin, line)) {
        if (line == "$") break;
        fout << line << '\n';
    }
    fout.close();

    // Step 2: Remove comments and save to no_comments.txt
    removeComments("input.txt", "no_comments.txt");

    // Step 3: Open no_comments.txt for lexical analysis
    ifstream fin("no_comments.txt");
    ofstream fid("identifier.txt");
    ofstream fsc("specialchar.txt");

    vector<int> numbers;
    int lineno = 0;
    string word;
    char ch;

    while (fin.get(ch)) {
        if (isdigit(ch)) {
            int value = ch - '0';
            while (fin.get(ch) && isdigit(ch)) {
                value = value * 10 + (ch - '0');
            }
            numbers.push_back(value);
            fin.unget();
        }
        else if (isalpha(ch)) {
            word = ch;
            while (fin.get(ch) && (isalnum(ch) || ch == '_' || ch == '$')) {
                word += ch;
            }
            fid << word << ' ';
            fin.unget();
        }
        else if (ch == ' ' || ch == '\t') {
            continue;
        }
        else if (ch == '\n') {
            lineno++;
        }
        else {
            fsc << ch;
        }
    }

    fin.close();
    fid.close();
    fsc.close();

    // Output numbers
    cout << "\nThe numbers in the program are: ";
    for (int num : numbers) {
        cout << num << " ";
    }
    cout << "\n";

    // Process identifiers
    ifstream fidin("identifier.txt");
    cout << "The keywords and identifiers are:\n";
    while (fidin >> word) {
        if (isKeyword(word)) {
            cout << word << " is a keyword\n";
        } else {
            cout << word << " is an identifier\n";
        }
    }
    fidin.close();

    // Process special characters
    ifstream fscin("specialchar.txt");
    cout << "\nSpecial characters are: ";
    while (fscin.get(ch)) {
        cout << ch << " ";
    }
    fscin.close();

    cout << "\nTotal number of lines: " << lineno << "\n";

    return 0;
}
