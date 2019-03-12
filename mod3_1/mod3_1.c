//Thomas Cooper
//3/19/17
//Module 3 assignment 1

//preprocessors
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

//these define some constants for sorting later. There are only 4 suits, there are only 13 cards in each suit, and total there will always be 52 cards
#define SUITS 4
#define FACES 13
#define CARDS 52

//function prototypes
void shuffle(unsigned int wDeck[][FACES], int wDeckOrder[]);
void deal(unsigned int wDeck[][FACES], const char *wFace[], const char *wSuit[], int wDeckOrder[], int playerHand[], int dealCount);
void checkHand(int playerHand[]);
bool pair(int playerHand[]);
bool twoPair(int playerHand[]);
bool threeOaK(int playerHand[]);
bool fourOaK(int playerHand[]);
bool flush(int playerHand[]);
bool straight(int playerHand[]);
bool fullHouse(int playerHand[]);


//main
int main(void)
{
	//datatypes being initalized
	char repeat = 'y';
	//the deck is initialized and all elements are set to zero
	unsigned int deck[SUITS][FACES] = { 0 };

	//datatypes for the deck order, the players hand, and the identifier keeping track of what the deal it is
	int deckOrder[CARDS], hand[5], dealNum = 0;

	//the suits are initialized here
	const char *suit[SUITS] =
	{ "Hearts", "Diamonds", "Clubs", "Spades" };

	//the face cards are initialized here
	const char *face[FACES] =
	{ "Ace", "Deuce", "Three", "Four",
	 "Five", "Six", "Seven", "Eight",
	 "Nine", "Ten", "Jack", "Queen", "King" };

	//random number generation is initialized here
	srand((unsigned int)time(NULL));

	//the deck is shuffled. the deck, and deckOrder arrays are passed in. This is the initial shuffle before the loop
	shuffle(deck, deckOrder);

	//while loop to keep the game going in case you still feel like losing
	while (repeat != 'n')
	{
		//the deal function is run. This gets the player their hand.
		deal(deck, face, suit, deckOrder, hand, dealNum);
		
		//the deal counter is incremented
		dealNum++;

		//function to check if the player has anything. only the hand array is passed in
		checkHand(hand);
		
		
		//the check to see if the player wishes to keep going
		printf_s("%s", "  Would you like to have a new hand dealt? y/n: ");
		scanf_s(" %c", &repeat, 1);
		//new line for pretty stuff
		puts("\n");

		//important. for one player you can get 10, 5 card hands out of a single deck before needing to shuffle again. This checks
		//the deal number to see if that limit has been reached and then the deal counter is set back to zero, and the shuffle function is run again
		if (dealNum > 9)
		{
			dealNum = 0;
			shuffle(deck, deckOrder);
		}
	}//end while
}//end main

//the function shuffle
void shuffle(unsigned int wDeck[][FACES], int wDeckOrder[])
{
	//the identifiers are created here
	int temp, notTwice[CARDS] = {0}, order = 0, cardNum = 0;
	int card = 0, q = 0;

		//nested for loops to traverse the deck
		for (size_t row = 0; row < SUITS; ++row) {
			for (size_t column = 0; column < FACES; ++column) {
				//random number generation between 0 and 51
				temp = rand() % 52;
				
				//simple check with a simple array to make certain that the same card is not dealt twice in the same shuffle
				while (notTwice[temp] == 1)
				{
					temp = rand() % 52;
				}

				//the assignment of the cardorder for dealing. This was much simpler to use a one dimensional array, 
				//however here is the original deck array also getting the assignments
				wDeck[row][column] = temp;
				wDeckOrder[cardNum] = temp;
				notTwice[temp] = 1;
				cardNum++;
			}
		}
}

//the deal function
void deal(unsigned int wDeck[][FACES], const char *wFace[], const char *wSuit[], int wDeckOrder[], int playerHand[], int dealCount)
{
	int cardNum = 0, temp, sTemp, skipCount;

	//to ensure that the first five cards are not dealt every time a skip counter is incremented. simple multiplication of 5 gets
	//the correct card order
	skipCount = 5 * dealCount;

	//the actual assignment of the cards into the five places in the players hand. since the entire deck is easily divided by 13 to seperate the 4 suits
	//the number is checked to see which quarter of the deck it falls in, and then was modulated by 13 to find its appropriate place in the suited section of the deck
	for (int i = 0; i < 5; i++)
	{
		playerHand[i] = wDeckOrder[cardNum + skipCount];

		if (wDeckOrder[cardNum + skipCount] >= 0 && wDeckOrder[cardNum + skipCount] < 13)
		{
			temp = wDeckOrder[cardNum + skipCount] % 13;
			printf_s("%5s of %s  ", wFace[temp], wSuit[0]);
		}

		if (wDeckOrder[cardNum + skipCount] >= 13 && wDeckOrder[cardNum + skipCount] < 26)
		{
			temp = wDeckOrder[cardNum + skipCount] % 13;
			printf_s("%5s of %s  ", wFace[temp], wSuit[1]);
		}

		if (wDeckOrder[cardNum + skipCount] >= 26 && wDeckOrder[cardNum + skipCount] < 39)
		{
			temp = wDeckOrder[cardNum + skipCount] % 13;
			printf_s("%5s of %s  ", wFace[temp], wSuit[2]);
		}

		if (wDeckOrder[cardNum + skipCount] >= 39 && wDeckOrder[cardNum + skipCount] < 52)
		{
			temp = wDeckOrder[cardNum + skipCount] % 13;
			printf_s("%5s of %s  ", wFace[temp], wSuit[3]);
		}
		cardNum++;
	}

	//bubble sort to put the hand in numerical order for simpler winning checks
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 1; j < 4; ++j)
		{
			if (playerHand[j] > playerHand[j + 1]) {
				sTemp = playerHand[j + 1];
				playerHand[j + 1] = playerHand[j];
				playerHand[j] = sTemp;
			}
		}
	}
}

