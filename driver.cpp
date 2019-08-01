#include <iostream>
#include <vector>
#include <ncurses.h>
#include <map>
#define SIZE 3
float version = 1.0;


using namespace std;


void gameMenu();

class Game {
	public:
		vector<vector<int>> board;
		int row[SIZE] = {0, 0, 0};
		int col[SIZE] = {0, 0, 0};
		int diag = 0;
		int revDiag = 0;
		map<int, bool> positions;
		Game() {
			system("clear");
			this->createBoard();
			this->displayBoard();
		}
		char getSymbol(int c) {
			switch(c) {
				case 0: return '-'; break;
				case 1: return 'X'; break;
				case -1: return 'O'; break;
			}
			return '-';
		}

		void createBoard() {
			this->board = {{0, 0, 0},
				       {0, 0, 0},
				       {0, 0, 0}};
			for(int i = 0; i < 9; i++)
				this->positions[i] = false;
		}

		void displayBoard() {
			for(auto i : this->board) {
				for(int j = 0; j < SIZE; j++) {
					cout << getSymbol(i[j]) << " ";
				}
				cout << "\n";
			}
		}


		void moveInfo() {
			int k = 0;
			cout << "Information for moves position - \n";
			for (int i = 0; i < SIZE; i++) {
				for(int j = 0 ; j < SIZE; j++)
					cout << k++ << " ";
				cout << "\n";
			}
		}

		void makeMove(int x, int p) {
			vector<int> move = {1, -1};
			
			int i = x/3;
			int j = x%3;

			this->positions[x] = true;
			this->board[i][j] = move[p%2];
		       	this->row[i] += move[p%2];
		       	this->col[j] += move[p%2];
			if(i == j)
				this->diag += move[p%2];
			if(i == (2 - j))
				this->revDiag += move[p%2];

		}

		/*bool checkWinner(int x, int p) {
			bool winRow = true, winCol = true, winDiag = true, winRevDiag = true;
			vector<int> move = {1, -1};
			for (int i = 0; i < SIZE; i++) {
				if(this->board[x/3][i] != move[p%2])
					winRow = false;
				if(this->board[i][x%3] != move[p%2])
					winCol = false;
				if((this->board[i][i] != move[p%2]) && x/3 == x%3)
					winDiag = false;
				if((this->board[x/3][i] != move[p%2]) && x/3 == (2 - (x%3)))
					winRevDiag = false;
			}
			if(winRow || winCol || winDiag || winRevDiag)
				return true;
			return false;
		}*/

		bool checkWinner(int x) {
			int i = x/3;
			int j = x%3;
			if((abs(this->row[i]) == SIZE) || (abs(this->col[j]) == SIZE) || ((i==j) && (abs(this->diag) == SIZE)) || ((i==(2-j)) && (abs(this->revDiag) == SIZE)))
				return true;
			return false;
		}

		void greetings(int p) {
			system("clear");
			cout << "Current State of the Board - \n";
			this->displayBoard();
			cout << "\t\t -- Congratulations, Player " << p%2 + 1 << "! You are the winner!--\n";
			cout << "\nPress any key to return to Main Menu..";
			cin.get();
			cin.get();
			gameMenu();
		}

		void gameScreen(int x) {
			
			string symbols = "XO";
			int pos, i;
			for(i = 0; i < 9; i++) {
				system("clear");
				cout << "Player " << i%2 + 1 << "'s Turn :: Symbol = " << symbols[i%2] << "\n";
				this->moveInfo();
				cout << "Current State of the Board - \n";
				this->displayBoard();
				cout << "\n\n\tEnter your move:";
				try{
					cin >> pos; 

					if(this->positions[pos] || (pos > 8) || (pos < 0))
						throw pos;
				
					makeMove(pos, i);
				
					if(i > 3 && this->checkWinner(pos)) {
						this->greetings(i);
						break;
					}
				}
				catch(int pos) {
					cout << "\nPosition already occupied or outside the grid, please try another position..\n";
					cin.get();
					cin.get();
					i--;
				}
			}
			if (i == 9) {
				system("clear");
				cout << "Current State of the Board - \n";
				this->displayBoard();
				cout << "\t\t --It's a tie!--\n";
				cout << "\nPress any key to return to Main Menu..";
				cin.get();
				cin.get();
				gameMenu();
			}
				
		}

};

