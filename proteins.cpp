#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct protein	//organize data about on protein unit
{
	int index;
	string gi;
	string ref;
	string name;
	string sequence;
};

int askchoice(int numberofoptions);			//asks player to choose between menu options
int checkinput(string userinput, int n);	//checks whether the user input is valid
void dispmenu();							//displays main menu
void disphit();								//displays hit menu
void disptable();							//displays amino acids table
void dispdescription(protein selection);	//displays protein description
void countamino(string testsequence);		//counts number of appearances of each amino acid in protein sequence

protein checkgi(vector<protein> v, string gitest);		//checks input gi against all gis in database and returns matching protein
protein checkref(vector<protein> v, string reftest);	//checks input ref against all refs in database and returns matching protein
vector<protein> checkkeyword(vector<protein> v, string keywordtest);	//checks input keyword against all protein names in database, returns all matching proteins


int main()
{
	int startchoice = 0;		//user selection between databases
	int menuchoice = 0;			//user selection for main menu
	int hitchoice = 0;			//user selection for hit menu
	int searchselection = 0;	//user selection between proteins returned from keyword search
	
	int id = 0;			//index searched by user
	string idgi;		//gi searched by user
	string idref;		//ref searched by user
	string keyword;		//keyword searched by user
	
	int counter = 0;		//used to associate each protein to its index
	int keywordcount = 0;	//used to iterate over all proteins returned from keyword search
	
	string file;	//input file to be loaded
	string output;	//output file to be written to
	
	protein inputprotein;	//protein unit to be loaded
	protein activeprotein;	//protein unit selected by user after search
	
	vector<protein> input;			//vector containing all proteins in database
	vector<protein> keywordmatches;	//vector containing all proteins returned from keyword search
	
	int correct = 2;		//used to check if one of the proteins returned from keyword search is correct
	int testcorrect = 0;	//same
	string inputcorrect;
	
	//delete the content of our output file
	output = "selected_protein.txt";
	ofstream outstream;
	outstream.open(output.c_str(), ios::trunc);
	outstream.close();
	
	cout << "\nWelcome to the Protein Database\n" << endl;
	cout << "1) Load the abridged protein data\n" << "2) Load the complete protein data\n" << "3) Quit database\n" << endl;
	
	startchoice = askchoice(3);
	switch(startchoice)
	{
		case 1:
		file = "protein_a.fa";
		break;
		
		case 2:
		file = "protein_c.fa";
		break;
		
		case 3:
		cout << "Exiting the database\n" << endl;
		return 0;
	}
	
	ifstream database(file.c_str());	//creating an input stream called database for reading the file called file
	
	if(!database)	//checking if something went wrong with creation of input stream
	{
		if (database.bad())
		{cout << "The input file is corrupted and cannot be opened." << endl;}
		if (database.fail())
		{database.clear();}
		else
		{
			cout << "The input file could not be opened [unknown reason].";
		}
	}
	
	cout << "Loading database...\n";
	
	while(!database.eof())	//loads input file into program and organizes it into a vector of structs
	{
	database.ignore(1000, '|');
	getline(database, inputprotein.gi, '|');
	database.ignore(1000, '|');
	getline(database, inputprotein.ref, '|');
	database.ignore(1000, ' ');
	getline(database, inputprotein.name, '\n');
	getline(database, inputprotein.sequence, '>');
	counter++;
	inputprotein.index = counter;
	input.push_back(inputprotein);
	}
	
	database.close();
	cout << "Database loaded." << endl;
	
	do
	{
		dispmenu();
		menuchoice = askchoice(6);
		switch(menuchoice)
		{
			case 1:	// List the total number of proteins in the database and display a table that associates each amino acid character to its proper name.
			cout << "\nThe proteins in the database are from GenBank(R)";
			cout << "\nTotal number of proteins in the database: " << input.size() << endl;
			disptable();
			break;
			
			case 2:	// Search by item id. This id refers to the protein in the order that it appears in the database.
			do
			{
				cout << "Enter an item id: ";
				cin >> id;
			} while((id <= 0) || (id >= input.size()));
			activeprotein = input[id-1];
			break;
			
			case 3: // Search by gi id. This id is the first identification labelled in the description line.
			do
			{
				cout << "Enter a gi id: ";
				cin >> idgi;
				activeprotein = checkgi(input, idgi);
				if(activeprotein.name == "fail")
				{
					cout << "Your search was unsuccessful." << endl;
				}
			} while(activeprotein.name == "fail");
			break;
			
			case 4: // Search by ref id. This id is the second identification labelled in the description line.
			do
			{
				cout << "Enter a ref id: ";
				cin >> idref;
				activeprotein = checkref(input, idref);
				if(activeprotein.name == "fail")
				{
					cout << "Your search was unsuccessful." << endl;
				}
			} while(activeprotein.name == "fail");
			break;
			
			case 5:	// Search by keyword. Search through the series of words within the third identification label of all the proteins within the database for a keyword match. 
			while(correct == 2)
			{
				cout << "Enter a keyword: ";
				cin >> keyword;
				keywordmatches = checkkeyword(input, keyword); 
			
				/*If a match is found, display the protein details to the users and ask whether
				this was the protein he or she was seeking.*/
				
				if(keywordmatches.size() == 0)
				{
					cout << "\nNo matches found." << endl;
					correct = 2;
				}
				
				else if(keywordmatches.size() == 1)
				{
					activeprotein = keywordmatches[0];
					dispdescription(activeprotein);
					cout << endl;
					do
					{
						cout << "Enter 1 if you wish to proceed with this protein, or 2 otherwise: ";
						cin >> inputcorrect;
						testcorrect = checkinput(inputcorrect, 2);
					} while((testcorrect != 1) && (testcorrect != 2));
					correct = testcorrect;
				}
				
				/*If multiple matches are found, display the options and
				allow the user to choose which one to select or none of the above.*/
				
				else
				{
					cout << "\nNumber of matches found: " << keywordmatches.size() << endl;
					for(keywordcount = 1; keywordcount <= keywordmatches.size(); keywordcount++)	//
					{
						activeprotein = keywordmatches[keywordcount - 1];
						cout << keywordcount << ") item id: " << activeprotein.index << ", gi id: " << activeprotein.gi << ", ref if: " << activeprotein.ref << endl;
						cout << activeprotein.name << endl;
					}
					cout << keywordcount << ") NONE OF THE ABOVE" << endl;
					cout << endl;
					searchselection = askchoice(keywordmatches.size() + 1);
					if(searchselection <= keywordmatches.size())
					{
						activeprotein = keywordmatches[searchselection - 1];
						correct = 1;
					}
					else
					{
						cout << "You will be redirected to a new search." << endl;
						correct = 2;
					}
				}
			}
			break;
			
			case 6:
			cout << "Exiting the database\n";
			return 0;
			break;
		}
		
		if(menuchoice != 1)
			{
				do
				{
					disphit();
					hitchoice = askchoice(5);
					switch(hitchoice)
					{
						case 1:
						dispdescription(activeprotein);
						break;
					
						case 2:
						cout << "\nProtein sequence:\n" << activeprotein.sequence;
						break;
				
						case 3:
						countamino(activeprotein.sequence);
						break;
				
						case 4:
						{
							outstream.open(output.c_str(), ios::app);
								if(!outstream)
								{
									cout << "The program could not create output file." << endl;
								}
								else 
								{
									outstream << ">gi|" << activeprotein.gi << "|ref|" << activeprotein.ref << "| " << activeprotein.name << endl;
									outstream << activeprotein.sequence << endl;
									cout << "The selected protein has been saved under the file name selected_protein.txt" << endl;
								}
							outstream.close();
						}
						break;
						
						case 5:
						menuchoice = 1;
						break;
					}
				} while (hitchoice != 5);
			}
	} while (menuchoice == 1); 
	return 0;
}