//the function to check the players hand
void checkHand(int playerHand[])
{
	//identifiers are created
	int handScore = 0;

	//check for four of a kind
	if (fourOaK(playerHand))
	{
		handScore = 7;
	}

	//check for fullhouse
	if (fullHouse(playerHand))
	{
		handScore = 6;
	}

	//check for flush
	if (flush(playerHand))
	{
		handScore = 5;
	}

	//check for straight
	if (straight(playerHand))
	{
		handScore = 4;
	}

	//check for three of a kind. Since a fullhouse is a pair and a three of a kind, we want to make sure this doesn't trigger when the slim chance of a fullhouse happens.
	//So we make sure that fullhouse is false before checking for three of a kind
	if (!fullHouse(playerHand) && threeOaK(playerHand))
	{
		handScore = 3;
	}

	//check for two pair. Since four of a kind, and fullhouse would trigger two pair, we make sure they are both false when checking for two pair
	if (twoPair(playerHand) && !fourOaK(playerHand) && !fullHouse(playerHand))
	{
		handScore = 2;
	}

	//check for a pair. Since two pair, four of a kind, full house, and three of a kind all contain a pair, we make sure they're false when checking for a pair
	if (pair(playerHand) && !twoPair(playerHand) && !fourOaK(playerHand) && !fullHouse(playerHand) && !threeOaK(playerHand))
	{
		handScore = 1;
	}

	//pretty stuff
	printf_s("%s", "\n");

	//never done a switch and case before, thought I would implement it here for the announcement of the hand
	switch (handScore)
	{
	case 7:
		puts("  You have a four of a kind!");
		break;
	case 6:
		puts("  You have a Full House!");
		break;
	case 5:
		puts("  You have a Flush!");
		break;
	case 4:
		puts("  You have a straight!");
		break;
	case 3:
		puts("  You have three of a kind!");
		break;
	case 2:
		puts("  You have two pairs!");
		break;
	case 1:
		puts("  You have a pair!");
		break;
	case 0:
		puts("  You have nothing!");
		break;
	}
}

//function to check if the hand elements in the hand contain two numbers that are equal. This function compares every 
//combination of possible pairs regardless of which element those pairs are in, i.e. second element and last element
bool pair(int playerHand[])
{
	if ((playerHand[0] % 13 == playerHand[1] % 13) ||
		(playerHand[0] % 13 == playerHand[2] % 13) ||
		(playerHand[0] % 13 == playerHand[3] % 13) ||
		(playerHand[0] % 13 == playerHand[4] % 13) ||
		(playerHand[1] % 13 == playerHand[2] % 13) ||
		(playerHand[1] % 13 == playerHand[3] % 13) ||
		(playerHand[1] % 13 == playerHand[4] % 13) ||
		(playerHand[2] % 13 == playerHand[3] % 13) ||
		(playerHand[2] % 13 == playerHand[4] % 13) ||
		(playerHand[3] % 13 == playerHand[4] % 13))
	{
		return true;
	}
	else return false;
}

//function to check for two pairs. This is just like the pair checking however its looking to make sure two sets of equality happen
bool twoPair(int playerHand[])
{
	if ((playerHand[0] % 13 == playerHand[1] % 13) && (playerHand[2] % 13 == playerHand[3] % 13) ||
		(playerHand[0] % 13 == playerHand[1] % 13) && (playerHand[2] % 13 == playerHand[4] % 13) ||
		(playerHand[0] % 13 == playerHand[1] % 13) && (playerHand[3] % 13 == playerHand[4] % 13) ||
		(playerHand[0] % 13 == playerHand[2] % 13) && (playerHand[3] % 13 == playerHand[4] % 13) ||
		(playerHand[1] % 13 == playerHand[2] % 13) && (playerHand[3] % 13 == playerHand[4] % 13) 
		)
	{
		return true;
	}

	else return false;
}

