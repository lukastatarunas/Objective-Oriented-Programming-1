#include <iostream>
#include <string>
#include <time.h>

using namespace std;

const int boardSize = 10; // A constant holding size of boards.
char playerBoard[boardSize][boardSize]; // Holds player's ship locations.
char playerHits[boardSize][boardSize]; // Holds player's hit locations.
char enemyBoard[boardSize][boardSize]; // Holds enemy's ship locations.
char enemyHits[boardSize][boardSize]; // Holds enemy's hit locations.
string player1Name; // Holds player 1 name.
string player2Name; // Holds player 2 name.
int currentPlayer = 1;
int gameMode; // Holds data on if the player is playing with another player or with a computer.
int player1ShipHealth[5] = { 2,3,3,4,5 }; // Holds the ship health for the submarine, cruiser, destroyer, battleship and air craft carrier for player 1.
int player2ShipHealth[5] = { 2,3,3,4,5 }; // Holds the ship health for the submarine, cruiser, destroyer, battleship and air craft carrier for player 2.
bool gameEnd = false; // Game runs as long as the game is not won.

class Game
{	
	public:
		void clearBoard(char clearBoard[10][10]); // Clears the board.
		void drawBoard(char drawBoard[10][10]); // Draws the board and places a given array inside of it.
		void placeShips(); // Places the ships.
		void playerShooting(char board[10][10], char hitBoard[10][10], char damageBoard[10][10]); // Allows the players to shoot at each others ships.
		void playGame(); // Overall game loop.
		void shipDestroyed(char hitsChar); // If a ship is hit this function counts which ship was hit and if it has been destroyed.
		void setShipType(int &currentShip, int &shipBeingPlaced, string &shipName, char &shipChar); // Changes the ship type while placing them on the board.
		int switchCurrentPlayer(); // Switches the current player to the other player.
		int mainMenu(); // Draws the main menu user interface.
};

int main()
{
	int choice = 0; // Clears choice.
	Game object;
	choice = object.mainMenu(); // Receives result from the mainMenu function.
	if (choice == 3) // If player selected exit...
	{
		return 0; // End program.
	}
	else // Otherwise clear both boards.
	{
		object.clearBoard(playerBoard);
		object.clearBoard(enemyBoard);
	}
	if (choice == 1)  // Player vs Computer.
	{
		player2Name = "Computer";
		gameMode = 1; // Single Player.
		object.drawBoard(playerBoard);
		object.placeShips(); // Players place ships.
		object.playGame();
		return 0; // Game has finished.
	}
	if (choice == 2) // Player vs Player.
	{
		gameMode = 2; // Multiplayer.
		system("CLS");
		cout << player1Name << " will place his ships first!" << endl;
		system("PAUSE");
		system("CLS");
		object.drawBoard(playerBoard); // Draws both boards.
		object.placeShips(); // Players place ships.
		object.playGame();
		return 0; // Game has finished.
	}
	system("PAUSE");
}

void Game::clearBoard(char clearBoard[10][10])
{
	for (int i = 0; i < boardSize; i++) // Goes through all values...
	{
		for (int j = 0; j < boardSize; j++)
		{
			clearBoard[i][j] = ' '; // And resets them to empty.
		}
	}
}

void Game::drawBoard(char drawBoard[10][10])
{
	cout << "  "; // Initial spacing.
	for (int i = 0; i < boardSize; i++)
	{
		cout << " " << i + 1 << "  "; // Top numbers of the board (1-10).
	}
	cout << endl;
	cout << " ";
	for (int i = 0; i < boardSize; i++)
	{
		cout << "+---"; // Top line.
	}
	cout << "+" << endl; // Final plus.
	for (int i = 0; i < boardSize; i++)
	{
		cout << char(i + 'A'); // Displays side letters.
		for (int j = 0; j < boardSize; j++)
		{
			cout << "| " << drawBoard[i][j] << " "; // Places an array inside the board.
		}
		cout << "|" << endl;
		cout << " ";
		for (int i = 0; i < boardSize; i++)
		{
			cout << "+---"; // Bottom lines.
		}
		cout << "+" << endl; // Final plus.
	}
	cout << endl;
}