/*class AI : public Game {
	public: 
		void undoMove(int x, int p) {
			vector<int> move = {1, -1};
			
			int i = x/3;
			int j = x%3;

			this->positions[x] = false;
			this->board[i][j] = 0;
		       	this->row[i] -= move[p%2];
		       	this->col[j] -= move[p%2];
			if(i == j)
				this->diag -= move[p%2];
			if(i == (2 - j))
				this->revDiag -= move[p%2];
			
		}
		
		int minimax(int pos, int player) {
			if(player == 9)
				return 0;
			else if((player%2 == 0) && (player < 9)){	
				vector<int> move = {1, -1};
				int score = 0, best = -1000;
				if(checkWinner(pos))
					return 10;
				if(player == 8)
					return 0;
				//cout << "DEBUG";
				for(int i = 0; i < 9; i++) {
					if(!this->positions[i]) {
						this->makeMove(i, player + 1);
						best = max(best,this-> minimax(i, player + 1));
						//this->displayBoard();
						this->undoMove(i, player + 1);
					}
				}
			       return best;	
			}
			else if((player%2 == 1) && (player < 9)){	
				vector<int> move = {1, -1};
				int score = 0, best = 1000;
				if(checkWinner(pos))
					return -10;
						
				//cout << "DEBUG";
				for(int i = 0; i < 9; i++) {
					if(!this->positions[i]) {
						this->makeMove(i, player + 1);
						best = min(best, this->minimax(i, player + 1));
						//this->displayBoard();
						this->undoMove(i, player + 1);
					}
				}
				return best;
			}
		}

		int AIMove(int player) {
			int i, best = -1000, move = 888; 
			for (i = 0; i < 9; i++) {

				//cout << "DEBUG";
				if(!this->positions[i]) {
					this->makeMove(i, player);
					int current = this->minimax(i, player);
				       	this->undoMove(i, player);

					if(current > best) {
						best = current;
						move = i;
					}	
				}
			}
			return move;
		}
		void gameScreen(int x) {
			system("clear");
			this->createBoard();
			this->displayBoard();
			
			string symbols = "XO";
			int pos, i;
			for(i = 0; i < 9; i++) {
				system("clear");
				cout << "Player " << i%2 + 1 << "'s Turn :: Symbol = " << symbols[i%2] << "\n";
				this->moveInfo();
				cout << "Current State of the Board - \n";
				this->displayBoard();
				cout << "\n\n\tEnter your move:";
				cin >> pos;
				if(i%2 == 0)
					makeMove(pos, i);
				else {
					pos = this->AIMove(i);
					cout << "AI Moves at position - " << pos;
					int x;
					cin >> x;
				       	this->makeMove(pos, i);	
				}


				if(i > 3 && this->checkWinner(pos)) {
					this->greetings(i);
					return;
				}
			}
			if (i == 9) {
				system("clear");
				cout << "Current State of the Board - \n";
				this->displayBoard();
				cout << "\t\t --It's a tie!--\n";
				cout << "\nPress any key to exit..";
				getchar();
			}
				
		}
};
*/
void gameMenu() {
	system("clear");
	int choice;
	cout << "\t--TIC TAC TOE version " << version << "--\n";
	//cout << "\t\t1. Human vs Human\n";
	//cout << "\t\t2. Human vs AI\n";
	//cout << "\t\t3. AI vs AI\n";
	//cout << "\t\t4. Exit\n";
	cout << "\t\t1. Play Game\n";
	cout << "\t\t2. Exit\n";
	try { 
		cin >> choice;
		
		if((choice > 2) || (choice < 0))
		       throw choice;	
		
		if(choice == 1) {
			Game g;
			g.gameScreen(choice);
		}
		else{
			cout << "\nThank You for Playing!\n";
			cin.get();
			cin.get();
			return;
		}
	}
	catch(int x) {
		cout << "\n\nPlease enter a valid choice..\n";
		cin.get();
		cin.get();
		gameMenu();
	}
}

int main() {
	gameMenu();
	return 0;
}
