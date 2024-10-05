/*This is a program that allows the user to play Blackjack (a popular casino card game). 
    Written by: Vhea He 
    SFU Computing ID: vha18 
    Student Number: 301539108

    My process for giving advice depends on the dealer's upcard, and the player's hand. Since the advice 
    function doesn't cheat, a player still may lose following the advice due to bad luck. Inspiration for these
    suggestions came from the Bicycle website. 
    --Advice system:--
    If the dealer's upcard is 7, 8, 9, 10, or ace, player will be prompted to keep hitting until they 
    reach at least 17.
    If the dealer's upcard is 4, 5, or 6, player will be prompted to keep hitting until they 
    reach at least 12.
    If the dealer's upcard is 2 or 3, player will be prompted to keep hitting until they 
    reach at least 13. 
    
    Academic honesty statement: I hereby confirm that this is my own work and I have not violated any of SFU’s 
    Code of Academic Integrity and Good Conduct (S10.01).
*/

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
const int BLACKJACK = 21; //Score for a blackjack

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
int getScore (const CardArray & hand);
char hitOrStand ();
int blackjack(CardArray & deck);

//PART 3
void playMultipleGames (CardArray & deck);

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
        cout << setw(3) << deck.cards[i].cardDescription << " ";
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
int getScore (const CardArray & hand){
    int score = 0;
    int numAces = 0;
    for (int i = 0; i < hand.numUsedCards; i++){
        if (hand.cards[i].cardValue == 11)
            numAces++;
        score += hand.cards[i].cardValue;
    }
    //PART 4 
    //Modifies the value of Aces in the player's hand so that it is optimal
    while (score > BLACKJACK && numAces > 0){
        score -=10;
        numAces--;
    }
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
    cout << "*Dealer*: " << (*dealerHand).cards[0].cardDescription << endl;
    cout << endl;

    //Deals second card to both dealer and player
    //Player can see their own cards, and dealer's first card
    cout << "Deal second card" << endl << "---------------" << endl;
    dealCard(deck, *playerHand);
    cout << "+Player+: " << (*playerHand).cards[0].cardDescription << " " << (*playerHand).cards[1].cardDescription << endl;
    dealCard(deck, *dealerHand);
    cout << "*Dealer*: " << (*dealerHand).cards[0].cardDescription << " " << "??" << endl << endl;

    //Cases where 21 is reached within first two cards
    if (getScore(*playerHand) == BLACKJACK && getScore(*dealerHand) == BLACKJACK){
        cout << "Both you and the dealer have reached 21 within the first two cards. This game is a draw." << endl;
        cout << "Here is the dealer's hand: " << (*dealerHand).cards[0].cardDescription << " " << (*dealerHand).cards[1].cardDescription << endl << endl;
        delete[] (*playerHand).cards;
        delete playerHand;
        delete[] (*dealerHand).cards;
        delete dealerHand;
        return 0;
    }

    else if (getScore(*playerHand) == BLACKJACK && getScore(*dealerHand) != BLACKJACK){
        cout << "You have reached 21 within the first two cards and the dealer has not. You win!" << endl;
        cout << "Here is the dealer's hand: " << (*dealerHand).cards[0].cardDescription << " " << (*dealerHand).cards[1].cardDescription << endl << endl; 
        delete[] (*playerHand).cards;
        delete playerHand;
        delete[] (*dealerHand).cards;
        delete dealerHand;
        return 1;
    }

    else if (getScore(*playerHand) != BLACKJACK && getScore(*dealerHand) == BLACKJACK){
        cout << "The dealer has reached 21 within the first two cards and you have not. You lose." << endl;
        cout << "Here is the dealer's hand: " << (*dealerHand).cards[0].cardDescription << " " << (*dealerHand).cards[1].cardDescription << endl << endl;
        delete[] (*playerHand).cards;
        delete playerHand;
        delete[] (*dealerHand).cards;
        delete dealerHand;
        return -1;
    }

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
        if (score > BLACKJACK){
            cout << "Bust! You lose." << endl << endl;
            delete[] (*playerHand).cards;
            delete playerHand;
            delete[] (*dealerHand).cards;
            delete dealerHand;
            return -1;
            break;
        }
        giveAdvice(*playerHand, *dealerHand);
        cout << endl;
        choice = hitOrStand();
    }

    cout << endl;
    //Dealer's turn to be dealt cards
    cout << "Dealing to dealer" << endl << "---------------" << endl;
    if (getScore(*dealerHand) > 17 && getScore(*dealerHand) < BLACKJACK){
        cout << "*Dealer*: ";
        printDeck(*dealerHand);
        cout << endl << endl;
    }
    while (getScore(*dealerHand) < 17){
        dealCard(deck, *dealerHand);
        cout << "*Dealer*: ";
        printDeck(*dealerHand);
        cout << endl << endl;
    }  
        //Indicate if dealer has busted
    if (getScore(*dealerHand) > BLACKJACK){
        cout << "*Dealer* (Final Hand): ";
        printDeck(*dealerHand);
        cout << endl << endl;
        cout << "The dealer has busted. You won!" << endl << endl;
        delete[] (*playerHand).cards;
        delete playerHand;
        delete[] (*dealerHand).cards;
        delete dealerHand;
        return 1;
    }

    //Output the final scores if neither party busts
    cout << "Player score: " << getScore(*playerHand) << ", " << "Dealer score: " << getScore(*dealerHand) << endl << endl; 

    //Output game results and return approptiate value
    if (getScore(*dealerHand) > getScore(*playerHand)){
        if (getScore(*dealerHand) == BLACKJACK)
            cout << "The dealer has reached 21. You lose." << endl << endl;
        else 
            cout << "The dealer has scored higher than you. You lose." << endl << endl;
        delete[] (*playerHand).cards;
        delete playerHand;
        delete[] (*dealerHand).cards;
        delete dealerHand;
        return -1;
    }
    else if (getScore(*dealerHand) < getScore(*playerHand)){
        if (getScore(*playerHand) == BLACKJACK)
            cout << "You have reached 21. Blackjack! You win!" << endl << endl;
        else 
            cout << "You scored higer than the dealer. You win!" << endl << endl;
        delete[] (*playerHand).cards;
        delete playerHand;
        delete[] (*dealerHand).cards;
        delete dealerHand;
        return 1;
    }
    else {
        if (getScore(*playerHand) == getScore(*dealerHand) == BLACKJACK)
            cout << "You and the dealer have both reached 21. The game is a draw." << endl << endl;
        else 
            cout << "You and the dealer got the same score. The game is a draw." << endl << endl;
        delete[] (*playerHand).cards;
        delete playerHand;
        delete[] (*dealerHand).cards;
        delete dealerHand;
        return 0;
    }
}

//PART 3

//Checks user input when they choose whether or not to play 
char playOrNot (){
    char cInput = 'a';
    string sInput;
    cout << "Please enter your choice. (Press y to play, any other letter to quit): ";
    cin >> sInput;
    cInput = tolower(sInput[0]);
    while (sInput.length() != 1){
            cin.clear();
            cin.ignore (1000, '\n');
            cout << "(Press y to play, any other letter to quit): ";
            cin >> sInput;
            cInput = tolower(sInput[0]);
        }
    return cInput;
}

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
    cout << "Do you want to play a hand of Blackjack?" << endl;
    anotherRound = playOrNot();
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