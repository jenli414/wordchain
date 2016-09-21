#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <set>
#include <limits>

using namespace std;

const string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";

void readDictionary(set<string>& dictionary);
void printWelcomeMessage();
void getWordLengthLimits(const set<string>& dictionary, int& shortestWord, int& longestWord);
void askForWordLength(int& wordLength, const int& shortestWord, const int& longestWord);
void askForNumOfGuesses(int& numOfGuesses);
void askIfShowRemainingWords(bool& showRemainingWords);
void printRemainingWords(const int& numOfRemainingWords);
void getRemainingWords(const set<string>& dictionary, const int& wordLength, set<string>& remainingWords);
void printGuessStatus(const int& numOfGuesses);
void askForGuess(set<char>& alreadyGuessedLetters, char& guess);
void printEndMessage(const bool& playerWon, const string& chosenWord);
void newGame();


int main() {
    set<string> dictionary;
    readDictionary(dictionary);
    printWelcomeMessage();
    int shortestWord = -1; //Unsigned
    int longestWord = 0;
    getWordLengthLimits(dictionary, shortestWord, longestWord);
    int wordLength;
    askForWordLength(wordLength, shortestWord, longestWord);
    int numOfGuesses;
    askForNumOfGuesses(numOfGuesses);
    bool showRemainingWords;
    askIfShowRemainingWords(showRemainingWords);
    set<string> remainingWords;
    getRemainingWords(dictionary, wordLength, remainingWords);
    return 0;
}

void readDictionary(set<string>& dictionary) {
    ifstream input;
    input.open("dictionary.txt");
    string word;
    while (getline(input, word)) {
        dictionary.insert(word);
        }
    input.close();
}

void printWelcomeMessage() {
    cout << "Welcome to Hangman." << endl;
}

void getWordLengthLimits(const set<string>& dictionary, int& shortestWord, int& longestWord) {
    string word;
    for (set<string>::const_iterator it = dictionary.begin(); it != dictionary.end(); ++it) {
        word = *it;
        if (shortestWord == -1) {
            shortestWord = word.length();
        } else if (word.length() < shortestWord) { //Unsigned
            shortestWord = word.length();
        }
        if (word.length() > longestWord) { //Unsigned
            longestWord = word.length();
        }
    }
}

void askForWordLength(int& wordLength, const int& shortestWord, const int& longestWord) {
    cout << "Enter a word length between " << shortestWord << " and " << longestWord << ": ";
    bool validInput = false;
    while (!validInput) {
        if (!(cin >> wordLength)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //FRÅGA KIM OM DETTA
            cout << endl << "Enter a NUMBER between " << shortestWord << " and " << longestWord << "!: ";

        } else if ((wordLength < shortestWord) || (wordLength > longestWord)) {
            cout << endl << "Enter a number BETWEEN " << shortestWord << " and " << longestWord << "!: ";
        } else {
            cout << endl;
            validInput = true;
        }
    }
}

void askForNumOfGuesses(int& numOfGuesses) {
    cout << "Enter number of guesses: ";
    bool validInput = false;
    while (!validInput) {
        if (!(cin >> numOfGuesses)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //FRÅGA KIM OM DETTA
            cout << endl << "Enter a NUMBER greater than 0!: ";
        } else if (numOfGuesses < 1) {
            cout << endl << "Enter a number GREATER than 0!: ";
        } else {
            cout << endl;
            validInput = true;
        }
    }
}

void askIfShowRemainingWords(bool& showRemainingWords) {
    cout << "Show remaining words? (y/n): ";
    char answer;
    bool validInput = false;
    while (!validInput) {
        if (!(cin >> answer)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //FRÅGA KIM OM DETTA
            cout << endl << "Enter y for yes and n for no!" << endl;
        } else if (answer == 'y') {
            showRemainingWords = true;
            cout << endl;
            validInput = true;
        } else if (answer == 'n') {
            showRemainingWords = false;
            cout << endl;
            validInput = true;
        }
    }
}

void printRemainingWords(const int& numOfRemainingWords) {
    cout << "Remaining words: " << numOfRemainingWords << endl;
}

void getRemainingWords(const set<string>& dictionary, const int& wordLength, set<string>& remainingWords) {
    for (auto const& word : dictionary) {
        if (word.length() == wordLength) {
           remainingWords.insert(word);
        }
    }
}

void printGuessStatus(const int& numOfGuesses, const set<char>& alreadyGuessedLetters, const string& currentResult) {
    cout << "Remaining guesses: " << numOfGuesses << "." << endl;
    cout << "Guessed letters: ";
    for (set<char>::const_iterator it = alreadyGuessedLetters.begin(); it != alreadyGuessedLetters.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl << currentResult;
}

void askForGuess(set<char>& alreadyGuessedLetters, char& guess) {
    cout << "Enter a guess: ";
    bool validInput = false;
    while (!validInput) {
        if (!(cin >> guess)) {
            cout << endl << "Enter a LETTER!" << endl;
        } else if (alreadyGuessedLetters.count(guess) == 1) {
            cout << endl << "You have already guessed that!" << endl;
        } else {
            cout << endl;
            validInput = true;
        }
    }
}

void printEndMessage(const bool& playerWon, const string& chosenWord) {
    if (playerWon) {
        cout << "Congratulations! You beat me even though I may or may not have cheated..." << endl;
    } else {
        cout << "You're out of guesses! The correct word was " << chosenWord << endl;
    }
}

void newGame();
