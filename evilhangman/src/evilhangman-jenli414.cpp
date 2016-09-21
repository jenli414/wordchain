#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <stack>
#include <set>
#include <limits>
#include <sstream>
#include <algorithm>
#include <locale>

using namespace std;

const string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";

void readDictionary(set<string>& dictionary);
void startGame(const set<string>& dictionary, const int& shortestWord, const int& longestWord);
void getWordLengthLimits(const set<string>& dictionary, int& shortestWord, int& longestWord);
void askForWordLength(int& wordLength, const int& shortestWord, const int& longestWord);
void askForNumOfGuesses(int& numOfGuesses);
void askIfShowNumOfPossibleWords(bool& showNumOfPossibleWords);
void setValidWords(const set<string>& dictionary, const int& wordLength, set<string>& validWords);
void askForGuess(set<char>& alreadyGuessedLetters, char& guess);
void findLargestWordFamily(const int& wordLength, const set<char>& includedLetters,
                           string& currentWordFamilyFormat, const char& guess,
                           queue<stack<string>>& wordFamilies, stack<string>& largestWordFamily,
                           bool& wordComplete);
bool isIncludedLetter(const char& letter, const string& word);
void setCurrentWordFamilyFormat(const set<char>& includedLetters, const stack<string>& wordFamily, string& currentWordFamilyFormat, bool& wordComplete);
void setVisibleToPlayer(const string& currenWordFamilyString, string& visibleToPlayer);
void printGuessStatus(const int& numOfGuesses, const set<char>& alreadyGuessedLetters, const string& currentResult);
void printNumOfPossibleWords(const int& numOfPossibleWords);
void printEndMessage(const bool& playerWon, const string& chosenWord);
void askForUserAction(char& userAction);


int main() {
    // Dictionary setup
    set<string> dictionary;
    readDictionary(dictionary);
    int shortestWord = -1; //Unsigned
    int longestWord = 0;
    getWordLengthLimits(dictionary, shortestWord, longestWord);
    //
    cout << "Welcome to Hangman." << endl << endl;
    char userAction = 's';
    bool run = true;
    while(run) {
        switch(userAction) {
        case 's':
            startGame(dictionary, shortestWord, longestWord);
            askForUserAction(userAction);
            break;
        case 'q':
            run = false;
        }
    }
    return 0;
}

void startGame(const set<string>& dictionary, const int& shortestWord, const int& longestWord) {
    // Game Settings user input
    int wordLength;
    askForWordLength(wordLength, shortestWord, longestWord);
    set<string> validWords;
    setValidWords(dictionary, wordLength, validWords);
    int numOfGuesses;
    askForNumOfGuesses(numOfGuesses);
    bool showNumOfPossibleWords;
    askIfShowNumOfPossibleWords(showNumOfPossibleWords);
    //
    char guess;
    set<char> alreadyGuessedLetters;
    set<char> includedLetters;
    string visibleToPlayer;
    string sampleWord;
    int numOfPossibleWords = 0;
    queue<stack<string>> wordFamilies;
    stack<string> largestWordFamily;
    for (auto const& word : validWords) {
        largestWordFamily.push(word);
        ++numOfPossibleWords;
    }
    wordFamilies.push(largestWordFamily);
    string currentWordFamilyFormat;
    bool wordComplete;
    bool gameOver = false;
    bool playerWon = false;
    string chosenWord;
    while (!gameOver) {
        askForGuess(alreadyGuessedLetters, guess);
        --numOfGuesses;
        if (!(numOfPossibleWords == 1)) {
            findLargestWordFamily(wordLength, includedLetters, currentWordFamilyFormat, guess, wordFamilies, largestWordFamily, wordComplete);
            numOfPossibleWords = largestWordFamily.size();
            sampleWord = largestWordFamily.top();
            if (isIncludedLetter(guess, sampleWord)) {
                includedLetters.insert(guess);
            }
        } else {
            if (chosenWord.empty()) {
                chosenWord = largestWordFamily.top();
            }
            if (isIncludedLetter(guess, chosenWord)) {
                includedLetters.insert(guess);
            }
        }
        setCurrentWordFamilyFormat(includedLetters, largestWordFamily, currentWordFamilyFormat, wordComplete);
        setVisibleToPlayer(currentWordFamilyFormat, visibleToPlayer);
        if (wordComplete) {
            gameOver = true;
            playerWon = true;
        } else if (numOfGuesses == 0) {
            if (chosenWord.empty()) {
                chosenWord = largestWordFamily.top();
            }
            gameOver = true;
            setVisibleToPlayer(chosenWord, visibleToPlayer);
        }
        printGuessStatus(numOfGuesses, alreadyGuessedLetters, visibleToPlayer);
        if (showNumOfPossibleWords) {
            printNumOfPossibleWords(largestWordFamily.front().size());
        }
    }
    printEndMessage(playerWon, chosenWord);
}