void Game::placeShips()
{
	string shipName = "Submarine"; // Holds the name for current ship.
	char boardChar; // Character value for the board.
	char shipChar = ' '; // Holds the character which represents the ship in the board.
	int currentShip = 1; // Holds the current selected ship and changes so that player could place all ships.
	int shipBeingPlaced = 1; // Stores the current ship being placed and increments by 1 when a ship is placed.
	int check = 0; // Checks if all board spaces are empty before placing a ship.
	int randomDirection; // Stores random direction for the computer.
	int x; // X board value.
	int y; // Y board value.
	int whichDirection; // Holds the direction where the player wants to place a ship.
	int direction = 1; // Allows the user to pick ship rotation.
	bool placingShips = true;
	while (placingShips) // While the player is placing ships...
	{
		{
			if (currentPlayer == 1) // Lets player 1 cycle through the ships.
			{
				setShipType(currentShip, shipBeingPlaced, shipName, shipChar);
			}
			if (currentPlayer == 2)
			{
				if (gameMode == 1) // Playing with computer.
				{
					srand(time(NULL)); // Initialise random.
					for (currentShip = 1; currentShip < 6; currentShip++) // For all ships.
					{
						x = rand() % 10 + 1; // Random number between 1-10.
						y = rand() % 10 + 1; // Random number between 1-10.
						randomDirection = rand() % 4 + 1;  // Random number between 1-4.
						if (enemyBoard[y - 1][x - 1] == ' ') // If not already picked.
						{
							setShipType(currentShip, shipBeingPlaced, shipName, shipChar);
							enemyBoard[y - 1][x - 1] = shipChar; // Mark location with a current character to represent a ship.
							check = 1; // Holds the value that is used to check that the required board space for a ship is clear.
							switch (randomDirection)
							{
							case 1: // Places a ship upwards.
								for (int j = 0; j < shipBeingPlaced; j++) // For the length of the ship...
								{
									if (y - 1 >= 0 && y - 1 <= boardSize && x - 1 >= 0 && x - 1 < boardSize && enemyBoard[y - 1][x - 1] == ' ') // If a ship is within the board and not intercepting with another ship.
									{
										check++; // Increments blank check...
									}
									y -= 1; // Checks next space.
								}
								if (check == shipBeingPlaced) // If all the spaces a ship needs are valid...
								{
									y += shipBeingPlaced; // Return y to starting value.
									for (int j = 0; j < shipBeingPlaced; j++) // For the length of the current ship...
									{
										enemyBoard[y - 1][x - 1] = shipChar; // Places a ship character in the board.
										y -= 1;
									}
								}
								else // If invalid location selected...
								{
									y += shipBeingPlaced; // Returns y to the starting place.
									enemyBoard[y - 1][x - 1] = ' '; // Clears it.
									currentShip--; // Returns a ship to the last ship.
								}
								break;
							case 2: // Places a ship downwards.
								for (int j = 0; j < shipBeingPlaced; j++)
								{
									if (y - 1 >= 0 && y - 1 <= boardSize && x - 1 >= 0 && x - 1 < boardSize && enemyBoard[y - 1][x - 1] == ' ')
									{
										check++;
									}
									y += 1;
								}
								if (check == shipBeingPlaced)
								{
									y -= shipBeingPlaced;
									for (int j = 0; j < shipBeingPlaced; j++)
									{
										enemyBoard[y - 1][x - 1] = shipChar;
										y += 1;
									}
								}
								else
								{
									y -= shipBeingPlaced;
									enemyBoard[y - 1][x - 1] = ' ';
									currentShip--;
								}
								break;
							case 3: // Places a ship facing towards the right.
								for (int j = 0; j < shipBeingPlaced; j++)
								{
									if (y - 1 >= 0 && y - 1 <= boardSize && x - 1 >= 0 && x - 1 < boardSize && enemyBoard[y - 1][x - 1] == ' ')
									{
										check++;
									}
									x += 1;
								}
								if (check == shipBeingPlaced)
								{
									x -= shipBeingPlaced;
									for (int j = 0; j < shipBeingPlaced; j++)
									{
										enemyBoard[y - 1][x - 1] = shipChar;
										x += 1;
									}
								}
								else
								{
									x -= shipBeingPlaced;
									enemyBoard[y - 1][x - 1] = ' ';
									currentShip--;
								}
								break;
							case 4: // Places a ship facing towards the left.
								for (int j = 0; j < shipBeingPlaced; j++)
								{
									if (y - 1 >= 0 && y - 1 <= boardSize && x - 1 >= 0 && x - 1 < boardSize && enemyBoard[y - 1][x - 1] == ' ')
									{
										check++;
									}
									x -= 1;
								}
								if (check == shipBeingPlaced)
								{
									x += shipBeingPlaced;
									for (int j = 0; j < shipBeingPlaced; j++)
									{
										enemyBoard[y - 1][x - 1] = shipChar;
										x -= 1;
									}
								}
								else
								{
									x += shipBeingPlaced;
									enemyBoard[y - 1][x - 1] = ' ';
									currentShip--;
								}
								break;
							default:
								break;
							}
						}
						else
						{
							currentShip -= 1; // If a ship location is invalid. Try that ship again.
						}
					}
				}
			}
			setShipType(currentShip, shipBeingPlaced, shipName, shipChar);
			if (currentShip > 5)
			{
				return; // Returns when all 5 ships have been placed.
			}
			// Takes input.
			cout << "Please select the X and Y coordinates for your " << shipName << ": " << endl;
			cout << "X: ";
			cin >> x;
			if (x > 10 || x < 1)
			{
				cout << "That position is invalid. Please try again!" << endl;
			}
			cout << "Y: ";
			cin >> boardChar; // Enters character Y value.
			y = toupper(boardChar); // Converts it to the uppercase letter value.
			y -= 64;
			if (y > 10 || y < 1)
			{
				cout << "That position is invalid. Please try again!" << endl;
			}
			else // If positions are valid...
			{
				if (currentPlayer == 1)
				{
					if (playerBoard[y - 1][x - 1] == ' ') // And board location is empty.
					{
						system("CLS");
						playerBoard[y - 1][x - 1] = shipChar; // Places character to represent a ship.
						drawBoard(playerBoard);
						cout << "Which direction would you like it to face?" << endl;
						cout << "1.) UP" << endl << "2.) DOWN" << endl << "3.) RIGHT" << endl << "4.) LEFT" << endl;
						cin >> whichDirection; // Asks for direction to be entered.
						check = 1;
						switch (whichDirection)
						{
						case 1: // Places a ship upwards.
							for (int j = 0; j < shipBeingPlaced; j++) // For the length of the ship...
							{
								if (y - 1 >= 0 && y - 1 <= boardSize && x - 1 >= 0 && x - 1 <= boardSize && playerBoard[y - 1][x - 1] == ' ') // If a ship is within the board and not intercepting with another ship.
								{
									check++; // Increments blank check...
								}
								y -= 1; // Checks next space.
							}
							if (check == shipBeingPlaced) // If all the spaces that a ship needs are valid...
							{
								y += shipBeingPlaced; // Returns y to the starting value.
								for (int j = 0; j < shipBeingPlaced; j++) // For the length of the current ship...
								{
									playerBoard[y - 1][x - 1] = shipChar; // Places a ship character in the board.
									y -= 1;
								}
							}
							else // If invalid location is selected...
							{
								system("CLS");
								cout << "Invalid Location!" << endl;
								y += shipBeingPlaced; // Returns y to the starting place.
								playerBoard[y - 1][x - 1] = ' '; // Clears it.
								drawBoard(playerBoard);
								currentShip--; // Returns a ship to the last ship.
								system("PAUSE");
							}
							break;
						case 2: // Places a ship downwards.
							for (int j = 0; j < shipBeingPlaced; j++)
							{
								if (y - 1 >= 0 && y - 1 <= boardSize && x - 1 >= 0 && x - 1 <= boardSize && playerBoard[y - 1][x - 1] == ' ')
								{
									check++;
								}
								y += 1;
							}
							if (check == shipBeingPlaced)
							{
								y -= shipBeingPlaced;
								for (int j = 0; j < shipBeingPlaced; j++)
								{

									playerBoard[y - 1][x - 1] = shipChar;
									y += 1;
								}
							}
							else
							{
								system("CLS");
								cout << "Invalid Location!" << endl;
								y -= shipBeingPlaced;
								playerBoard[y - 1][x - 1] = ' ';
								drawBoard(playerBoard);
								currentShip--;
								system("PAUSE");
							}
							break;
						case 3: // Places a ship facing towards the right.
							for (int j = 0; j < shipBeingPlaced; j++)
							{
								if (y - 1 >= 0 && y - 1 <= boardSize && x - 1 >= 0 && x - 1 < boardSize && playerBoard[y - 1][x - 1] == ' ')
								{
									check++;
								}
								x += 1;
							}
							if (check == shipBeingPlaced)
							{
								x -= shipBeingPlaced;
								for (int j = 0; j < shipBeingPlaced; j++)
								{
									playerBoard[y - 1][x - 1] = shipChar;
									x += 1;
								}
							}
							else
							{
								system("CLS");
								cout << "Invalid Location!" << endl;
								x -= shipBeingPlaced;
								playerBoard[y - 1][x - 1] = ' ';
								drawBoard(playerBoard);
								currentShip--;
								system("PAUSE");
							}
							break;
						case 4: // Places a ship facing towards the left.
							for (int j = 0; j < shipBeingPlaced; j++)
							{
								if (y - 1 >= 0 && y - 1 <= boardSize && x - 1 >= 0 && x - 1 <= boardSize && playerBoard[y - 1][x - 1] == ' ')
								{
									check++;
								}
								x -= 1;
							}
							if (check == shipBeingPlaced)
							{
								x += shipBeingPlaced;
								for (int j = 0; j < shipBeingPlaced; j++)
								{
									playerBoard[y - 1][x - 1] = shipChar;
									x -= 1;
								}
							}
							else
							{
								system("CLS");
								cout << "Invalid Location!" << endl;
								x += shipBeingPlaced;
								playerBoard[y - 1][x - 1] = ' ';
								drawBoard(playerBoard);
								currentShip--;
								system("PAUSE");
							}
							break;
						default:
							break;
						}
						system("CLS");
						drawBoard(playerBoard);
						currentShip++; // Increments the value so that the next ship could be placed.
					}
				}
				if (currentPlayer == 2 && gameMode == 2) // If player 2 is another player and not a computer.
				{
					if (enemyBoard[y - 1][x - 1] == ' ')
					{
						system("CLS");
						enemyBoard[y - 1][x - 1] = shipChar; // Places a character to represent a ship.
						drawBoard(enemyBoard);
						cout << "Which direction would you like it to face?" << endl;
						cout << "1.) UP" << endl << "2.) DOWN" << endl << "3.) RIGHT" << endl << "4.) LEFT" << endl;
						cin >> whichDirection;
						check = 1;
						switch (whichDirection)
						{
						case 1: // Places a ship upwards.
							for (int j = 0; j < shipBeingPlaced; j++) // For the length of the ship...
							{
								if (y - 1 >= 0 && y - 1 <= boardSize && x - 1 >= 0 && x - 1 < boardSize && enemyBoard[y - 1][x - 1] == ' ') // If a ship is within the board and not intercepting with another ship.
								{
									check++; // Increments blank check...
								}
								y -= 1; // Checks next space.
							}
							if (check == shipBeingPlaced) // If all the spaces a ship needs are valid...
							{
								y += shipBeingPlaced; // Returns y to starting value.
								for (int j = 0; j < shipBeingPlaced; j++) // For the length of the current ship...
								{
									enemyBoard[y - 1][x - 1] = shipChar; // Places a ship character in the board.
									y -= 1;
								}
							}
							else // If invalid location selected...
							{
								system("CLS");
								cout << "Invalid Location!" << endl;
								y += shipBeingPlaced; // Returns y to the starting place.
								enemyBoard[y - 1][x - 1] = ' '; // Clears it.
								drawBoard(enemyBoard);
								currentShip--; // Returns to the last ship.
								system("PAUSE");
							}
							break;
						case 2: // Places a ship downwards.
							for (int j = 0; j < shipBeingPlaced; j++)
							{
								if (y - 1 >= 0 && y - 1 <= boardSize && x - 1 >= 0 && x - 1 < boardSize && enemyBoard[y - 1][x - 1] == ' ')
								{
									check++;
								}
								y += 1;
							}
							if (check == shipBeingPlaced)
							{
								y -= shipBeingPlaced;
								for (int j = 0; j < shipBeingPlaced; j++)
								{
									enemyBoard[y - 1][x - 1] = shipChar;
									y += 1;
								}
							}
							else
							{
								system("CLS");
								cout << "Invalid Location!" << endl;
								y -= shipBeingPlaced;
								enemyBoard[y - 1][x - 1] = ' ';
								drawBoard(enemyBoard);
								currentShip--;
								system("PAUSE");
							}
							break;
						case 3: // Places a ship facing towards the right.
							for (int j = 0; j < shipBeingPlaced; j++)
							{
								if (y - 1 >= 0 && y - 1 <= boardSize && x - 1 >= 0 && x - 1 < boardSize && enemyBoard[y - 1][x - 1] == ' ')
								{
									check++;
								}
								x += 1;
							}
							if (check == shipBeingPlaced)
							{
								x -= shipBeingPlaced;
								for (int j = 0; j < shipBeingPlaced; j++)
								{
									enemyBoard[y - 1][x - 1] = shipChar;
									x += 1;
								}
							}
							else
							{
								system("CLS");
								cout << "Invalid Location!" << endl;
								x -= shipBeingPlaced;
								enemyBoard[y - 1][x - 1] = ' ';
								drawBoard(enemyBoard);
								currentShip--;
								system("PAUSE");
							}
							break;
						case 4: // Places a ship facing towards the left.
							for (int j = 0; j < shipBeingPlaced; j++)
							{
								if (y - 1 >= 0 && y - 1 <= boardSize && x - 1 >= 0 && x - 1 < boardSize && enemyBoard[y - 1][x - 1] == ' ')
								{
									check++;
								}
								x -= 1;
							}
							if (check == shipBeingPlaced)
							{
								x += shipBeingPlaced;
								for (int j = 0; j < shipBeingPlaced; j++)
								{
									enemyBoard[y - 1][x - 1] = shipChar;
									x -= 1;
								}
							}
							else
							{
								system("CLS");
								cout << "Invalid Location!" << endl;
								x += shipBeingPlaced;
								enemyBoard[y - 1][x - 1] = ' ';
								drawBoard(enemyBoard);
								currentShip--;
								system("PAUSE");
							}
							break;
						default:
							break;
						}
						system("CLS");
						drawBoard(enemyBoard);
						currentShip++;
					}
				}
			}
		}
	}
}

