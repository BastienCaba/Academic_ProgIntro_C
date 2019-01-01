#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;

//----------------------------------------------------------------------
// functions declaration
//----------------------------------------------------------------------

int askchoice(int minvalue, int numberofoptions); //used to check user input

//----------------------------------------------------------------------
// class Cell
//----------------------------------------------------------------------

class Cell	//class cell containing temporary board and functions relative to normal cell behaviour
{
	protected:
		int temprow;						//# rows of temporary board
		int tempcolumn;						//# columns of temporary board
		vector< vector<char> > tempboard;	//temporary board used to store temporarily state of board during updating process
		
	public:
		Cell(); 																//default constructor
		Cell(int temprowin, int tempcolumnin);									//constructor used
		~Cell() {cout << "Cell destructor was called successfully." << endl;};	//destructor
		Cell(const Cell& cellin);												//copy function
		
		virtual vector< vector<char> > get_temp_board() {return tempboard;};	//access function for tempboard
		int get_temprow() {return temprow;};									//access function for tempboard
		int get_tempcolumn() {return tempcolumn;};								//access function for tempcolumn
		
		//virtual function relative to normal cell behaviour
		virtual void set_state(vector< vector<char> > inputboard);				//inputs a board, applies cell behaviour to it, and stores next step board in tempboard
};

Cell::Cell()
{
	vector<char> temp_row;
	temp_row.push_back(' ');
	tempboard.push_back(temp_row);
}

Cell::Cell(int temprowin, int tempcolumnin)
{
	int i = 0;
	int j = 0;
	temprow = temprowin;
	tempcolumn = tempcolumnin;
	
	for(i=0; i<(temprow+2); i++) //program board is larger than user board such that borders can be initialized to empty cells
	{
		vector<char> temp_row;
		for(j = 0; j < (tempcolumn+2); j++)
		{
			temp_row.push_back(' ');
		}
		tempboard.push_back(temp_row);
	}
}

Cell::Cell(const Cell& cellin)
{
	int i = 0;
	int j = 0;
	for(i=0; i<(temprow+2); i++)
	{
		for(j=0; j<(tempcolumn+2); j++)
		{
			tempboard[i][j] = cellin.tempboard[i][j];
		}
	}
}

//takes input board and outputs result to temporary board
void Cell::set_state(vector< vector<char> > inputboard)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	
	int neighbour = 0;
	
	for (i=1; i<=temprow; i++)
	{
		for (j=1; j<=tempcolumn; j++)
		{
			//iterates through only user board (i.e. not through full system board)
			if(inputboard[i][j] == 'O')	//if cell is alive
			{
				neighbour = -1;	//start at -1 to ignore cell on current slot
				for (k=(i-1); k<=(i+1); k++)
				{
					for (l=(j-1); l<=(j+1); l++)
					{
						//iterates through every neighbouring slot
						if(inputboard[k][l] == 'O')
						{
							neighbour++; //counts # of neighbours
						}
					}
				}
				if((neighbour <= 1) || (neighbour >= 4))
				{
					tempboard[i][j] = ' ';	//cells dies
				}
				else
				{
					tempboard[i][j] = 'O';	//cell stays alive
				}
			}
			else if(inputboard[i][j] == ' ') //if cell is dead
			{
				neighbour = 0; //in this case we don't need to ignore cell on current slot
				for (k=(i-1); k<=(i+1); k++)
				{
					for (l=(j-1); l<=(j+1); l++)
					{
						//iterates through every neighbouring slot
						if(inputboard[k][l] == 'O')
						{
							neighbour++; //counts # of neighbours
						}
					}
				}
				if(neighbour == 3)
				{
					tempboard[i][j] = 'O';	//cell rebirth
				}
				else
				{
					tempboard[i][j] = ' ';	//cell stays dead
				}
			}
		}
	}
}


//----------------------------------------------------------------------
// class Cancer
//----------------------------------------------------------------------

class Cancer : public Cell	//class cancer, derived from class cell (has access to public elements of cell)
{
	public:
		Cancer();
		Cancer(int temprowin, int tempcolumnin);
		Cancer(const Cancer& cancerin);
		~Cancer();
		vector< vector<char> > get_temp_board() {return tempboard;};
		void set_state(vector< vector<char> > inputboard); //function relative to cancer cell behaviour
};

