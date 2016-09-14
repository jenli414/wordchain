/* Lab 2: Wordchain
 * jenli414
 * sabse455 */

#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <set>
#include <sstream>


using namespace std;

// Function declarations.
void printWelcomeMessage();
void getWordsFromUser(string& word1, string& word2);
void findShortestChain(const string& word1, const string& word2, stack<string>& result);
void setDictionary(set<string>& dictionary);
void setNeighbours(queue<string>& neighbours, const string& currentWord, set<string>& usedWords, const set<string>& dictionary);
void addNeighbour(const string& word, queue<string>& neighbours, set<string>& usedWords, const set<string>& dictionary);
bool isValidWord(const string& word, const set<string>& dictionary);
void printResult(const string& word1, const string& word2, stack<string>& result);
void printEndMessage();

// Constants.
const string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";

/* Main part of the program. Prints a welcome message, gets two words from the user
 * and finds the shortest path between them by moving between valid english words that
 * only differ by one letter. */
int main() {
    printWelcomeMessage();
    string word1;
    string word2;
    getWordsFromUser(word1, word2);
    stack<string> result;
    findShortestChain(word1, word2, result);
    printResult(word1, word2, result);
    printEndMessage();
    return 0;
}

// Prints welcome message.
void printWelcomeMessage() {
    cout << "Welcome to TDDD86 Word Chain." << endl;
    cout << "If you give me two English words, I will transform the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;
}

// Gets two words from the user and saves them in word1 and word2.
void getWordsFromUser(string& word1, string& word2){
    cout << "Please type two words: ";
    cin >> word1 >> word2;
    cout << endl;
}

// Finds the shortest path between word1 and word2 and stores the result in result.
void findShortestChain(const string& word1, const string& word2, stack<string>& result) {
    queue<stack<string>> chains;
    stack<string> firstChain;
    firstChain.push(word1);
    stack<string> currentChain;
    string currentWord;
    set<string> dictionary;
    setDictionary(dictionary);
    set<string> usedWords = {word1};
    queue<string> neighbours;
    if (word1.length() == word2.length()) {
        chains.push(firstChain);
    }
    while (!chains.empty()) {
        currentChain = chains.front();
        chains.pop();
        currentWord = currentChain.top();
        if (currentWord == word2) {
            result = currentChain;
            while (!chains.empty()) {
                chains.pop();
            }
        } else {
            setNeighbours(neighbours, currentWord, usedWords, dictionary);
            while (!neighbours.empty()) {
                stack<string> nextChain = currentChain;
                nextChain.push(neighbours.front());
                neighbours.pop();
                chains.push(nextChain);
            }
        }
    }
}

// Opens dictionary.txt and stores the words from it in a set<string>.
void setDictionary(set<string>& dictionary) {
    ifstream input;
    input.open("dictionary.txt");
    string word;
    while (getline(input, word)) {
        dictionary.insert(word);
        }
    input.close();
}

// Sets the queue of valid neighbours of a word.
void setNeighbours(queue<string>& neighbours, const string& currentWord,
                   set<string>& usedWords, const set<string>& dictionary) {
    string word;
    int currentWordLength = currentWord.length();
    int alphabethLength = ALPHABET.length();
    for (int j = 0; j < alphabethLength; ++j) {
        for (int i = 0; i < currentWordLength - 1; ++i) {
            stringstream ss;
            ss << currentWord.substr(0,i) << ALPHABET[j] <<
                  currentWord.substr(i + 1,currentWord.length() - i);
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

// Adds a word to neighbours if it hasn't already been used and is a valid word.
void addNeighbour(const string& word, queue<string>& neighbours, set<string>& usedWords,
                  const set<string>& dictionary) {
    if ((usedWords.count(word) != 1) && (isValidWord(word, dictionary))) {
        neighbours.push(word);
        usedWords.insert(word);
    }
}

// Returns whether a word is valid by checking if it's in the dictionary.
bool isValidWord(const string& word, const set<string>& dictionary) {
    return dictionary.count(word) == 1;
}

// Prints the result to the user.
void printResult(const string& word1, const string& word2, stack<string>& result) {
    cout << "Chain from " << word2 << " back to " << word1 << ":" << endl;
    if (result.empty()) {
        cout << "Couldn't find path from " << word2 << " back to " << word1 << ".";
    }
    while (!result.empty()) {
        cout << result.top() << " ";
        result.pop();
    }
    cout << endl;
}

// Prints end message to user.
void printEndMessage() {
    cout << "Have a nice day!" << endl;
}
