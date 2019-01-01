#include <iostream>
#include <string>
#include <cstring>

using namespace std;

//initializing global variables
	char Grid[6][7]; 				//array that represents all possible slots in the grid
	int TrueColumn = 0; 			//actual corresponding column in grid
	int LowRow = 0; 				//lowest possible row value in actual column corresponding to player selection
	
	string Player1_Name; 			//name of player 1
	string Player2_Name; 			//name of player 2
	string CurrentPlayer_Name; 		//name of player currently active during turn

	char Player1; 			//symbol corresponding to player 1 (X or O)
	char Player2; 			//symbol corresponding to player 2 (X or O)
	char CurrentPlayer; 	//symbol corresponding to player currently active during turn

//declaring functions
	void StartRoutine(); 					//runs a start routine
	void Initgrid();						//initializes the grid
	void AskXorO(); 						//asks players to choose between X and O
	void DisplayGrid(); 					//displays current grid state
	void AskMove(); 						//asks player to select a column
	int FindLowestRow(int ActualColumn); 	//returns lowest available slot in selected column
	bool CheckWin(char GridTest[6][7]); 	//checks whole grid if line of 4+ identical symbols has been formed

	
//main program
int main()
{
	int turn = 0; 		//used to alternate between players 1 and 2
	bool Over = false; 	//checks if a player has won	
	
	StartRoutine();
	do
		{
			if(turn%2 == 0)
			{
				cout << "Turn " << turn + 1 << endl; //displays current turn number
				CurrentPlayer = Player1;
				CurrentPlayer_Name = Player1_Name;
				cout << CurrentPlayer_Name << ", your turn! ";
				AskMove();
			}
			else
			{
				cout << "Turn " << turn + 1 << endl;  //displays current turn number
				CurrentPlayer = Player2;
				CurrentPlayer_Name = Player2_Name;
				cout << CurrentPlayer_Name << ", your turn! ";
				AskMove();
			}
			Over = CheckWin(Grid);
			turn++;
		}	while((Over == false) && (turn <= 41));
	
	if(Over)
	{
		cout << "Congratulations! " << CurrentPlayer_Name << " has won in " << turn << " turns!" << endl;
	}
	else if(turn > 41)
	{
		cout << "It's a tie!" << endl; 
	}
	
	return 0;
}	

//initializes the grid
void Initgrid()
{
	int i, j;
	for(i = 5; i >= 0; i--)
	{
		for(j = 0; j < 7; j++)
		{
			Grid[i][j] = ' ';
		}
	}
}

//runs a start routine
void StartRoutine()
{
	Initgrid();
	cout << "\nConnect Four Game" << endl;
	DisplayGrid();
	cout << endl;
		
		cout << "Player 1, please enter your name (no space): ";
		cin >> Player1_Name;
		AskXorO();
		
		cout << "\nPlayer 2, please enter your name (no space): ";
		cin >> Player2_Name;
		cout << Player2_Name << ", you are " << Player2 << "." << endl;
		
	DisplayGrid();
	cout << endl;
}

//asks players to choose between X and O
void AskXorO()
{
	do
	{	cout << Player1_Name << ", please choose X or O: ";
		cin >> Player1;
	}	while (Player1 != 'O' && Player1!= 'X');
	
	if(Player1 == 'O')
		{
			Player2 = 'X';
		}
	else
		{
			Player2 = 'O';
		}
}

//displays current grid state
void DisplayGrid()
{
	int i = 0, j = 0;
	cout << "\n 1 2 3 4 5 6 7" << endl; //displays top grid row
	for (i = 0; i < 12; i++) //iterates over all rows of grid, starting from top descending
	{
		if (i%2 == 0) //even lines must contain grid slots separated by | characters
			{
				for (j = 0; j <= 6; j++) //iterates over whole line, from left to right
				{
					cout << "|" << Grid[i/2][j];
				}	cout << "|" << endl;
			}
		else //odd lines must contain separations between rows
		{
			cout << "---------------";
			cout << endl;
		}
	}
}

//asks player to select a column
void AskMove()
{
	int Selection_Player = 0; 	//column selected by player
	string buffer1; 			//buffer to check that move selected by player is only one value
	char buffer2; 				//buffer to check that move selected by player is between 1 and 8
	
	cout << "Please choose a column: ";
	cin >> buffer1;
	buffer2 = buffer1[0];
	
	while((buffer1.size() > 2) || ((int(buffer2) >= 49 && int(buffer2) <= 55) == false))
	{
		cout << "The value you have entered is invalid. \nPlease type a number between 1 and 7: ";
		cin >> buffer1;
		buffer2 = buffer1[0];
	}
	
	Selection_Player = buffer2 - '0';
	TrueColumn = Selection_Player - 1;
	
	//puts player symbol in lowest available grid slot
	LowRow = FindLowestRow(TrueColumn);
	Grid[LowRow][TrueColumn] = CurrentPlayer;
	DisplayGrid();
	cout << endl;
}

//returns lowest available slot in selected column
int FindLowestRow(int ActualColumn)
{
	int LowestRow = 6; //start at bottom of grid + 1
	do
	{
		LowestRow--; //go up next slot
		if(LowestRow < 0)
		{
			cout << "Column " << TrueColumn + 1 << " is full." << endl;
			AskMove();
			LowRow = FindLowestRow(TrueColumn);
			Grid[LowRow][TrueColumn] = CurrentPlayer;
			DisplayGrid();
		}
	}	while (Grid[LowestRow][ActualColumn] == 'X' || Grid[LowestRow][ActualColumn] == 'O'); //check if current slot is already taken
	return LowestRow;
}

//checks whole grid if line of 4+ identical symbols has been formed
bool CheckWin(char GridTest[6][7])
{
int row = 0;
int column = 0;
bool win = false;
bool check = false;	
int a = 0;
int b = 0;
int c = 0;
int d = 0;
	
	for(row = 5; row >= 0; row--) //iterates from bottom to top row
		{
			for(column = 0; column <= 6; column++) //iterates on each row from left to right
			{
				if(GridTest[row][column] != ' ') //check slot is not empty
				{					
					for(a = 1; GridTest[row][column] == GridTest[row][column + a]; a++);
					for(b = 1; GridTest[row][column] == GridTest[row + b][column]; b++);
					for(c = 1; GridTest[row][column] == GridTest[row + c][column + c]; c++);
					for(d = 1; GridTest[row][column] == GridTest[row + d][column - d]; d++);
						
					check = ((a >= 4) || (b >= 4) || (c >= 4) || (d >= 4));
					if(check)
					{
						win = true;
					}
				}	
			}
		}
	return win;
}