void Game::playerShooting(char board[10][10], char hitBoard[10][10], char damageBoard[10][10])
{
	char hitChar = ' '; // Character that is hit.
	char shotValue; // Shot value.
	int shootX;
	int shootY;
	bool lastHit = true; // While the last shot was a hit.
	while (lastHit == true)
	{
		system("CLS");
		if (currentPlayer == 1 || currentPlayer == 2 && gameMode == 2) // If it is player 1 or a human player 2.
		{
			drawBoard(board);
			drawBoard(hitBoard);
		}
		if (gameMode == 2 || currentPlayer == 1) // If it is player 1 or multiplayer.
		{
			cout << "Where would you like to shoot? (X,Y)" << endl;
			cout << "X: ";
			cin >> shootY;
			if (shootY > 10 || shootY < 1) // Invalid location entered.
			{
				cout << "That position is invalid. Please try again!" << endl;
			}
			cout << "Y: ";
			cin >> shotValue; // Enters character Y value.
			shootX = toupper(shotValue); // Converts to uppercase value.	
			shootX -= 64;
			if (shootX > 10 || shootX < 1) // Invalid location entered.
			{
				cout << "That position is invalid. Please try again!" << endl;
			}
		}
		if (gameMode == 1 && currentPlayer == 2) // Shoots randomly if it is computer's turn.
		{
			shootX = rand() % 10 + 1;
			shootY = rand() % 10 + 1;
		}
		if (hitBoard[shootX - 1][shootY - 1] != 'X' && hitBoard[shootX - 1][shootY - 1] != 'M') // If the player have not already shot this place before.
		{
			if (damageBoard[shootX - 1][shootY - 1] != ' ') // Player hits then...
			{
				hitChar = damageBoard[shootX - 1][shootY - 1]; // Finds out which ship has been hit.
				hitBoard[shootX - 1][shootY - 1] = 'X'; // Marks hit with an 'X'.
				damageBoard[shootX - 1][shootY - 1] = 'X'; // Marks hit players ship with an 'X'.
				system("CLS");
				if (currentPlayer == 1 || currentPlayer == 2 && gameMode == 2)
				{
					drawBoard(board);
				}
				drawBoard(hitBoard);
				cout << "HIT!" << endl;
				shipDestroyed(hitChar); // Minus health from the hit.
				system("PAUSE");
			}
			else
			{
				system("CLS"); // If a player misses.
				hitBoard[shootX - 1][shootY - 1] = 'M'; // Mark miss location with an 'M'.
				if (currentPlayer == 1 || currentPlayer == 2 && gameMode == 2) // If against human player.
				{
					drawBoard(board);
				}
				drawBoard(hitBoard);
				if (currentPlayer == 1) // Player 1 missed.
				{
					cout << endl << player1Name << " missed!" << endl;
				}
				else if (currentPlayer == 2) // Player 2 missed.
				{
					cout << endl << player2Name << " missed!" << endl;
				}
				system("PAUSE");
				system("CLS");
				lastHit = false;
				return;
			}
		}
	}
}

