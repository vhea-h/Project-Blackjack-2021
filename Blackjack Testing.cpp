/*This is a program that allows the user to play Blackjack (a popular casino card game). 
    Written by: Vhea He */

//Libraries Included: 
#include <iostream>
#include <cmath>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iomanip>
using namespace std;

//Declaring constants 
const int MAX_DECK = 52; //Deck of cards without joker cards have 52 cards
const int MAX_HAND = 12; //Maximum number of cards in a hand for bust

//PART 1

//Card Structure Definition
struct Card {
    string cardSuit;
    string cardDescription;
    int cardRank;
    int cardValue;

    Card() {
        cardSuit = "";
        cardDescription = "";
        cardRank = 0;
        cardValue = 0;
    }
};

//Card Array Structure Definition 
struct CardArray {
    Card *cards;
    int maxCards;
    int numUsedCards;

    CardArray(){
        cards = nullptr;
        maxCards = 0;
        numUsedCards = 0;
    }
};

//Function Definitions
//PART 1
void getNewDeck (CardArray & deck);
void printDeck(const CardArray & deck);
void shuffleDeck (CardArray & deck);

//PART 2
void dealCard(CardArray & deck, CardArray & hand);
int getScore (CardArray & hand);
char hitOrStand ();
int blackjack(CardArray & deck);

//PART 3
void playMultipleGames (CardArray & deck);

//PART 4
void aceModifier(CardArray & hand);

//PART 5
void giveAdvice(CardArray & playerHand, CardArray & dealerHand);

int main(){
    CardArray *deck1 = new CardArray();
    getNewDeck(*deck1);
    printDeck (*deck1);
    cout << endl;
    shuffleDeck(*deck1);
    cout << "Shuffled Deck:" << endl;
    printDeck (*deck1);
    cout << endl;

    playMultipleGames (*deck1);
    //hitOrStand();
    delete[] (*deck1).cards;
    delete deck1;
    return 0;
}

//PART 1 cont. 

//Creates a deck of cards
void getNewDeck (CardArray & deck){
    deck.maxCards = deck.numUsedCards = MAX_DECK;
    deck.cards = new Card [MAX_DECK];

    //Rank and suit assignment for Spades suit
    for (int r = 0; r < 13; r++){
        deck.cards[r].cardSuit = "Spades";
        deck.cards[r].cardRank = r + 1;
    }

    //Rank and suit assignment for Hearts suit
    for (int r = 13; r < 26; r++){
        deck.cards[r].cardSuit = "Hearts";
        deck.cards[r].cardRank = r - 12;
    }

    //Rank and suit assignment for Diamonds suit
    for (int r = 26; r < 39; r++){
        deck.cards[r].cardSuit = "Diamonds";
        deck.cards[r].cardRank = r - 25;
    }

    //Rank and suit assignment for Clubs suit
    for (int r = 39; r < MAX_DECK; r++){
        deck.cards[r].cardSuit = "Clubs";
        deck.cards[r].cardRank = r - 38;
    }

    //Assigns value and description to each card in deck
    for (int i = 0; i < MAX_DECK; i++){

        //Assigns description to each card in deck 
        if (deck.cards[i].cardRank == 1)
            deck.cards[i].cardDescription = "A" + (deck.cards[i].cardSuit).substr(0,1);
        else if (deck.cards[i].cardRank > 1 && deck.cards[i].cardRank < 11)
            deck.cards[i].cardDescription = to_string(deck.cards[i].cardRank) + (deck.cards[i].cardSuit).substr(0,1);
        else if (deck.cards[i].cardRank == 11)
            deck.cards[i].cardDescription = "J" + (deck.cards[i].cardSuit).substr(0,1);
        else if (deck.cards[i].cardRank == 12)
            deck.cards[i].cardDescription = "Q" + (deck.cards[i].cardSuit).substr(0,1);
        else if (deck.cards[i].cardRank == 13)
            deck.cards[i].cardDescription = "K" + (deck.cards[i].cardSuit).substr(0,1);
        
        //Assigns value to each card in deck
        if (deck.cards[i].cardRank > 1 && deck.cards[i].cardRank < 11)
            deck.cards[i].cardValue = deck.cards[i].cardRank;
        else if (deck.cards[i].cardRank == 1)
            deck.cards[i].cardValue = 11;
        else if (deck.cards[i].cardRank >= 11)
            deck.cards[i].cardValue = 10;
    }
}

//Prints a deck of cards
void printDeck(const CardArray & deck){
    for (int i = 0; i < deck.numUsedCards; i++){
        cout << deck.cards[i].cardDescription << " ";
        if (i % 13 == 12)
            cout << endl;
    }
}

