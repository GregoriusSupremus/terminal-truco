#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    int cardNumber;
    char cardSuit;
    int hierarchy;
    bool isPlayed;
} trucoCard;

trucoCard hand[3] = {};
trucoCard oppHand[3] = {};

void playCard(int index) {
    hand[index].isPlayed = true;
}

void compareCards(int playerHierarchy, int aiHierarchy) {

}

int main(void) 
{
   hand[0].cardNumber = 1; 
   hand[1].cardNumber = 2; 
   hand[2].cardNumber = 3; 
   
   hand[0].cardSuit = 'o';
   hand[1].cardSuit = 'p';
   hand[2].cardSuit = 'e';

   hand[0].isPlayed = false;
   hand[1].isPlayed = false;
   hand[2].isPlayed = false;

   hand[0].hierarchy = 8;
   hand[1].hierarchy = 9;
   hand[2].hierarchy = 10;

   oppHand[0].cardNumber = 7; 
   oppHand[1].cardNumber = 3; 
   oppHand[2].cardNumber = 9; 
   
   oppHand[0].cardSuit = 'e';
   oppHand[1].cardSuit = 'c';
   oppHand[2].cardSuit = 'o';

   oppHand[0].isPlayed = false;
   oppHand[1].isPlayed = false;
   oppHand[2].isPlayed = false;

   oppHand[0].hierarchy = 14;
   oppHand[1].hierarchy = 9;
   oppHand[2].hierarchy = 4;
}