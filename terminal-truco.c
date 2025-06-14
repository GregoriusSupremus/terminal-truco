#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define HAND_SIZE 3
#define DIFFERENT_CARDS 10
#define RANDOM_INDEX rand() % DIFFERENT_CARDS
#define RANDOM_HAND_CARD (rand() % 3)
#define DECK_SIZE 40

typedef struct {
    int cardNumber;
    char cardSuit;
    int hierarchy;
    bool isPlayed;
} trucoCard;

trucoCard hand[HAND_SIZE] = {};
trucoCard oppHand[HAND_SIZE] = {};
int currentHandSize = HAND_SIZE;
int possibleNumbers[DIFFERENT_CARDS] = {1, 2, 3, 4, 5, 6, 7, 10, 11, 12};
int cardsPlayed = 0;
char possibleSuits[4] = {'P', 'O', 'E', 'C'};
trucoCard deck[DECK_SIZE] = {};
trucoCard nullCard;

int playerPoints = 0;
int aiPoints = 0;

int playerCounter = 0;
int aiCounter = 0;
    
int roundValue = 1;

int generateHierarchy(int cardNumber, char cardSuit) {

    int hierarchy;

    if (cardNumber == 3) {
        hierarchy = 10;
    }

    if (cardNumber == 2) {
        hierarchy = 9;
    }

    if (cardNumber == 1) {
        hierarchy = 8;
        if (cardSuit == 'E') {hierarchy = 14;}
        if (cardSuit == 'C') {hierarchy = 13;}
    }

    if (cardNumber == 12) {
        hierarchy = 7;
    }

    if (cardNumber == 11) {
        hierarchy = 6;
    }

    if (cardNumber == 10) {
        hierarchy = 5;
    }

    if (cardNumber == 7) {
        hierarchy = 4;
        if (cardSuit == 'O') {hierarchy = 11;}
        if (cardSuit == 'C') {hierarchy = 12;}
    }

    if (cardNumber == 6) {
        hierarchy = 3;
    }

    if (cardNumber == 5) {
        hierarchy = 2;
    }

    if (cardNumber == 4) {
        hierarchy = 1;
    }
    return hierarchy;
}

void generateDeck() {
    
    for (int i = 0; i < 10; i++) {
        deck[i].cardNumber = possibleNumbers[i];
        deck[i].cardSuit = possibleSuits[0];
        deck[i].hierarchy = generateHierarchy(deck[i].cardNumber, deck[i].cardSuit); 
        deck[i].isPlayed = false;
    }

    for (int i = 10; i < 20; i++) {
        deck[i].cardNumber = possibleNumbers[i-10];
        deck[i].cardSuit = possibleSuits[1];
        deck[i].hierarchy = generateHierarchy(deck[i].cardNumber, deck[i].cardSuit); 
        deck[i].isPlayed = false;
    }

    for (int i = 20; i < 30; i++) {
        deck[i].cardNumber = possibleNumbers[i-20];
        deck[i].cardSuit = possibleSuits[2];
        deck[i].hierarchy = generateHierarchy(deck[i].cardNumber, deck[i].cardSuit); 
        deck[i].isPlayed = false;
    }

    for (int i = 30; i < 40; i++) {
        deck[i].cardNumber = possibleNumbers[i-30];
        deck[i].cardSuit = possibleSuits[3];
        deck[i].hierarchy = generateHierarchy(deck[i].cardNumber, deck[i].cardSuit); 
        deck[i].isPlayed = false;
    }
    
}