//when an object of class Cancer is declared, the appropriate elements of class Cell are constructed
Cancer::Cancer():Cell() {}
Cancer::Cancer(int temprowin, int tempcolumnin):Cell(temprowin, tempcolumnin) {}
Cancer::~Cancer() {cout << "Cancer destructor was called successfully." << endl;}

Cancer::Cancer(const Cancer& cancerin)
{
	int i = 0;
	int j = 0;
	for(i=0; i<(temprow+2); i++)
	{
		for(j=0; j<(tempcolumn+2); j++)
		{
			tempboard[i][j] = cancerin.tempboard[i][j];
		}
	}
}

void Cancer::set_state(vector< vector<char> > inputboard)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	
	int neighbour = 0;
	
	for (i=1; i<=temprow; i++)
	{
		for (j=1; j<=tempcolumn; j++)
		{
			//iterates through only user board (i.e. not through full system board)
			if(inputboard[i][j] == 'X')	//if cell is alive
			{
				neighbour = -1;	//start at -1 to ignore cell on current slot
				for (k=(i-1); k<=(i+1); k++)
				{
					for (l=(j-1); l<=(j+1); l++)
					{
						//iterates through every neighbouring slot
						if(inputboard[k][l] == 'X')
						{
							neighbour++; //counts # of neighbours
						}
					}
				}
				if((neighbour <= 1) || (neighbour >= 5)) //condition is different here
				{
					tempboard[i][j] = ' ';	//cells dies
				}
				else
				{
					tempboard[i][j] = 'X';	//cell stays alive
				}
			}
			else if(inputboard[i][j] == ' ') //if cell is dead
			{
				neighbour = 0; //in this case we don't need to ignore cell on current slot
				for (k=(i-1); k<=(i+1); k++)
				{
					for (l=(j-1); l<=(j+1); l++)
					{
						//iterates through every neighbouring slot
						if(inputboard[k][l] == 'X')
						{
							neighbour++; //counts # of neighbours
						}
					}
				}
				if(neighbour == 3)
				{
					tempboard[i][j] = 'X';	//cell rebirth
				}
				else
				{
					tempboard[i][j] = ' ';	//cell stays dead
				}
			}
		}
	}
}


//----------------------------------------------------------------------
// class Board
//----------------------------------------------------------------------

class Board	//class board containing true board and functions to initialize, update and display board
{
	private:
		int menuoption;	//user selection between normal (1) and cancer (2) cells
		int time;		//time value at any instant
		int row;		//# rows in final board
		int column;		//# columns in final board
		vector< vector<char> > board;	//final board
		
	public:
		Board(); 						//default constructor
		Board(int rowin, int columnin);	//constructor used
		Board(const Board& boardin);	//copy function
		~Board() {cout << "Board destructor was called successfully." << endl;};	//destructor
		
		//access functions
		int get_time() {return time;};
		int get_row() {return row;};
		int get_column() {return column;};
		vector< vector<char> > get_board() {return board;};
		
		//functions to manipulate board
		void seed_cells(int selection, double confluence);	//initializes board randomly using confluence value and initializes time
		void display();										//displays board
		int get_num_cells();								//gets number of cells alive on board
		void next_state(); 									//updates board
		void set_board(vector< vector<char> > inputboard);	//sets state of input board to state of board
};

Board::Board()
{
	vector<char> temp_row;
	temp_row.push_back(' ');
	board.push_back(temp_row);
}

Board::Board(int rowin, int columnin)
{
	int i = 0;
	int j = 0;
	row = rowin;
	column = columnin;
	for(i = 0; i < (row+2); i++) //program board is larger than user board such that borders can be initialized to empty cells
	{
		vector<char> temp_row;
		for(j = 0; j < (column+2); j++)
		{
			temp_row.push_back(' ');
		}
		board.push_back(temp_row);
	}
}

Board::Board(const Board& boardin)
{
	int i = 0;
	int j = 0;
	for(i=0; i<(row+2); i++)
	{
		for(j=0; j<(column+2); j++)
		{
			board[i][j] = boardin.board[i][j];
		}
	}
}

void Board::display()
{
	int i = 0, j = 0;
	for (i=1; i<=row; i++)
	{
		for (j=1; j<=column; j++)
		{
			cout << board[i][j];
		}
		cout << endl;
	}
}