//function to check for three of a kind this function is just like the two pair function but instead of two sets of two for equality, it just makes sure the first 
//checked card is also equal to another card in the array.
bool threeOaK(int playerHand[])
{
	if ((playerHand[0] % 13 == playerHand[1] % 13 && playerHand[0] % 13 == playerHand[2] % 13) ||
		(playerHand[0] % 13 == playerHand[1] % 13 && playerHand[0] % 13 == playerHand[3] % 13) ||
		(playerHand[0] % 13 == playerHand[1] % 13 && playerHand[0] % 13 == playerHand[4] % 13) ||
		(playerHand[0] % 13 == playerHand[2] % 13 && playerHand[0] % 13 == playerHand[3] % 13) ||
		(playerHand[0] % 13 == playerHand[2] % 13 && playerHand[0] % 13 == playerHand[4] % 13) ||
		(playerHand[0] % 13 == playerHand[3] % 13 && playerHand[0] % 13 == playerHand[4] % 13) ||
		(playerHand[1] % 13 == playerHand[2] % 13 && playerHand[1] % 13 == playerHand[3] % 13) ||
		(playerHand[1] % 13 == playerHand[2] % 13 && playerHand[1] % 13 == playerHand[4] % 13) ||
		(playerHand[2] % 13 == playerHand[3] % 13 && playerHand[2] % 13 == playerHand[4] % 13))
	{
		return true;
	}
	else return false;
}

//function to check for four of a kind. Its just like the others but checks to make sure the first checked card is equal to three others
bool fourOaK(int playerHand[])
{
	if ((playerHand[0] % 13 == playerHand[1] % 13 && playerHand[0] % 13 == playerHand[2] % 13 && playerHand[0] % 13 == playerHand[3] % 13) ||
		(playerHand[0] % 13 == playerHand[1] % 13 && playerHand[0] % 13 == playerHand[2] % 13 && playerHand[0] % 13 == playerHand[4] % 13) ||
		(playerHand[0] % 13 == playerHand[1] % 13 && playerHand[0] % 13 == playerHand[3] % 13 && playerHand[0] % 13 == playerHand[4] % 13) ||
		(playerHand[0] % 13 == playerHand[2] % 13 && playerHand[0] % 13 == playerHand[3] % 13 && playerHand[0] % 13 == playerHand[4] % 13) ||
		(playerHand[1] % 13 == playerHand[2] % 13 && playerHand[0] % 13 == playerHand[3] % 13 && playerHand[0] % 13 == playerHand[4] % 13))
	{
		return true;
	}

	else return false;
}

//function to check for a flush. This function just checks to make sure that all of the cards are within the same section of the deck, i.e. 0 - 12 are hearts
bool flush(int playerHand[])
{
	if (((playerHand[0] >= 0 && playerHand[0] < 13) && (playerHand[1] >= 0 && playerHand[1] < 13) && (playerHand[2] >= 0 && playerHand[2] < 13) && (playerHand[3] >= 0 && playerHand[3] < 13) && (playerHand[4] >= 0 && playerHand[4] < 13)) ||
		((playerHand[0] >= 13 && playerHand[0] < 26) && (playerHand[1] >= 13 && playerHand[1] < 26) && (playerHand[2] >= 13 && playerHand[2] < 26) && (playerHand[3] >= 13 && playerHand[3] < 26) && (playerHand[4] >= 13 && playerHand[4] < 26)) ||
		((playerHand[0] >= 26 && playerHand[0] < 39) && (playerHand[1] >= 26 && playerHand[1] < 39) && (playerHand[2] >= 26 && playerHand[2] < 39) && (playerHand[3] >= 26 && playerHand[3] < 39) && (playerHand[4] >= 26 && playerHand[4] < 39)) ||
		((playerHand[0] >= 39 && playerHand[0] < 52) && (playerHand[1] >= 39 && playerHand[1] < 52) && (playerHand[2] >= 39 && playerHand[2] < 52) && (playerHand[3] >= 39 && playerHand[3] < 52) && (playerHand[4] >= 39 && playerHand[4] < 52)))
	{
		return true;
	}

	else return false;
}

//function to check for a straight. Simple check to ensure that the cards are in sequential order.
bool straight(int playerHand[])
{
	if (((playerHand[0] % 13) == ((playerHand[1] % 13) - 1)) &&
		((playerHand[1] % 13) == ((playerHand[2] % 13) - 1)) &&
		((playerHand[2] % 13) == ((playerHand[3] % 13) - 1)) &&
		((playerHand[3] % 13) == ((playerHand[4] % 13) - 1)))
	{
		return true;
	}

	else return false;
}

//function to check for a full house. It checks that the first two cards are equal and the last three cards are equal, and vice versa.
bool fullHouse(int playerHand[])
{
	if (((playerHand[0] % 13 == playerHand[1] % 13) && ((playerHand[2] % 13 == playerHand[3] % 13) && (playerHand[2] % 13 == playerHand[4] % 13))) ||
		((playerHand[0] % 13 == playerHand[1] % 13) && (playerHand[0] % 13 == playerHand[2] % 13)) && (playerHand[3] % 13 == playerHand[4] % 13))
	{
		return true;
	}

	else return false;
}