int checkinput(string userinput, int n)
{
	char buffer = userinput[0];
	int output;
	
if((userinput.size() > 2) || ((int(buffer) < 49) || (int(buffer) > 49+(n-1))))
	{
		output = 0;
	}
	else
	{
		output = buffer - '0';
	}
	return output;
}

int askchoice(int numberofoptions)
{
	string choice;
	int test;
	
	cout << "Please enter a value between 1 and " << numberofoptions << ": ";
	cin >> choice;
	test = checkinput(choice, numberofoptions);
	
	while(test == 0)
	{
		cout << "The input you have entered is invalid.\nPlease enter a value between 1 and " << numberofoptions << ": ";
		cin >> choice;
		test = checkinput(choice, numberofoptions);
	}
	return test;
}

void dispmenu()
{
	cout << endl << "\tMAIN MENU" << endl;
	cout << "\t1. Overview of the database" << endl;
	cout << "\t2. Search by protein #" << endl;
	cout << "\t3. Search by gi #" << endl;
	cout << "\t4. Search by ref #" << endl;
	cout << "\t5. Search by keyword" << endl;
	cout << "\t6. Quit database" << endl;
	cout << endl;
}

void disphit()
{
	cout << endl << "\tHIT MENU" << endl;
	cout << "\t1. Description of the protein" << endl;	//Display the gi id, the ref id, and the name of the protein.
	cout << "\t2. Protein sequence" << endl;			//Display the full sequence.
	cout << "\t3. Protein statistics" << endl;			//Display the number of amino acids in the sequence for each amino acid.
	cout << "\t4. Record protein to file" << endl;		//Record the selected protein to a file called ‘selected_proteins.txt’.
	cout << "\t5. Return to main menu" << endl;
	cout << endl;
}

