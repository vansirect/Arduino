#include<iostream>
using namespace std;

void display(char brd[8][8]) {
    cout << "  "; // Space for row numbers
    for (int i = 0; i < 8; ++i) {
		cout << i << " "; // we are printing the column numbers at the top of the board
    }
    cout << endl;

    for (int i = 0; i < 8; ++i) {
		cout << i << " "; // we are also printing the row numbers on the left side of the board
        for (int j = 0; j < 8; ++j) {
			cout << brd[i][j] << " ";// so basically when its done completing the row it prints the row number and then goes to the next row
        }
        cout << endl;
    }
}

void initboard(char brd[8][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((i + j) % 2 == 0)
               /*basically what we a are doing here is checking the square is light or dark
			   * because in checkers you place the piece on dark squares
               * 
			   * so lets say if where  i = 0 and j = 0 then 0+0 = 0 and 0%2 = 0 so it will be light square
			   * and if i = 0 and j = 1 then 0+1 = 1 and 1%2 = 1 so it will be dark square
               * 
               * 
               * 
               
               */
            {
                brd[i][j] = ' '; // Light square
            }
            else {
                if (i < 3) {
					brd[i][j] = 'X'; //here its less than 3 because i is the row number and we are placing the pieces on the top 3 rows
                }
                else if (i > 4) {
                    brd[i][j] = 'O'; 
                }
                else {
                    brd[i][j] = '.'; 
                }
            }
        }
    }
}
bool isValidMove(char brd[8][8], int startRow, int startCol, int endRow, int endCol, char player) {
    // Check bounds
    if (startRow < 0 || startRow >= 8 || startCol < 0 || startCol >= 8 ||
        endRow < 0 || endRow >= 8 || endCol < 0 || endCol >= 8) {
        return false;// since its a 8x8 board you cant have anything above 8 or below 0 
    }

   
    if (brd[startRow][startCol] != player) {
        // checking if position youre entering to move a player has a player
        return false;
    }

    
    if (brd[endRow][endCol] != '.') {
        // checks if youre moving to a position that should be empty
        return false;
    }

  
    int rowDiff = endRow - startRow;
    int colDiff = abs(endCol - startCol);
    /* basically what we are trying to do here is to ensure youre only moving
    * up a row lets say  (2, 3) to  (3, 4) for the row difference rowDiff = 3 - 2 = 1 and colDiff = |4 - 3| = 1  where abs is the modulus
	* this is valid because it should be a diagonal move moving by 1 row and 1 column not more than that
    * 
    
    
    
    */

	if (colDiff != 1) { // ensuring its only one column move
        return false;
    }

    if (player == 'X') {
		if (rowDiff != 1) { //here X moves down so it should be a positive row difference
            return false;
        }
    }
    else if (player == 'O') {
		if (rowDiff != -1) { // here O moves up so it should be a negative row difference
            return false;
        }
    }

    return true;
}
void makeMove(char brd[8][8], int startRow, int startCol, int endRow, int endCol) {
    
	brd[endRow][endCol] = brd[startRow][startCol];// we are setting the end position to the player
    brd[startRow][startCol] = '.'; // set the starting position to empty
}
void getInput(char brd[8][8], char player, int& startRow, int& startCol, int& endRow, int& endCol) {
    while (true) {
        cout  << player << ", enter move (startRow ,startCol ,endRow ,endCol): ";
        cin >> startRow >> startCol >> endRow >> endCol;

        if (isValidMove(brd, startRow, startCol, endRow, endCol, player)) {
            break; // Valid move, exit the loop
        }
        else {
            cout << "Invalid move. Try again." << endl;
        }
    }
}
void playGame(char brd[8][8], char& currentPlayer) {
    while (true) {
        int startRow, startCol, endRow, endCol;
        getInput(brd, currentPlayer, startRow, startCol, endRow, endCol);   
        makeMove(brd, startRow, startCol, endRow, endCol);
        display(brd);
		currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';/*
		 we are basically switching the player if the current player is X we switch it to O and vice versa for the next turn
        
        
        */
    }
}





int main() {
    char brd[8][8];
    initboard(brd);
    display(brd);
	char currentplayer = 'X';
	playGame(brd, currentplayer);
    

    return 0;
}