//Shuffles deck
void shuffleDeck (CardArray & deck){
    srand (time(0));
    for (int oldValueIndex = 0; oldValueIndex < MAX_DECK; oldValueIndex++){
        int newValueIndex = rand()%MAX_DECK;
        Card temp;
        temp = deck.cards[oldValueIndex];
        deck.cards[oldValueIndex] = deck.cards[newValueIndex];
        deck.cards[newValueIndex] = temp;
    }
}

//PART 2

//Deals a single card
void dealCard(CardArray & deck, CardArray & hand) {
    //Increments the # of cards in hand by 1 
    //Sets next unused hand card to last deck card
    //Decrements the # of cards in deck by 1
    hand.cards[hand.numUsedCards++] = deck.cards[(--deck.numUsedCards)];
    if (deck.numUsedCards == 0){
        getNewDeck(deck);
        shuffleDeck(deck);
    }
}

//Gets the score of a hand
int getScore (CardArray & hand){
    int score = 0;
    for (int i = 0; i < hand.numUsedCards; i++)
        score += hand.cards[i].cardValue;
    return score;
}

//Gets and checks user input whether to hit or to stand
char hitOrStand (){
    char cInput = 'a';
    string sInput;
    cout << "Enter h to hit or s to stand: ";
    cin >> sInput;
    cInput = tolower(sInput[0]);
    while ((cInput != 'h' && cInput != 's' && cInput != 'H' && cInput != 'S') || (sInput.length() != 1)){
            cin.clear();
            cin.ignore (1000, '\n');
            cout << "Enter h to hit or s to stand: ";
            cin >> sInput;
            cInput = tolower(sInput[0]);
        }
    return cInput;
}

//Plays a single game of blackjack
int blackjack(CardArray & deck){
    CardArray *playerHand = new CardArray();
    (*playerHand).cards = new Card[MAX_HAND];
    CardArray *dealerHand = new CardArray();
    (*dealerHand).cards = new Card[MAX_HAND];

    //Deals first card to both dealer and player
    //Player is shown both cards
    cout << "Deal first card" << endl << "---------------" << endl;
    dealCard(deck, *playerHand);
    cout << "+Player+: " << (*playerHand).cards[0].cardDescription << endl;
    dealCard(deck, *dealerHand);
    cout << "*Dealer*: " << (*dealerHand).cards[0].cardDescription << endl << endl;
    cout << endl;

    //Deals second card to both dealer and player
    //Player can see their own cards, and dealer's first card
    cout << "Deal second card" << endl << "---------------" << endl;
    dealCard(deck, *playerHand);
    cout << "+Player+: " << (*playerHand).cards[0].cardDescription << " " << (*playerHand).cards[1].cardDescription << endl;
    dealCard(deck, *dealerHand);
    cout << "*Dealer*: " << (*dealerHand).cards[0].cardDescription << " " << "??" << endl << endl;

    //Player's turn to be dealt cards
    cout << "Dealing to player" << endl << "---------------" << endl;

    //Give advice to player before their choice
    giveAdvice(*playerHand, *dealerHand);
    cout << endl;

    //Player chooses to hit or stand
    char choice = hitOrStand();
    cout << endl;
    while (choice == 'h'){
        dealCard(deck, *playerHand);
        int score = getScore(*playerHand);
        cout << "+Player+: ";
        printDeck(*playerHand);
        cout << endl << endl;
        
        //Indicate if player has busted
        //Change value of Aces to 
        if (score > 21){
            aceModifier (*playerHand);
            if (score > 21){
                cout << "Bust! You lose." << endl << endl;
                delete[] (*playerHand).cards;
                delete playerHand;
                delete[] (*dealerHand).cards;
                delete dealerHand;
                return -1;
                break;
            }
        }
        giveAdvice(*playerHand, *dealerHand);
        cout << endl;
        choice = hitOrStand();
    }

    cout << endl;
    //Dealer's turn to be dealt cards
    cout << "Dealing to dealer" << endl << "---------------" << endl;
    if (getScore(*dealerHand) < 17){
        dealCard(deck, *dealerHand);
        cout << "*Dealer*: ";
        printDeck(*dealerHand);
        cout << endl << endl;
        //Indicate if dealer has busted
        if (getScore(*dealerHand) > 21){
            aceModifier (*playerHand);
            if (getScore(*dealerHand) > 21){
                cout << "The dealer has busted. You won!" << endl << endl;
                return 1;
            }
        }
    }

    //Output the final scores if neither party busts
    cout << "Player score: " << getScore(*playerHand) << ", " << "Dealer score: " << getScore(*dealerHand) << endl << endl; 

    //Output game results and return approptiate value
    if (getScore(*dealerHand) > getScore(*playerHand)){
        if (getScore(*dealerHand) == 21)
            cout << "The dealer has reached 21. You lose." << endl << endl;
        else 
            cout << "The dealer has scored higher than you. You lose." << endl << endl;
        return -1;
    }
    else if (getScore(*dealerHand) < getScore(*playerHand)){
        if (getScore(*playerHand) == 21)
            cout << "You have reached 21. Blackjack! You win!" << endl << endl;
        else 
            cout << "You scored higer than the dealer. You win!" << endl << endl;
        return 1;
    }
    else {
        if (getScore(*playerHand) == getScore(*dealerHand) == 21)
            cout << "You and the dealer have both reached 21. The game is a draw." << endl << endl;
        else 
            cout << "You and the dealer got the same score. The game is a draw." << endl << endl;
        return 0;
    }
}