void disptable()
{
	cout << "_____________________________________________________________________________" << endl;
	cout << " Letter" << "\t\t" << "Amino Acid\t\t" << "Letter" << "\t" << "Amino Acid" << endl;
	cout << "-----------------------------------------------------------------------------" << endl;
	cout << " A\t\t" << "Alanine" << "\t\t\t" << "P\t" << "Proline" << endl;
	cout << " B\t\t" << "Aspartate/Asparagine" << "\t" << "Q\t" << "Glutamine" << endl;
	cout << " C\t\t" << "Cystine" << "\t\t\t" << "R\t" << "Arginine" << endl;
	cout << " D\t\t" << "Aspartate" << "\t\t" << "S\t" << "Serine" << endl;
	cout << " E\t\t" << "Glutamate" << "\t\t" << "T\t" << "Threonine" << endl;
	cout << " F\t\t" << "Phenylalanine" << "\t\t" << "U\t" << "Selenocysteine" << endl;
	cout << " G\t\t" << "Glycine" << "\t\t\t" << "V\t" << "Valine" << endl;
	cout << " H\t\t" << "Histidine" << "\t\t" << "W\t" << "Tryptophan" << endl;
	cout << " I\t\t" << "Isoleucine" << "\t\t" << "Y\t" << "Tyrosine" << endl;
	cout << " K\t\t" << "Lysine" << "\t\t\t" << "Z\t" << "Glutamate/Glutamine" << endl;
	cout << " L\t\t" << "Leucine" << "\t\t\t" << "X\t" << "any" << endl;
	cout << " M\t\t" << "Methionine" << "\t\t" << "*\t" << "translation stop" << endl;
	cout << " N\t\t" << "Asparagine" << "\t\t" << "-\t" << "gap of indeterminate length" << endl;
	cout << "-----------------------------------------------------------------------------" << endl;
}

void dispdescription(protein selection)
{
	cout << endl;
	cout << "Description of the protein:\n" << endl;
	cout << "item id:" << selection.index << endl;
	cout << "gi id:\t" << selection.gi << endl;
	cout << "ref id:\t" << selection.ref << endl;
	cout << "name:\t" << selection.name << endl;
}