// Ask the user if it wants to play again and sets userAction accordingly.
void askForUserAction(char& userAction) {
    cout << "Play again? (y/n): ";
    string answer;
    bool validInput = false;
    while (!validInput) {
        cin >> answer;
        if (answer == "y") {
            userAction = 's';
            cout << endl;
            validInput = true;
        } else if (answer == "n") {
            userAction = 'q';
            cout << endl;
            validInput = true;
        } else {
            cout << endl << "Enter y for yes and n for no!: ";
        }
    }
}

// Reads the dictionary.txt file and puts the words in a set.
void readDictionary(set<string>& dictionary) {
    ifstream input;
    input.open("dictionary.txt");
    string word;
    while (getline(input, word)) {
        dictionary.insert(word);
    }
    input.close();
}

/* Sets shortestWord and longestWord to the length of the shortest and longest
 * word in the dictionary.*/
void getWordLengthLimits(const set<string>& dictionary, int& shortestWord,
                         int& longestWord) {
    string word;
    for (set<string>::const_iterator it =
         dictionary.begin(); it != dictionary.end(); ++it) {
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

/* Asks what wordlength the user wants.
 * Has to be an integer between the shortest and longest word in the dictionary.*/
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

// Asks how many guesses the user wants. Has to be an integer greater than 0.
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
            if (numOfGuesses > 26) {
                numOfGuesses = 26;
            }
            cout << endl;
            validInput = true;
        }
    }
}

// Asks if the user wants to see how many possible words remain.
void askIfShowNumOfPossibleWords(bool& showNumOfPossibleWords) {
    cout << "Show remaining words? (y/n): ";
    string answer;
    bool validInput = false;
    while (!validInput) {
        cin >> answer;
        if (answer == "y") {
            showNumOfPossibleWords = true;
            cout << endl;
            validInput = true;
        } else if (answer == "n") {
            showNumOfPossibleWords = false;
            cout << endl;
            validInput = true;
        } else {
            cout << endl << "Enter y for yes and n for no!: ";
        }
    }
}

// Prints number of possible words remaining.
void printNumOfPossibleWords(const int& numOfPossibleWords) {
    cout << "Possible words remaining: " << numOfPossibleWords << endl << endl;
}

// Sets validWords to the words in the dictionary with the correct wordlength.
void setValidWords(const set<string>& dictionary, const int& wordLength, set<string>& validWords) {
    if (!validWords.empty()) {
        validWords.clear();
    }
    for (auto const& word : dictionary) {
        if (word.length() == wordLength) {
           validWords.insert(word);
        }
    }
}