void shuffleDeck() {
    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        trucoCard temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void drawCards() {
    for (int i = 0; i < 3; i++) {
        hand[i] = deck[i];
    }

    for (int j = 3; j < 6; j++) {
        oppHand[j-3] = deck[j];
    }

    printf("\nSuas cartas: "); 
    for (int i = 0; i < currentHandSize; i++) {
        printf("%d%c ", hand[i].cardNumber, hand[i].cardSuit);
    }

    printf("\nCartas do oponente (para testes): "); 
    for (int j = 0; j < currentHandSize; j++) {
        printf("%d%c ", oppHand[j].cardNumber, oppHand[j].cardSuit);
    }
    printf("\n"); 
}  

int compareCards(trucoCard playerCard, trucoCard aiCard) {

    if (playerCard.hierarchy == aiCard.hierarchy) {
        playerCounter += 1;
        aiCounter += 1;
    } 
    if (playerCard.hierarchy > aiCard.hierarchy) {
        playerCounter += 1;
    }
    
    if (playerCard.hierarchy < aiCard.hierarchy) {
        aiCounter += 1;
    }
    return 0;
}


int addPoints() {
    
    if (playerCounter == 2) {
        playerPoints += roundValue;
    }

    if (aiCounter == 2) {
        aiPoints += roundValue;
    }
    return 0;
}

int main() {

    int on = 1;
    int console;

    srand(time(NULL));
    generateDeck();
    shuffleDeck();

    printf("\nGUIA:\n0. FECHAR PROGRAMA\n1. JOGAR PRIMEIRA CARTA\n2. JOGAR SEGUNDA CARTA\n3. JOGAR TERCEIRA CARTA\n4. VER DECK INCIAL\n");
    
    drawCards();
    
    while (on == 1) {
        printf("\n\nCONSOLE: ");
        scanf("%d", &console);

        if (console == 0) {
            printf("\nPROGRAMA ENCERRADO.\n");
            on = 0;
        } 

        if (console == 1) {
            int randomIndex = RANDOM_HAND_CARD;
        
            if (hand[0].isPlayed) {
                printf("Jogue outra carta."); 
            }
            else {
                printf("\nTu jogou: %d%c", hand[0].cardNumber, hand[0].cardSuit);
                hand[0].isPlayed = true;

                while(oppHand[randomIndex].isPlayed) {randomIndex = RANDOM_HAND_CARD;}
                printf("\nOponente jogou: %d%c", oppHand[randomIndex].cardNumber, oppHand[randomIndex].cardSuit);
                oppHand[randomIndex].isPlayed = true;
                compareCards(hand[0], oppHand[randomIndex]);
                cardsPlayed += 1;
            }
        }
            
        if (console == 2) {
            int randomIndex = RANDOM_HAND_CARD;
        
            if (hand[1].isPlayed) {
                printf("Jogue outra carta."); 
            }
            else {
                printf("\nTu jogou: %d%c", hand[1].cardNumber, hand[1].cardSuit);
                hand[1].isPlayed = true;

                while(oppHand[randomIndex].isPlayed) {randomIndex = RANDOM_HAND_CARD;}
                printf("\nOponente jogou: %d%c", oppHand[randomIndex].cardNumber, oppHand[randomIndex].cardSuit);
                oppHand[randomIndex].isPlayed = true;
                compareCards(hand[1], oppHand[randomIndex]);
                cardsPlayed += 1;
            }
        }

        if (console == 3) {
            int randomIndex = RANDOM_HAND_CARD;
        
            if (hand[2].isPlayed) {
                printf("Jogue outra carta."); 
            }
            else {
                printf("\nTu jogou: %d%c", hand[2].cardNumber, hand[2].cardSuit);
                hand[2].isPlayed = true;

                while(oppHand[randomIndex].isPlayed) {randomIndex = RANDOM_HAND_CARD;}
                printf("\nOponente jogou: %d%c", oppHand[randomIndex].cardNumber, oppHand[randomIndex].cardSuit);
                oppHand[randomIndex].isPlayed = true;
                compareCards(hand[2], oppHand[randomIndex]);
                cardsPlayed += 1;
            }
        }

        if (cardsPlayed == 3) {
            addPoints();
            printf("\n\nPONTOS");
            printf("\nTu: %d", playerPoints);
            printf("\nOponente: %d", aiPoints);
            playerCounter = 0;
            aiCounter = 0;
        }

        if (console == 4) {
            for (int i = 0; i < DECK_SIZE; i++) {
                printf("%d", deck[i].cardNumber);
                printf("%c \n", deck[i].cardSuit);
            }
        }
    }
}