void countamino(string testsequence)
{
	int i = 0;
	int A = 0;
	int B = 0;
	int C = 0;
	int D = 0;
	int E = 0;
	int F = 0;
	int G = 0;
	int H = 0;
	int I = 0;
	int K = 0;
	int L = 0;
	int M = 0;
	int N = 0;
	int P = 0;
	int Q = 0;
	int R = 0;
	int S = 0;
	int T = 0;
	int U = 0;
	int V = 0;
	int W = 0;
	int X = 0;
	int Y = 0;
	int Z = 0;
	int star = 0;
	int line = 0;
	int count= 0;
	
	
	for(i = 0; i <= testsequence.size(); i++)
	{
		switch(testsequence[i])
		{
			case 'A':
			A++;
			count++;
			break;
			
			case 'B':
			B++;
			count++;
			break;
			
			case 'C':
			C++;
			count++;
			break;
			
			case 'D':
			D++;
			count++;
			break;
			
			case 'E':
			E++;
			count++;
			break;
			
			case 'F':
			F++;
			count++;
			break;
			
			case 'G':
			G++;
			count++;
			break;
			
			case 'H':
			H++;
			count++;
			break;
			
			case 'I':
			I++;
			count++;
			break;
			
			case 'K':
			K++;
			count++;
			break;
			
			case 'L':
			L++;
			count++;
			break;
			
			case 'M':
			M++;
			count++;
			break;
			
			case 'N':
			N++;
			count++;
			break;
			
			case 'P':
			P++;
			count++;
			break;
			
			case 'Q':
			Q++;
			count++;
			break;
			
			case 'R':
			R++;
			count++;
			break;
			
			case 'S':
			S++;
			count++;
			break;
			
			case 'T':
			T++;
			count++;
			break;
			
			case 'U':
			U++;
			count++;
			break;
			
			case 'V':
			V++;
			count++;
			break;
			
			case 'W':
			W++;
			count++;
			break;
			
			case 'X':
			X++;
			count++;
			break;
			
			case 'Y':
			Y++;
			count++;
			break;
			
			case 'Z':
			Z++;
			count++;
			break;
			
			case '*':
			star++;
			count++;
			break;
			
			case '-':
			line++;
			count++;
			break;			
		}
	}
	cout << "\nReport on the protein statistics:\n" << endl;
	cout << "Total number of amino acids: " << count << endl;
	cout << endl;
	cout << "A\t" << A << "\t" << "P\t" << P << endl;
	cout << "B\t" << B << "\t" << "Q\t" << Q << endl;
	cout << "C\t" << C << "\t" << "R\t" << R << endl;
	cout << "D\t" << D << "\t" << "S\t" << S << endl;
	cout << "E\t" << E << "\t" << "T\t" << T << endl;
	cout << "F\t" << F << "\t" << "U\t" << U << endl;
	cout << "G\t" << G << "\t" << "V\t" << V << endl;
	cout << "H\t" << H << "\t" << "W\t" << W << endl;
	cout << "I\t" << I << "\t" << "X\t" << X << endl;
	cout << "K\t" << K << "\t" << "Y\t" << Y << endl;
	cout << "L\t" << L << "\t" << "Z\t" << Z << endl;
	cout << "M\t" << M << "\t" << "*\t" << star << endl;
	cout << "N\t" << N << "\t" << "-\t" << line << endl;
}

protein checkgi(vector<protein> v, string gitest)
{
	int i = 0;
	protein protactiv;
	for(i = 0; i < v.size(); i++)
	{
		protactiv = v[i];
		if(protactiv.gi == gitest)
		{
			return protactiv;
		}
	}
	protactiv.name = "fail";
	return protactiv;
}

protein checkref(vector<protein> v, string reftest)
{
	int i = 0;
	protein protactiv;
	for(i = 0; i < v.size(); i++)
	{
		protactiv = v[i];
		if(protactiv.ref == reftest)
		{
			return protactiv;
		}
	}
	protactiv.name = "fail";
	return protactiv;
}

vector<protein> checkkeyword(vector<protein> v, string keywordtest)
{
	int i = 0;
	int j = 0;
	int n = 0;
	int test = 0;
	
	protein protactiv;
	string nameactiv;
	
	vector<protein> match;
	
	for(i = 0; i < v.size(); i++)
	{
		protactiv = v[i];
		nameactiv = protactiv.name;
		for(j = 0; j <= (nameactiv.size() - keywordtest.size()); j++)
		{
			if(nameactiv[j] == keywordtest[0])
			{
				for(test = 1; (nameactiv[j+test] == keywordtest[test]) && (test <= keywordtest.size()); test++);
				if(test == keywordtest.size())
				{
					match.push_back(protactiv);
				}
			}
		}
	}
	return match;
}