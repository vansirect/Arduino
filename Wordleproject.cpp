#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

const int notmatched = 0;
// not matched means no value is in the correct position or in the word
// partial matched means there is a letter in the word but not in the correct position
// matched means the letter is in the word and in the correct position
const int partialmatched = 1;
const int matched = 2;
const int guessesnumber = 6;
const int wordlength = 5;

void toUpper(string& input) {
    transform(input.begin(), input.end(), input.begin(), [](char c) {
        return toupper(c);
        });
    /*
    std::transform(input_begin, input_end, output_begin, unary_op); unary op is the function to be done to each element
    [](char c) {
        return toupper(c);
        }// lambda function to return the letter to upper case 

     // sytntax [ capture clause ] (parameters) -> return-type
{
     definition of method
}



    
    */

}


string getrandomword() {
    return "READY";
}

bool isvalid(string word) {
    return word.length() == wordlength && word.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == string::npos;
    /* checks if the length is 5 and if it contains any of the above letters
    *find_first_not_of finds the first letter that doesnt match the above letters
    * npos means it wasnt founs
    * 
    
    
    
    */
}

void markMatch(vector<vector<int>>& matches, int tryindex, string target, string guess) {
    // not matched  means no character matched or was in 
    for (int i = 0; i < wordlength; i++) {
        matches[tryindex][i] = notmatched;// setting all the values to not matched ensuring we have a clean slate before starting
    }
    for (int j = 0; j < wordlength; j++) {
        //this loop goes through each character in  guess[j] and guess is what we input
        for (int i = 0; i < wordlength; i++) {
        // this loop goes through each character in target[i] where target is the letter we want
            if (guess[j] == target[i]) {
                // this checks if the letters are the same
                if (i == j) {
                    matches[tryindex][j] = matched;
                    // this checks if they are in the same position
                    break;
                }
                else {
                    // since its else , it means its the correct letter but wrong position so its partial matched
                    matches[tryindex][j] = partialmatched;
                }
                /*
                * the result in matches could be matches[0] = {2, 2, 2, 0, 0};
                * where 2 is matched , 0 is not matched
                
                
                */ 



                // if its neither it remains notmatched
            }
        }
    }
}

bool isallmatch(string target, string guess) {
    // just checks whether all match
    for (int i = 0; i < wordlength; i++) {
        if (guess[i] != target[i]) {
            return false;
        }
    }
    return true;
}

void printwordle(vector<string> tries, vector<vector<int>> matches, int currenttry) {
    system("cls"); 
    cout << "==================== WORDLE ====================" << endl;
    for (int i = 0; i <= currenttry && i < tries.size(); i++) {
        /*current try is how many guesses the player has made e.g  currenttry = 2
        * Tries is the word  input like  ["READY", "WORDS", "GUESS"]
        */
        string row = "| ";
        for (int j = 0; j < wordlength; j++) {
            char letter = tries[i][j];
            if (matches[i][j] == matched) {
                row += "\033[32m"; //if its matched then its set to green
            }
            else if (matches[i][j] == partialmatched) {
                row += "\033[33m"; //yellow
            }
            row += letter;// so fills the row with each letter 
            
            row += "\033[0m "; // none matched then no text
            // so "| \033[32mR\033[0m E \033[33mA\033[0m D \033[32mY\033[0m |"   \033[32mR sets the color R to green

        }
        row += "|";
        cout << row << endl;
    }
    cout << "================================================" << endl;
}

int main() {
    vector<string> guesses(guessesnumber);
    vector<vector<int>> matches(guessesnumber, vector<int>(wordlength, notmatched));
    string targetword = getrandomword();
    string input;
    int currenttry = 0;

    while (currenttry < guessesnumber) {
        cout << "Enter your guess (or Q to quit): ";
        getline(cin, input);
        toUpper(input);

        if (input == "Q") {
            cout << "Game ended. Goodbye!" << endl;
            break;
        }

        if (!isvalid(input)) {
            cout << "Invalid input! Guess must be " << wordlength << " uppercase letters." << endl;
            continue;
        }

        guesses[currenttry] = input;
        markMatch(matches, currenttry, targetword, input);
        printwordle(guesses, matches, currenttry);

        if (isallmatch(targetword, input)) {
            cout << "Congratulations! You found the word: " << targetword << endl;
            break;
        }

        currenttry++;
        if (currenttry == guessesnumber) {
            cout << "You've used all attempts. The word was: " << targetword << endl;
        }
    }
    
    return 0;
} 