void Game::playGame()
{
	while (gameEnd == false) // While game has not ended.
	{
		switchCurrentPlayer(); // Switches player.
		cout << currentPlayer << endl;
		if (currentPlayer == 1) // Runs routine for corresponding player.
		{
			system("CLS");
			cout << player1Name << "'s turn!" << endl;
			system("PAUSE");
			playerShooting(playerBoard, enemyHits, enemyBoard);
		}
		if (currentPlayer == 2)
		{
			system("CLS");
			cout << player2Name << "'s turn!" << endl;
			system("PAUSE");
			playerShooting(enemyBoard, playerHits, playerBoard);
		}
	}
}

void Game::shipDestroyed(char hitsChar)
{
	if (currentPlayer == 1) // If a player 1 ships are hit.
	{
		switch (hitsChar)
		{
		case 'S': // If Submarine is hit.
			player1ShipHealth[0]--; // Decrease Submarine health.
			if (player1ShipHealth[0] == 0) // If health is 0.
			{
				cout << endl << "Submarine has been sunk!" << endl; // Show sunk message.
			}
			break;
		case 'D': // Destroyer.
			player1ShipHealth[1]--;
			if (player1ShipHealth[1] == 0)
			{
				cout << endl << "Destoyer has been sunk!" << endl;
			}
			break;
		case 'C': // Cruiser.
			player1ShipHealth[2]--;
			if (player1ShipHealth[2] == 0)
			{
				cout << endl << "Cruiser has been sunk!" << endl;
			}
			break;
		case 'B': // Battleship.
			player1ShipHealth[3]--;
			if (player1ShipHealth[3] == 0)
			{
				cout << endl << "Battleship has been sunk!" << endl;
			}
			break;
		case 'A': // Air craft carrier.
			player1ShipHealth[4]--;
			if (player1ShipHealth[4] == 0)
			{
				cout << endl << "Aircraft Carrier has been sunk!" << endl;
			}
			break;
		}
	}
	else if (currentPlayer == 2) // Same as above for player 2.
	{
		switch (hitsChar)
		{
		case 'S':
			player2ShipHealth[0]--;
			if (player2ShipHealth[0] == 0)
			{
				cout << endl << "Submarine has been sunk!" << endl;
			}
			break;
		case 'D':
			player2ShipHealth[1]--;
			if (player2ShipHealth[1] == 0)
			{
				cout << endl << "Destoyer has been sunk!" << endl;
			}
			break;
		case 'C':
			player2ShipHealth[2]--;
			if (player2ShipHealth[2] == 0)
			{
				cout << endl << "Cruiser has been sunk!" << endl;
			}
			break;
		case 'B':
			player2ShipHealth[3]--;
			if (player2ShipHealth[3] == 0)
			{
				cout << endl << "Battleship has been sunk!" << endl;
			}
			break;
		case 'A':
			player2ShipHealth[4]--;
			if (player2ShipHealth[4] == 0)
			{
				cout << endl << "Aircraft Carrier has been sunk!" << endl;
			}
			break;
		}
	}
	if (player1ShipHealth[0] == 0 && player1ShipHealth[1] == 0 && player1ShipHealth[2] == 0 && player1ShipHealth[3] == 0 && player1ShipHealth[4] == 0) // All of player 1 ships have been destroyed.
	{
		system("CLS");
		cout << player1Name << " Wins!" << endl;
	}
	if (player2ShipHealth[0] == 0 && player2ShipHealth[1] == 0 && player2ShipHealth[2] == 0 && player2ShipHealth[3] == 0 && player2ShipHealth[4] == 0) // All of player 2 ships have been destroyed.
	{
		system("CLS");
		cout << player2Name << " Wins!" << endl;
	}
}

