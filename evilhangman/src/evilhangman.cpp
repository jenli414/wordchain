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
void getWordLengthLimits(const set<string>& dictionary, int& shortestWord,
                         int& longestWord);
void startGame(const set<string>& dictionary, const int& shortestWord,
               const int& longestWord);
void askForWordLength(int& wordLength, const int& shortestWord,
                      const int& longestWord);
void askForNumOfGuesses(int& numOfGuesses);
void askIfShowNumOfPossibleWords(bool& showNumOfPossibleWords);
void setInitialLargestWordFamily(const set<string>& dictionary,
                                 const int& wordLength,
                                 stack<string>& largestWordFamily);
void askForGuess(set<char>& alreadyGuessedLetters, char& guess);
void findLargestWordFamily(const int& wordLength,
                           const set<char>& alreadyGuessedLetters,
                           const char& guess, stack<string>& largestWordFamily);
void checkBranches(queue<stack<string>>& wordFamilies, stack<string>& equalsGuess,
                   stack<string>& doesntEqualGuess);
void keepLargestWordFamily(queue<stack<string>>& wordFamilies);
bool isComplete(const set<char>& alreadyGuessedLetters, const string& chosenWord);
void printGuessStatus(const int& numOfGuesses,
                      const set<char>& alreadyGuessedLetters,
                      const string& currentResult);
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
        }void askForGuess(set<char>& alreadyGuessedLetters, char& guess);
    }
    return 0;
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

void startGame(const set<string>& dictionary, const int& shortestWord,
               const int& longestWord) {
    int wordLength;
    askForWordLength(wordLength, shortestWord, longestWord);
    int numOfGuesses;
    askForNumOfGuesses(numOfGuesses);
    bool showNumOfPossibleWords;
    askIfShowNumOfPossibleWords(showNumOfPossibleWords);
    stack<string> largestWordFamily;
    setInitialLargestWordFamily(dictionary, wordLength, largestWordFamily);
    char guess;
    set<char> alreadyGuessedLetters;
    bool gameOver = false;
    bool playerWon = false;
    while (!playerWon && numOfGuesses > 0) {
        askForGuess(alreadyGuessedLetters, guess);
        --numOfGuesses;
        if (largestWordFamily.size() == 1) {
            alreadyGuessedLetters.insert(guess);
            if (isComplete(alreadyGuessedLetters, largestWordFamily.top())) {
                playerWon = true;
                gameOver = true;
            }
        } else {
             findLargestWordFamily(wordLength, alreadyGuessedLetters, guess,
                                   largestWordFamily);
             alreadyGuessedLetters.insert(guess);
             if (largestWordFamily.size() == 1 &&
                     isComplete(alreadyGuessedLetters, largestWordFamily.top())) {
                     playerWon = true;
                     gameOver = true;
             }
        }
        printGuessStatus(numOfGuesses, alreadyGuessedLetters,
                         largestWordFamily.top());
        if (showNumOfPossibleWords) {
            printNumOfPossibleWords(largestWordFamily.size());
        }
    }
    printEndMessage(playerWon, largestWordFamily.top());
}

/* Asks what wordlength the user wants. Has to be an integer between
 * the shortest and longest word in the dictionary.*/
void askForWordLength(int& wordLength, const int& shortestWord,
                      const int& longestWord) {
    cout << "Enter a word length between " << shortestWord << " and "
         << longestWord << ": ";
    bool validInput = false;
    while (!validInput) {
        if (!(cin >> wordLength)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //FRÅGA KIM OM DETTA
            cout << endl << "Enter a NUMBER between " << shortestWord << " and "
                 << longestWord << "!: ";

        } else if ((wordLength < shortestWord) || (wordLength > longestWord)) {
            cout << endl << "Enter a number BETWEEN " << shortestWord << " and "
                 << longestWord << "!: ";
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
        } else if (answer == "n") {void getWordLengthLimits(const set<string>& dictionary, int& shortestWord,
                                                            int& longestWord);
            showNumOfPossibleWords = false;
            cout << endl;
            validInput = true;
        } else {
            cout << endl << "Enter y for yes and n for no!: ";
        }
    }
}

