#include <iostream>
#include <stdio.h>
#include <time.h>
#include <vector>

//Using declarations, apparently using directives are bad practice, but since I have only 1 namespace it shouldn't matter too much, just doing it this way for practice;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::swap;

const string suitNames[] = { "Invalid", "Spades", "Hearts", "Clubs", "Diamonds" };
const string cardNames[] = { "Invalid", "Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };
const int cardValues[] = { -1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11 };

class Card {
	enum Suit {
		spades = 1,
		hearts = 2,	
		clubs = 3,
		diamonds = 4,
	};
	int number;
	Suit suit;

public:
	Card (int, int);

	Suit getSuit() {
		return suit;
	}

	int getNumber() {
		return number;
	}
};

Card::Card(int num, int suitEnum) {
	if (!(num > 0 && num <= 13)) {
		cout << "Invalid Card Number Detected (Expected 1 thru 13) got " << num << endl;
		return;
	}
	if (!(suitEnum > 0 && suitEnum <= 4)) {
		cout << "Invalid Suit Enumerable Detected (Expected 1 thru 4) got " << suitEnum << endl;
		return;
	}
	number = num;
	suit = static_cast<Suit>(suitEnum);
}

class Deck {
	vector<Card> Deck;
public:
	vector<Card> getDeck() {
		return Deck;
	}


	void createDeck() {
		for (int suit = 1; suit <= 4; ++suit) {
			for (int num = 1; num <= 13; ++num) {
				Deck.push_back(Card (num, suit));
			}
		}
	}

	void shuffleDeck() { 
		srand(time(NULL));

		int numOfShuffles = rand() % 21 + 5; //Shuffle 5 to 25 times
		int swapsPerShuffle = rand() % 26 + 1; //1 to 52 cards will be swapped per shuffle

		for (int shuffles = 0; shuffles <= numOfShuffles; ++shuffles) {
			for (int swaps = 0; swaps <= swapsPerShuffle; ++swaps) {
				int swapIndex = rand() % Deck.size(); //index of target card which will be swapped with another random card
				int otherCard = rand() % Deck.size();
				swap(Deck[swapIndex], Deck[otherCard]);

			}
		}
	}

	Card drawCardFromDeck() {
		Card drawnCard = Deck.back();
		Deck.pop_back();
		return drawnCard;
	}

	void drawCardIntoDeck(Card drawnCard) {
		Deck.push_back(drawnCard);
	}
};

class Player {
protected:
	vector<Card> hand;
	bool isWinner = false;
	double money = 0;

public:
	void printHand() {
		for (vector<Card>::iterator card = hand.begin(); card != hand.end(); ++card) {
			cout << "The " << cardNames[card->getNumber()] << " of " << suitNames[card->getSuit()] << endl;
		}
	}

	vector<Card> getHand() {
		return hand;
	}

	void drawCardIntoHand(Card drawnCard) {
		hand.push_back(drawnCard);
	}

	Card drawCardFromHand() {
		Card drawnCard = hand.back();
		hand.pop_back();
		return drawnCard;
	}
	
	bool getIsWinner() {
		return isWinner;
	}

	void setIsWinner(bool won) {
		isWinner = won;
	}
	
	void setMoney(double newValue) {
		money = newValue;
	}
	
	double getMoney() {
		return money;
	}
	
	int totalCardValue() {
		int sum = 0;
		int numAces = 0;
		for (vector<Card>::iterator card = hand.begin(); card != hand.end(); ++card) {
			if (cardValues[card->getNumber()] == 1) {
				++numAces;
			}
			sum += cardValues[card->getNumber()];
		}

		for (int ace = 0; ace <= numAces; ++ace) {
			if (sum <= 11) {
				sum += 10; //Aces are still being added as 1 above, so if we want to consider an ace to be 11, just add 10
			}
		}
		return sum;
	}

};

class Dealer : public Player {
public:
	void initialHandReveal() {
		cout << "A face down card, and the " << cardNames[hand.at(1).getNumber()] << " of " << suitNames[hand.at(1).getSuit()];
	}
};

void drawCard(Player &player, Deck &deck) {
	Card drawnCard = deck.drawCardFromDeck();
	player.drawCardIntoHand(drawnCard);
	return;
}

void returnCardsToDeck(Player& player, Deck& deck) {
	while (player.getHand().size() != 0) {
		Card drawnCard = player.drawCardFromHand();
		deck.drawCardIntoDeck(drawnCard);
	}
	return;
}


int main() //TODO: Add split
{
	enum gameConditions {
		gameInProgress = 0,
		endGame = 1,
	};
	string anotherRound;
	gameConditions gameStatus = gameInProgress;
	Player player1;
	Dealer dealer; 
	player1.setMoney(1000.0);
	double bet = 0;
	Deck deck;
	Deck discardedPile;
	deck.createDeck();
	deck.shuffleDeck();
	while (gameStatus == gameInProgress) {

		//Initial bet
		cout << endl << "You currently have " << player1.getMoney() << " dollars. How much would you like to bet?" << endl;
		do {
			cin >> bet;
			if (bet > player1.getMoney()) {
				cout << "You don't have enough money!" << endl;
			}
			else {
				player1.setMoney(player1.getMoney() - bet);
				break;
			}
		} while (bet > player1.getMoney());

		//Draw cards
		drawCard(player1, deck);
		drawCard(player1, deck);

		//Initialize dealer's hand
		drawCard(dealer, deck);
		drawCard(dealer, deck);

		cout << "The dealer's hand contains: " << endl;
		dealer.initialHandReveal();
		cout << endl;
		//TO DO: Implement insurance bets


		cout << "Your hand contains: " << endl;
		player1.printHand();

		//Option to double down
		string doubleDownDecision;
		bool isDoubledDown;
		cout << "Would you like to double down? ('yes' or 'no')" << endl;
		cin >> doubleDownDecision;

		while (doubleDownDecision != "yes" && doubleDownDecision != "no") {
			cout << "Invalid entry, type 'yes' or 'no'." << endl;
			cin >> doubleDownDecision;
		}

		if (doubleDownDecision == "yes" && player1.getMoney() >= bet) {
			player1.setMoney(player1.getMoney() - bet);
			bet *= 2;
			isDoubledDown = true;
			cout << "You now have " << player1.getMoney() << " dollars. Your bet is increased to " << bet << " dollars." << endl;
		}
		else if (doubleDownDecision == "no") {
			isDoubledDown = false;
		}
		else if (doubleDownDecision == "yes" && player1.getMoney() < bet) {
			cout << "You don't have enough money to double down!";
			isDoubledDown = false;
		}

		bool isStaying = false;
		bool playerBusted = false;
		
		do {
			if (player1.totalCardValue() > 21) {
				cout << "Bust!" << " You have " << player1.getMoney() << " left" << endl;
				playerBusted = true;
				break;
			}
			cout << "Would you like to hit or stay?" << endl;
			string decision;
			cin >> decision;

			if (decision != "hit" && decision != "stay") {
				cout << "Invalid entry, type 'hit' or 'stay'" << endl;
			}
			else if (decision == "hit" && player1.totalCardValue() <= 21) {
				drawCard(player1, deck);
				cout << "Your new hand is " << endl;
				player1.printHand();
			}
			else if (decision == "stay") {
				cout << "You have chosen to stay." << endl;
				isStaying = true;
			}
		} while (!isStaying && isDoubledDown == false);

		if (!playerBusted) {
			cout << "It is now the dealer's turn. They reveal their face down card. Their hand contains:" << endl;
			dealer.printHand();

			cout << "The dealer will now draw cards..." << endl;

			while (dealer.totalCardValue() < 17 && !playerBusted) {
				drawCard(dealer, deck);
			}

			if (dealer.totalCardValue() < 17) {
				cout << "The dealer's updated hand contains: " << endl;
				dealer.printHand();
			}

			if (dealer.totalCardValue() > 21) {
				double oldMoney = player1.getMoney();
				player1.setMoney(oldMoney + 3 * (bet) / 2);
				cout << "Your hand has a value of " << player1.totalCardValue() << ", while the dealer's hand has a total value of " << dealer.totalCardValue() << endl;
				cout << "The dealer has busted! You win!" << "The dealer pays you " << (3 * bet) / 2 << " dollars. You now have a total of " << player1.getMoney() << endl;
				return 0;
			}

			cout << "Your hand has a value of " << player1.totalCardValue() << ", while the dealer's hand has a total value of " << dealer.totalCardValue() << endl;
			if (player1.totalCardValue() <= 21) {
				if (21 - player1.totalCardValue() < 21 - dealer.totalCardValue()) {
					double oldMoney = player1.getMoney();
					player1.setMoney(oldMoney + 3 * (bet) / 2);
					cout << "You have won!" << "The dealer pays you " << (3 * bet) / 2 << " dollars. You now have a total of " << player1.getMoney() << endl;
				}
				else if (21 - player1.totalCardValue() > 21 - dealer.totalCardValue()) {
					cout << "You lose." << endl;
				}
				else if (21 - player1.totalCardValue() == 21 - dealer.totalCardValue()) {
					cout << "Tie, your bet will be returned." << endl;
					player1.setMoney(player1.getMoney() + bet);
				}
			}

		}

		while (anotherRound != "yes" && anotherRound != "no" && player1.getMoney() != 0) {
			cout << "Would you like to play another round?" << endl;
			cin >> anotherRound;
			if (anotherRound == "yes") {
				//player and dealer's cards go to discarded pile. deck is reshuffled, bet is reset to 0
				//THIS LOGIC IS BROKEN RN, IF YOU BUST AND LOSE ALL UR MONEY THE GAME JUST MAGICALLY RESETS ANYWAYS LOL -> This happened to me twice now
				bet = 0;
				returnCardsToDeck(player1, discardedPile);
				returnCardsToDeck(dealer, discardedPile);
				if (deck.getDeck().size() == 0) {
					cout << "The deck is out of cards, shuffling the discarded pile..." << endl;
					//TODO: IMPLEMENT THIS
				}
				deck.shuffleDeck();
			}
			else if (anotherRound == "no" || player1.getMoney() == 0) {
				gameStatus = endGame;
				return 0;
			}
			else {
				cout << "Invalid entry, type yes or no." << endl;
			}

		}
	}

	return 0;
}