void Game::setShipType(int &currentShip, int &shipBeingPlaced, string &shipName, char &shipChar) // Cycles through all ships.
{
	// Constants holding lengths of different ships.
	const int Submarine = 2;
	const int Destroyer = 3;
	const int Cruiser = 3;
	const int Battleship = 4;
	const int AirCraftCarrier = 5;
	switch (currentShip)
	{
	case 1: // Submarine.
		shipBeingPlaced = Submarine;
		shipName = "Submarine";
		shipChar = 'S';
		break;
	case 2: // Destroyer.
		shipBeingPlaced = Destroyer;
		shipName = "Destroyer";
		shipChar = 'D';
		break;
	case 3: // Cruiser.
		shipBeingPlaced = Cruiser;
		shipName = "Cruiser";
		shipChar = 'C';
		break;
	case 4: // BattleShip.
		shipBeingPlaced = Battleship;
		shipName = "Battleship";
		shipChar = 'B';
		break;
	case 5: // Air Craft Carrier.
		shipBeingPlaced = AirCraftCarrier;
		shipName = "AirCraft Carrier";
		shipChar = 'A';
		break;
	default: // Runs correct default depending on if players are human or computer.
		system("CLS");
		if (currentPlayer == 1)
		{
			if (gameMode == 1)
			{
				cout << "Your ships have been successfully placed!" << endl;
				system("PAUSE");
				system("CLS");
				currentPlayer = 2;
			}
			if (gameMode == 2)
			{
				cout << "Ships are successfully placed! Pass the game to " << player2Name << " so they may place their ships!" << endl;
				system("PAUSE");
				system("CLS");
				currentPlayer = 2;
				drawBoard(enemyBoard);
			}
			currentShip = 1;
			break;
		}
		if (currentPlayer == 2)
		{
			cout << player2Name << " has finished placing his ships!" << endl;
			cout << player1Name << " will take the first shot!" << endl;
			system("PAUSE");
			currentPlayer = 2;
			return;
		}
		break;
	}
}

