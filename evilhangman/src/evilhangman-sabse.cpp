#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <set>
#include <limits>
#include <queue>
#include <sstream>
#include <cmath>


using namespace std;

const string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";

void readDictionary(set<string>& dictionary);
void printWelcomeMessage();
void getWordLengthLimits(const set<string>& dictionary, int& shortestWord, int& longestWord);
void askForWordLength(int& wordLength, const int& shortestWord, const int& longestWord);
void askForNumOfGuesses(int& numOfGuesses);
void askIfShowRemainingWords(bool& showRemainingWords);
void printRemainingWords(const int& numOfRemainingWords);
void getWords(const set<string>& dictionary, const int& wordLength, queue<stack<string>>& possibleWords);
void printGuessStatus(const int& numOfGuesses, const set<char>& alreadyGuessedLetters, const string& currentResult);
void askForGuess(set<char>& alreadyGuessedLetters, char& guess);
void playRound(queue<stack<string>>& possibleWords, const char& guess, const int& wordLength);
void getBiggestFamily(queue<stack<string>>& possibleWords);
void updateCurrentResult(const stack<string>& wordStack, string& currentResult, const char& guess);
void setCurrentResult(string& currentResult, const int& wordLength);
bool checkPlayerWon(const string& currentResult);

void printEndMessage(const bool& playerWon, const string& chosenWord);


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
    bool playerWon = false;
    string currentResult;
    setCurrentResult(currentResult, wordLength);
    set<char> alreadyGuessedLetters;
    queue<stack<string>> possibleWords;
    getWords(dictionary, wordLength, possibleWords);
    char guess;
    while (!playerWon && (numOfGuesses > 0)){
        numOfGuesses--;
        askForGuess(alreadyGuessedLetters, guess);
        alreadyGuessedLetters.insert(guess);
        playRound(possibleWords, guess, wordLength);
        updateCurrentResult(possibleWords.front(),currentResult, guess);
        playerWon = checkPlayerWon(currentResult);
        if (!playerWon){
            printGuessStatus(numOfGuesses, alreadyGuessedLetters, currentResult);
            if (showRemainingWords){
                printRemainingWords(possibleWords.front().size());
            }
        }
    }
    stack<string> finalStack = possibleWords.front();
    printEndMessage(playerWon, finalStack.top());
    return 0;
}

void readDictionary(set<string>& dictionary) {
    ifstream input;
    input.open("di.txt");
    string word;
    while (getline(input, word)) {
        dictionary.insert(word);
        }
    input.close();
}


void updateCurrentResult(const stack<string>& wordStack, string& currentResult, const char& guess){
    string word = wordStack.top();
    stringstream ss;
    for (int i = 0; i < word.length(); ++i) {
        if (guess == word[i]){
            ss << guess;
        } else {
            ss << currentResult[i];
        }
    }
    currentResult = ss.str();
}

void setCurrentResult(string& currentResult, const int& wordLength){
    stringstream ss;
    for (int i =0; i< wordLength; ++i){largestWordFamily = currentWordFamily;
        ss << "-";
    }
    currentResult = ss.str();
}

bool checkPlayerWon(const string& currentResult){
    for (int i = 0; i < currentResult.length(); ++i) {
        if (currentResult[i]== '-'){
            return false;
        }
    }
    return true;
}

void playRound(queue<stack<string>>& possibleWords, const char& guess, const int& wordLength){
    string currentWord;
    for (int ind=0; ind <wordLength; ++ind){
        for (int stackCounter =0; stackCounter <= exp2(ind); ++stackCounter){
            stack<string> containsGuess;
            stack<string> doesntContainGuess;
            stack<string> wordsToSort;
            wordsToSort = possibleWords.front(); //flytta ut?
            possibleWords.pop();
            while (!wordsToSort.empty()){
                currentWord = wordsToSort.top();
                wordsToSort.pop();
                if (currentWord[ind] == guess){
                    containsGuess.push(currentWord);
                } else {
                    doesntContainGuess.push(currentWord);
                }
            }
            possibleWords.push(containsGuess);
            possibleWords.push(doesntContainGuess);
        }
    }
    getBiggestFamily(possibleWords);

}

void getBiggestFamily(queue<stack<string>>& possibleWords){
    stack<string> biggestFamily = possibleWords.front();
    possibleWords.pop();
    int currentMax = biggestFamily.size();
    stack<string> currentFamily;
    while (!possibleWords.empty()){
        currentFamily = possibleWords.front();
        possibleWords.pop();
        if (currentFamily.size() > currentMax){
            biggestFamily = currentFamily;
            currentMax = biggestFamily.size();
        }
    }
    possibleWords.push(biggestFamily);
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

void getWords(const set<string>& dictionary, const int& wordLength, queue<stack<string>>& possibleWords ) {
    stack<string> wordStack;
    for (auto const& word : dictionary) {
        if (word.length() == wordLength) {
           wordStack.push(word);
        }
    }
    possibleWords.push(wordStack);
}

void printGuessStatus(const int& numOfGuesses, const set<char>& alreadyGuessedLetters, const string& currentResult) {
    cout << "Remaining guesses: " << numOfGuesses << "." << endl;
    cout << "Guessed letters: ";
    for (set<char>::const_iterator it = alreadyGuessedLetters.begin(); it != alreadyGuessedLetters.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl << currentResult << endl;
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
            alreadyGuessedLetters.insert(guess);
            validInput = true;
        }
    }
}

void printEndMessage(const bool& playerWon, const string& chosenWord) {
    if (playerWon) {            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //FRÅGA KIM OM DETTA
        cout << chosenWord <<endl << "Congratulations! You beat me even though I may or may not have cheated..." << endl;
    } else {
        cout << "You're out of guesses! The correct word was " << chosenWord << endl;
    }
}