void Board::seed_cells(int selection, double confluence)
{
	int n = 0; 		//used to iterate until total number of cells needed is reached
	int x = 0; 		//x coordinate of slot in board
	int y = 0; 		//y coordinate of slot in board
	double total;	//total number of slots that must be cells
	
	total = ((confluence/100.0)*(row*column));
	time = 0; 				//we initialize time here
	menuoption = selection;	//stores user menu selection in menuoption
	
	srand(1);	//sets a pattern for pseudo-random numbers to be generated
	
	for(n = 0; n < total; n++)
	{
		x = (rand() % row) + 1;		//we want a value between 1 and row (inclusive)
		y = (rand() % column) + 1;	//we want a value between 1 and column (inclusive)
		if (selection == 1) //if 1, fill with normal cells
		{
			if(board[x][y] == ' ')
			{
				board[x][y] = 'O';
			}
			else
			{
				n--;
			}
		}
		else if (selection == 2) //if 2, fill with cancer cells
		{
			if(board[x][y] == ' ')
			{
				board[x][y] = 'X';
			}
			else
			{
				n--;
			}
		}
	}
}

int Board::get_num_cells()
{
	int cells = 0;
	int i = 0;
	int j = 0;
	
	for (i=1; i<=row; i++)
	{
		for (j=1; j<=column; j++)
		{
			if(board[i][j] != ' ')	//function works for both normal and cancer cells this way
			{
				cells++;
			}
		}
	}
	return cells;
}

void Board::next_state()
{
	vector< vector<char> > buffer_board;
	Cell cell(row, column);
	if(menuoption == 1)
	{
		cell.set_state(board);					//applies normal cell behaviour to board, puts next state in tempboard
		buffer_board = cell.get_temp_board();	//passes tempboard to buffer_board
		set_board(buffer_board);				//overwrites board with buffer_board
	}
	else if(menuoption == 2)
	{
		Cancer cancer(row, column);					
		cancer.set_state(board);				//applies cancer cell behaviour to board, puts next state in tempboard
		buffer_board = cancer.get_temp_board();	//passes tempboard to buffer_board
		set_board(buffer_board);				//overwrites board with buffer_board
	}
	time++;	//increments time
}

void Board::set_board(vector< vector<char> > inputboard)
{
	int i = 0;
	int j = 0;
	for(i=0; i<(row+2); i++)
	{
		for(j=0; j<(column+2); j++)
		{
			board[i][j] = inputboard[i][j];
		}
	}
}


//----------------------------------------------------------------------
// int main()
//----------------------------------------------------------------------

int main()
{
	int startchoice = 0;
	double confluence = 0;
	int time = 0;
	char c;
	Board board(20, 75);
	
	cout << "\nWelcome to the cell simulator\n" << endl;
	cout << "Select your cell type:\n" << "1) Normal cells or\n" << "2) Cancer cells\n" << "3) Quit\n" << endl;
	
	startchoice = askchoice(1, 3);
	switch(startchoice)
	{
		case 1:
		cout << "You have selected normal cells simulation.\n" << endl;
		break;
		
		case 2:
		cout << "You have selected cancer cells simulation.\n" << endl;
		break;
		
		case 3:
		cout << "Exiting the program...\n" << endl;
		return 0;
		break;
	}

	cout << "Select the confluence percentage (%)" << endl;
	confluence = askchoice(1,100);
	cout << endl;
	
	board.seed_cells(startchoice, confluence);
	system("cls");
	
	while(c!='q')
	{
		system("cls");
		cout << "time: " << board.get_time() << endl;
		cout << "number of cells: " << board.get_num_cells() << endl;
		board.display();
		cout << "Press [enter] to continue, or [q] and [enter] to quit" << endl;
		c = cin.get();
		board.next_state();
	}
	
	return 0;
}

int askchoice(int minvalue, int numberofoptions)
{
	int choice;
	int output;
	bool test1 = false;
	bool test2 = false;
	
	do
	{
		test1 = false;
		test2 = false;
		cout << "Please enter a value between " << minvalue << " and " << numberofoptions << ": ";
		cin >> choice;
		test1 = cin.fail();
		if((choice < minvalue) || (choice>numberofoptions))
		{
			test2 = true;
		}
		else
		{
			output = choice;
		}
		cin.clear();
		cin.ignore(1000,'\n');
	} while((test1 == true) || (test2 == true));

	return output;
}