int Game::switchCurrentPlayer() // Switches the current player.
{
	if (currentPlayer == 1)
	{
		currentPlayer = 2;
	}
	else if (currentPlayer == 2)
	{
		currentPlayer = 1;
	}
	return currentPlayer;
}

int Game::mainMenu()
{
	// Draws the main menu user interface.
	int menuChoice;
	cout << "**************************************************" << endl;
	cout << "                    Battleships                   " << endl;
	cout << "**************************************************" << endl << endl;
	cout << "Welcome! What would you like to play?" << endl << endl;
	cout << "1.) Single Player" << endl;
	cout << "2.) Multiplayer" << endl;
	cout << "3.) Exit Game" << endl;
	cin >> menuChoice; // Asks for user's choice.
	if (menuChoice == 1) // If a single player is selected then ask for player's name.
	{
		cout << "Enter Player Name: ";
		cin >> player1Name;
	}
	if (menuChoice == 2) // If a multiplayer is selected then ask for both players names.
	{
		cout << "Enter Player 1 Name: ";
		cin >> player1Name;
		cout << "Enter Player 2 Name: ";
		cin >> player2Name;
	}
	if (menuChoice == 3) // If exit is selected then send message.
	{
		cout << "Thank you for playing!" << endl;
		system("PAUSE");
	}
	system("CLS");
	return menuChoice;
}
