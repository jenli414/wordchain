#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <unordered_set>
#include <sstream>


using namespace std;


void printWelcomeMessage();
void getWordsFromUser(string& w1, string& w2);
void findShortestChain(string& w1, string& w2, stack<string>& result);
void setDictionary(unordered_set<string>& dictionary);
void setNeighbours(queue<string>& neighbours, string& currentWord, unordered_set<string>& usedWords, unordered_set<string>& dictionary);
void addNeighbour(string& word, queue<string>& neighbours, unordered_set<string>& usedWords, unordered_set<string>& dictionary);
bool isValidWord(string& word, unordered_set<string>& dictionary);
void printResult(string& w1, string& w2, stack<string>& result);
void printEndMessage();


const string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";


int main() {
    printWelcomeMessage();
    string w1;
    string w2;
    getWordsFromUser(w1, w2);
    stack<string> result;
    findShortestChain(w1, w2, result);
    if (!result.empty()) {
        printResult(w1, w2, result);
    } else {
        cout << "Couldn't find path from " << w2 << " back to " << w1 << "." << endl;
    }
    printEndMessage();
    return 0;
}


void printWelcomeMessage() {
    cout << "Welcome to TDDD86 Word Chain." << endl;
    cout << "If you give me two English words, I will transform the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;
}


void getWordsFromUser(string& w1, string& w2){
    cout << "Please type two words: ";
    cin >> w1 >> w2;
    cout << endl;
}


void findShortestChain(string& w1, string& w2, stack<string>& result) {
    queue<stack<string>> q;
    stack<string> firstStack;
    firstStack.push(w1);
    stack<string> currentStack;
    string currentWord;
    unordered_set<string> dictionary;
    setDictionary(dictionary);
    unordered_set<string> usedWords = {w1};
    queue<string> neighbours;
    if (w1.length() == w2.length()) {
        q.push(firstStack);
    }
    while (!q.empty()) {
        currentStack = q.front();
        q.pop();
        currentWord = currentStack.top();
        if (currentWord == w2) {
            result = currentStack;
            while (!q.empty()) {
                q.pop();
            }
        } else {
            setNeighbours(neighbours, currentWord, usedWords, dictionary);
            while (!neighbours.empty()) {
                stack<string> nextStack = currentStack;
                nextStack.push(neighbours.front());
                neighbours.pop();
                q.push(nextStack);
            }
        }
    }
}


void setDictionary(unordered_set<string>& dictionary) {
    ifstream input;
    input.open("dictionary.txt");
    string word;
    while (getline(input, word)) {
        dictionary.insert(word);
        }
    input.close();
}


void setNeighbours(queue<string>& neighbours, string& currentWord, unordered_set<string>& usedWords, unordered_set<string>& dictionary) {
    string word;
    int currentWordLength = currentWord.length();
    int alphabethLength = ALPHABET.length();
    for (int j = 0; j < alphabethLength; ++j) {
        for (int i = 0; i < currentWordLength - 1; ++i) {
            stringstream ss;
            ss << currentWord.substr(0,i) << ALPHABET[j] << currentWord.substr(i + 1,currentWord.length() - i);
            word = ss.str();
            addNeighbour(word, neighbours, usedWords, dictionary);
        }
    }
    string wordMinusLastLetter = currentWord.substr(0,currentWordLength - 1);
    for (int j = 0; j < alphabethLength; ++j) {
        stringstream ss;
        ss << wordMinusLastLetter << ALPHABET[j];
        word = ss.str();
        addNeighbour(word, neighbours, usedWords, dictionary);
    }
}


void addNeighbour(string& word, queue<string>& neighbours, unordered_set<string>& usedWords, unordered_set<string>& dictionary) {
    if ((usedWords.count(word) != 1) && (isValidWord(word, dictionary))) {
        neighbours.push(word);
        usedWords.insert(word);
    }
}


bool isValidWord(string& word, unordered_set<string>& dictionary) {
    if (dictionary.count(word) == 1) {
        return true;
    } else {
        return false;
    }
}


void printResult(string& w1, string& w2, stack<string>& result) {
    cout << "Chain from " << w2 << " back to " << w1 << ":" << endl;
    while (!result.empty()) {
        cout << result.top() << " ";
        result.pop();
    }
    cout << endl;
}


void printEndMessage() {
    cout << "Have a nice day!" << endl;
}