// Prints remaining guesses, already guessed letters and the current state of the game.
void printGuessStatus(const int& numOfGuesses, const set<char>& alreadyGuessedLetters, const string& currentResult) {
    cout << "Remaining guesses: " << numOfGuesses << endl;
    cout << "Guessed letters: ";
    for (set<char>::const_iterator it = alreadyGuessedLetters.begin(); it != alreadyGuessedLetters.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl << currentResult << endl << endl;
}

// Asks user for a valid guess.
void askForGuess(set<char>& alreadyGuessedLetters, char& guess) {
    string input;
    locale loc;
    cout << "Enter a guess: ";
    bool validInput = false;
    while (!validInput) {
        cin >> input;
        string::iterator firstChar = input.begin();
        if (input.length() != 1 || !(isalpha(*firstChar, loc))) {
            cout << endl << "Enter a LETTER!: ";
        } else {
            stringstream convert(input);
            convert >> guess;
            if (alreadyGuessedLetters.count(guess) == 1) {
                cout << endl << "You have already guessed that! Try again: ";
            } else {
                alreadyGuessedLetters.insert(guess);
                cout << endl;
                validInput = true;
            }
        }
    }
}

void findLargestWordFamily(const int& wordLength, const set<char>& includedLetters, string& currentWordFamilyFormat, const char& guess, queue<stack<string>>& wordFamilies, stack<string>& largestWordFamily, bool& wordComplete) {
    stack<string> currentWordFamily;
    string currentWord;
    largestWordFamily = currentWordFamily;
    stack<string> equalsGuess;
    stack<string> doesntEqualGuess;
    for (int count = 0; count < wordLength; ++count) {
        int initialWordFamilies = wordFamilies.size();
        for (int i = 0; i < initialWordFamilies; ++i) {
            currentWordFamily = wordFamilies.front();
            wordFamilies.pop();
            setCurrentWordFamilyFormat(includedLetters, currentWordFamily, currentWordFamilyFormat, wordComplete);
            if (includedLetters.count(currentWordFamilyFormat[count])) {
                wordFamilies.push(currentWordFamily);
            } else {
                while (!currentWordFamily.empty()) {
                    currentWord = currentWordFamily.top();
                    currentWordFamily.pop();
                    if (currentWord[count] == guess) {
                        equalsGuess.push(currentWord);
                    } else {
                        doesntEqualGuess.push(currentWord);
                    }
                }
                checkBranches(wordFamilies, equalsGuess, doesntEqualGuess);
            }
        }

    }
    setLargestWordFamily(wordFamilies, largestWordFamily);
    while (!wordFamilies.empty()) {
        currentWordFamily = wordFamilies.front();
        wordFamilies.pop();
        if (currentWordFamily.size() > largestWordFamily.size()) {
            largestWordFamily = currentWordFamily;
        }
    }
    wordFamilies.push(largestWordFamily);
}

void checkBranches(queue<stack<string>>& wordFamilies, stack<string>& equalsGuess,
                   stack<string>& doesntEqualGuess) {
    if (!(equalsGuess.empty())) {
        wordFamilies.push(equalsGuess);
        while (!(equalsGuess.empty())) {
            equalsGuess.pop();
        }
    }
    if (!(doesntEqualGuess.empty())) {
        wordFamilies.push(doesntEqualGuess);
        while (!(doesntEqualGuess.empty())) {
            doesntEqualGuess.pop();
        }
    }
}

bool isIncludedLetter(const char& letter, const string& word) {
    for (basic_string<char>::const_iterator it = word.begin(); it < word.end(); ++it) {
        if (*it == letter) {
            return true;
        }
    }
    return false;
}

// Sets up the string used to determine
void setCurrentWordFamilyFormat(const set<char>& includedLetters, const stack<string>& wordFamily, string& currentWordFamilyFormat, bool& wordComplete) {
    wordComplete = true;
    stringstream currentWordFamilyFormatStream;
    string sampleWord = wordFamily.top();
    for (string::iterator it = sampleWord.begin(); it < sampleWord.end(); ++it) {
        if (includedLetters.count(*it)) {
            currentWordFamilyFormatStream << *it;
        } else {
            wordComplete = false;
            currentWordFamilyFormatStream << "_";
        }
    }
    currentWordFamilyFormat = currentWordFamilyFormatStream.str();

}

// Sets up the string representation of the game that is shown to the player.
void setVisibleToPlayer(const string& currentWordFamilyFormat, string& visibleToPlayer) {
    stringstream visibleToPlayerStream;
    for (basic_string<char>::const_iterator it = currentWordFamilyFormat.begin(); it < currentWordFamilyFormat.end(); ++it) {
        visibleToPlayerStream << *it << " ";
    }
    visibleToPlayer = visibleToPlayerStream.str();
}

// Prints end message. Differs depending on if the player won or not.
void printEndMessage(const bool& playerWon, const string& chosenWord) {
    if (playerWon) {
        cout << "Congratulations! You beat me even though I may or may not have cheated..." << endl;
        cout << "The correct word was " << chosenWord << "!" << endl;
    } else {
        cout << "You're out of guesses! The correct word was " << chosenWord << endl << endl;
    }
}