//PART 3

//Allows user to play multiple games of Blackjack, until they choose to quit.
    //Outputs the amount of wins, losses and draws
void playMultipleGames (CardArray & deck){
    char anotherRound;
    int outcome;
    int gameCounter = 0; 
    int wins = 0;
    int losses = 0;
    int draws = 0;
    cout << "BLACKJACK" << endl << "---------" << endl;
    cout << "Do you want to play a hand of Blackjack?" << endl << "(Press y to play, any other letter to quit): ";
    cin >> anotherRound;
    cout << endl;

    while (anotherRound == 'y' || anotherRound == 'Y'){
        outcome = blackjack(deck);
        if (outcome == 1)
            wins++;
        else if (outcome == -1)
            losses++;
        else if (outcome == 0)
            draws++;
        cout << "Do you want to play another hand of Blackjack?" << endl << "(Press y to play, any other letter to quit): ";
        cin >> anotherRound;
        cout << endl;
    }
    gameCounter = wins + losses + draws;

    cout << "Thanks for playing Blackjack!" << endl << "-----------------------------" << endl << "You played " << gameCounter << " games, and here is your record: " << endl << endl;
    cout << "Wins: " << wins << endl;
    cout << "Losses: " << losses << endl;
    cout << "Draws: " << draws << endl;
}

//PART 4 

//Modifies the value of Aces in the player's hand so that it is optimal
//Assumes that score is already above 21, since there is no need to modify aces otherwise
void aceModifier(CardArray & hand){
    for (int i = 0; i < hand.numUsedCards; i++){
        if (hand.cards[i].cardRank == 1){
            if (getScore(hand) > 21)
                hand.cards[i].cardValue = 1;
        }
    }

}

//PART 5

//Gives advice to the player based on the dealer's upcard, and the player's hand
void giveAdvice(CardArray & playerHand, CardArray & dealerHand){
    if (dealerHand.cards[0].cardValue > 6){
        if (getScore(playerHand) < 17){
            cout << "Suggestion: You should hit again." <<endl;
            cout << "The dealer's upcard is a good one, so you should not stop hitting until your score is 17 or higher." << endl;
        }
        if (getScore(playerHand) >= 17){
            cout << "Suggestion: You should stand." << endl;
            cout << "The dealer's upcard is a good one. Since your score is high, it would be risky to keep hitting." << endl;
        }
    }
    else if (dealerHand.cards[0].cardValue > 3 && dealerHand.cards[0].cardValue < 7){
        if (getScore(playerHand) < 12){
            cout << "Suggestion: You should hit again." <<endl;
            cout << "The dealer's upcard is a poor one, so you should not stop hitting until your score is 12 or higher." << endl;
        }
        if (getScore(playerHand) >= 12){
            cout << "Suggestion: You should stand." << endl;
            cout << "The dealer's upcard is a poor one, and they have a high chance of going bust. In this case, you don't want to take any risks." << endl;
        }
    }
    else if (dealerHand.cards[0].cardValue == 2 || dealerHand.cards[0].cardValue == 3){
        if (getScore(playerHand) < 13){
            cout << "Suggestion: You should hit again." <<endl;
            cout << "The dealer's upcard is a fair one, so you should not stop hitting until your score is 13 or higher." << endl;
        }
        if (getScore(playerHand) >= 13){
            cout << "Suggestion: You should stand." << endl;
            cout << "The dealer's upcard is a fair one, so standing now would be the safest option for you." << endl;
        }
    }
}