/* Sets the initial largest word family to the words in the dictionary
 * with the given wordlength. */
void setInitialLargestWordFamily(const set<string>& dictionary,
                                 const int& wordLength,
                                 stack<string>& largestWordFamily) {
    for (auto const& word : dictionary) {
        if (word.length() == wordLength) {
           largestWordFamily.push(word);
        }
    }
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
                cout << endl;
                validInput = true;
            }
        }
    }
}

/* Sets largestWordFamily to the largest possible word family depending on
 * what letter the player has guessed.*/
void findLargestWordFamily(const int& wordLength,
                           const set<char>& alreadyGuessedLetters,
                           const char& guess, stack<string>& largestWordFamily) {
    queue<stack<string>> wordFamilies;
    wordFamilies.push(largestWordFamily);
    stack<string> currentWordFamily;
    string currentWord;
    stack<string> equalsGuess;
    stack<string> doesntEqualGuess;
    for (int count = 0; count < wordLength; ++count) {
        int initialWordFamilies = wordFamilies.size();
        for (int i = 0; i < initialWordFamilies; ++i) {
            currentWordFamily = wordFamilies.front();
            wordFamilies.pop();
            if (alreadyGuessedLetters.count(currentWordFamily.top()[count])) {
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
    keepLargestWordFamily(wordFamilies);
    largestWordFamily = wordFamilies.front();
}

/* Checks if the resulting stacks of the sorting in findLargestWordFamily contain
 * any words and should be pushed back to the wordFamilies queue.
 * Also clears the stacks if needed.*/
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

// Removes every word family in wordFamilies but the largest one.
void keepLargestWordFamily(queue<stack<string>>& wordFamilies) {
    int biggestFamily = wordFamilies.front().size();
    wordFamilies.push(wordFamilies.front());
    wordFamilies.pop();
    stack<string> currentFamily;
    while (!(wordFamilies.size() == 1)) {
        currentFamily = wordFamilies.front();
        if (currentFamily.size() > biggestFamily) {
            wordFamilies.push(currentFamily);
            biggestFamily = currentFamily.size();
        }
        wordFamilies.pop();
    }
}

/* Returns whether a word is complete, i.e. every letter in chosenWord
 * has been guessed. */
bool isComplete(const set<char>& alreadyGuessedLetters, const string& chosenWord) {
    bool wordComplete = true;
    for (string::const_iterator it = chosenWord.begin();
         it < chosenWord.end(); ++it) {
        if (!alreadyGuessedLetters.count(*it)) {
            wordComplete = false;
        }
    }
    return wordComplete;
}

/* Prints remaining guesses, already guessed letters and the current
 * state of the game. */
void printGuessStatus(const int& numOfGuesses,
                      const set<char>& alreadyGuessedLetters,
                      const string& currentResult) {
    cout << "Remaining guesses: " << numOfGuesses << endl;
    cout << "Guessed letters: ";
    for (set<char>::const_iterator it = alreadyGuessedLetters.begin();
         it != alreadyGuessedLetters.end(); ++it) {
        cout << *it << " ";
    }
    stringstream visibleToPlayer;
    for (string::const_iterator it = currentResult.begin();
         it != currentResult.end(); ++it) {
        if (alreadyGuessedLetters.count(*it)) {
            visibleToPlayer << *it << " ";
        } else {
            visibleToPlayer << "_ ";
        }
    }
    cout << endl << visibleToPlayer.str() << endl << endl;
}

// Prints number of possible words remaining.
void printNumOfPossibleWords(const int& numOfPossibleWords) {
    cout << "Possible words remaining: " << numOfPossibleWords << endl << endl;
}

// Prints end message. Differs depending on if the player won or not.
void printEndMessage(const bool& playerWon, const string& chosenWord) {
    if (playerWon) {
        cout << "Congratulations! You beat me even though I may or may not have cheated..."
             << endl;
    } else {
        cout << "You're out of guesses!" << endl;
    }
    cout << "The correct word was " << chosenWord << "!" << endl << endl;
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
