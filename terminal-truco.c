#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#define HAND_SIZE 3
#define DIFFERENT_CARDS 10
#define RANDOM_INDEX rand() % DIFFERENT_CARDS
#define RANDOM_HAND_CARD (rand() % 3)
#define DECK_SIZE 40

typedef struct
{
    int cardNumber;
    char cardSuit;
    int hierarchy;
    bool isPlayed;
    int envidoValue;
} trucoCard;

trucoCard hand[HAND_SIZE] = {};
trucoCard oppHand[HAND_SIZE] = {};
int currentHandSize = HAND_SIZE;
int possibleNumbers[DIFFERENT_CARDS] = {1, 2, 3, 4, 5, 6, 7, 10, 11, 12};

char possibleSuits[4] = {'P', 'O', 'E', 'C'};
trucoCard deck[DECK_SIZE] = {};
trucoCard nullCard;

int playerPoints = 0;
int aiPoints = 0;

int playerCounter = 0;
int aiCounter = 0;

bool wasEnvidoAsked;

int roundValue = 1;
int cardsPlayed = 0;

bool youPlayNext;


int flipCoin () 
{
    int coin = (rand() % 2) + 1;
    return coin;
}

int generateHierarchy(int cardNumber, char cardSuit)
{
    int hierarchy = 0;

    if (cardNumber == 3)
    {
        hierarchy = 10;
    }

    else if (cardNumber == 2)
    {
        hierarchy = 9;
    }

    else if (cardNumber == 1)
    {
        hierarchy = 8;

        if (cardSuit == 'E')
        {
            hierarchy = 14;
        }
        if (cardSuit == 'P')
        {
            hierarchy = 13;
        }
    }

    else if (cardNumber == 12)
    {
        hierarchy = 7;
    }

    else if (cardNumber == 11)
    {
        hierarchy = 6;
    }

    else if (cardNumber == 10)
    {
        hierarchy = 5;
    }

    else if (cardNumber == 7)
    {
        hierarchy = 4;
        if (cardSuit == 'O')
        {
            hierarchy = 11;
        }
        if (cardSuit == 'E')
        {
            hierarchy = 12;
        }   
    }

    else if (cardNumber == 6)
    {
        hierarchy = 3;
    }

    else if (cardNumber == 5)
    {
        hierarchy = 2;
    }

    else if (cardNumber == 4)
    {
        hierarchy = 1;
    }
    return hierarchy;
}

int generateEnvidoValues(int number)
{
    int envidoValue;
    if (number >= 10)
    {
        envidoValue = (number - 10);
    }
    else {envidoValue = number;}
    return envidoValue;
}

void generateDeck()
{
    for (int i = 0; i < 10; i++)
    {
        deck[i].cardNumber = possibleNumbers[i];
        deck[i].cardSuit = possibleSuits[0];
        deck[i].hierarchy = generateHierarchy(deck[i].cardNumber, deck[i].cardSuit);
        deck[i].isPlayed = false;
        deck[i].envidoValue = generateEnvidoValues(deck[i].cardNumber);
    }

    for (int i = 10; i < 20; i++)
    {
        deck[i].cardNumber = possibleNumbers[i - 10];
        deck[i].cardSuit = possibleSuits[1];
        deck[i].hierarchy = generateHierarchy(deck[i].cardNumber, deck[i].cardSuit);
        deck[i].isPlayed = false;
        deck[i].envidoValue = generateEnvidoValues(deck[i].cardNumber);
    }

    for (int i = 20; i < 30; i++)
    {
        deck[i].cardNumber = possibleNumbers[i - 20];
        deck[i].cardSuit = possibleSuits[2];
        deck[i].hierarchy = generateHierarchy(deck[i].cardNumber, deck[i].cardSuit);
        deck[i].isPlayed = false;
        deck[i].envidoValue = generateEnvidoValues(deck[i].cardNumber);
    }

    for (int i = 30; i < 40; i++)
    {
        deck[i].cardNumber = possibleNumbers[i - 30];
        deck[i].cardSuit = possibleSuits[3];
        deck[i].hierarchy = generateHierarchy(deck[i].cardNumber, deck[i].cardSuit);
        deck[i].isPlayed = false;
        deck[i].envidoValue = generateEnvidoValues(deck[i].cardNumber);
    }
}

void shuffleDeck()
{
    for (int i = DECK_SIZE - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        trucoCard temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void drawCards()
{
    for (int i = 0; i < 3; i++)
    {
        hand[i] = deck[i];
    }
    for (int j = 3; j < 6; j++)
    {
        oppHand[j - 3] = deck[j];
    }
}

void showCards() 
{
    printf("\nSuas cartas: ");
    for (int i = 0; i < currentHandSize; i++)
    {
        printf("%d%c ", hand[i].cardNumber, hand[i].cardSuit);
    }
    printf("\nCartas do oponente (para testes): ");
    for (int j = 0; j < currentHandSize; j++)
    {
        printf("%d%c ", oppHand[j].cardNumber, oppHand[j].cardSuit);
    }
    printf("\n");
}

int compareCards(trucoCard playerCard, trucoCard aiCard)
{
    if (playerCard.hierarchy == aiCard.hierarchy)
    {
        playerCounter += 1;
        aiCounter += 1;
        return 0;
    }
    if (playerCard.hierarchy > aiCard.hierarchy)
    {
        playerCounter += 1;
        return 1;
    }
    if (playerCard.hierarchy < aiCard.hierarchy)
    {
        aiCounter += 1;
        return 2;
    }

    return 3;
}

void addPoints() 
{
    if (playerCounter >= 2 && playerCounter != aiCounter)
    {
        playerPoints += roundValue;
    }
    if (aiCounter >= 2 && playerCounter != aiCounter)
    {
        aiPoints += roundValue;
    }
}

void showPoints()
{
    printf("\n-------------PONTOS-------------");
    printf("\nTu: %d", playerPoints);
    printf("\nOponente: %d", aiPoints);
    printf("\n--------------------------------");
}

void resetCards() 
{
    for (int i = 0; i < 3; i++) 
    {
        hand[i].isPlayed = false;
    }
}

bool checkFlor()
{
    if (hand[0].cardSuit == hand[1].cardSuit &&  hand[1].cardSuit == hand[2].cardSuit)
    {
        printf("\nTU: Que FLOR cheirosa!");
        playerPoints += 3;
        showPoints();
        return true;
    }
    if (oppHand[0].cardSuit == oppHand[1].cardSuit &&  oppHand[1].cardSuit == oppHand[2].cardSuit)
    {
        printf("\nOPONENTE: FLOR... BIP... BOP...");
        aiPoints += 3;
        showPoints();
        return true;
    }
    return false;
}

void startNewRound()
{
    playerCounter = 0;
    aiCounter = 0;  
    cardsPlayed = 0;
    roundValue = 1;
    wasEnvidoAsked = false;
    resetCards();
    shuffleDeck();
    drawCards();
    showCards();
    checkFlor();
}

int calculateEnvido() 
{ 
    int playerSum;
    int aiSum;

    if (hand[0].cardSuit == hand[1].cardSuit) 
    {
        playerSum = hand[0].envidoValue + hand[1].envidoValue + 20;
    }
    if (hand[0].cardSuit == hand[2].cardSuit) 
    {
        playerSum = hand[0].envidoValue + hand[2].envidoValue + 20;
    }
    if (hand[1].cardSuit == hand[2].cardSuit) 
    {
        playerSum = hand[1].envidoValue + hand[2].envidoValue + 20;
    }

    if (oppHand[0].cardSuit == oppHand[1].cardSuit) 
    {
        aiSum = oppHand[0].envidoValue + oppHand[1].envidoValue + 20;
    }
    if (oppHand[0].cardSuit == oppHand[2].cardSuit) 
    {
        aiSum = oppHand[0].envidoValue + oppHand[2].envidoValue + 20;
    }
    if (oppHand[1].cardSuit == oppHand[2].cardSuit) 
    {
        aiSum = oppHand[1].envidoValue + oppHand[2].envidoValue + 20;
    }

    if (playerSum > aiSum) 
    {
        return 1;
    }
    else if (playerSum < aiSum) 
    {
        return 2;
    }
    else if (playerSum == aiSum) 
    {
        return 3;
    }
    return 0;
}

void playerAskEnvido()
{
    if (checkFlor() == true) 
    {
        printf("\nERRO. FLOR CANTADA.");
    }
    else if (wasEnvidoAsked == true)
    {
        printf("\nDE NOVO NAO");
    }

    else if (wasEnvidoAsked == false)
    {
        printf("\nTU: ENVIDO.");
        int oppChoice = (rand() % 2) + 1;
        if (oppChoice == 1) 
        {
            printf("\nOPONENTE: QUERO."); 
            if (calculateEnvido() == 1)
            {
                playerPoints += 2;
                showPoints();
            }
            if (calculateEnvido() == 2)
            {
                aiPoints += 2;
                showPoints();
            }
            if (calculateEnvido() == 3)
            {
                aiPoints += 2;
                showPoints();
            }
        }
        if (oppChoice == 2) {printf("\nOPONENTE: NAO QUERO..."); playerPoints += 1; showPoints();}
    }
}

int aiAskEnvido()
{
    if (checkFlor() == true) 
    {
        return 0;
    }

    printf("\nOPONENTE: ENVIDO.");
    printf("\n1. QUERO // 2. NAO QUERO\n");

    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        printf("\nTU: QUERO."); 
        if (calculateEnvido() == 1)
        {
            playerPoints += 2;
            showPoints();
        }
        if (calculateEnvido() == 2)
        {
            aiPoints += 2;
            showPoints();
        }
        if (calculateEnvido() == 3)
        {
            aiPoints += 2;
            showPoints();
        }
    }
    if (choice == 2) 
    {
        printf("\nTU: NAO QUERO..."); 
        aiPoints += 1;
        showPoints();
    } 
    return 0;
}

trucoCard playerPlayCard()
{
    int choice;

    printf("\nJOGUE UMA CARTA: 1. %d%c ", hand[0].cardNumber, hand[0].cardSuit);
    printf("2. %d%c ", hand[1].cardNumber, hand[1].cardSuit);
    printf("3. %d%c ", hand[2].cardNumber, hand[2].cardSuit);
    printf("4. Envido\n");
    scanf(" %d", &choice);

    if (choice == 4)
    {
        playerAskEnvido();
        wasEnvidoAsked = true;
    }

    while(hand[choice - 1].isPlayed)
    {
        printf("\nJOGUE OUTRA CARTA: ");
        scanf(" %d", &choice);
    }

    printf("\nTU: %d", hand[choice - 1].cardNumber);
    printf("%c", hand[choice - 1].cardSuit);

    hand[choice - 1].isPlayed = true;
    cardsPlayed += 1;

    return hand[choice - 1];
}

trucoCard aiPlayCard()
{
    int chanceToAskEnvido = rand() % 100;
    if (chanceToAskEnvido <= 15)
    {
        aiAskEnvido();     
        wasEnvidoAsked = true;
    }
    
    int randomIndex = RANDOM_HAND_CARD;

    while (oppHand[randomIndex].isPlayed)
    {
        randomIndex = RANDOM_HAND_CARD;
    }

    printf("\nOPONENTE: %d", oppHand[randomIndex].cardNumber);
    printf("%c", oppHand[randomIndex].cardSuit);

    oppHand[randomIndex].isPlayed = true;
    cardsPlayed += 1;

    return oppHand[randomIndex];
}


int main()
{   
    srand(time(NULL));
    generateDeck();

    int pointsToWin = 15;
    int coin = flipCoin();

    if (coin == 1) 
    {
        printf("\nTU JOGA PRIMEIRO.");
        youPlayNext = true;
    } else if (coin == 2) 
    {
        printf("\nA IA JOGA PRIMEIRO.");
        youPlayNext = false;
    }

    bool whoWentFirst = youPlayNext;

    int mainLoop = 1;
    startNewRound();

    while (mainLoop == 1) 
    {
        if (youPlayNext == false) 
        {   
            trucoCard aiCard = aiPlayCard();
            trucoCard playerCard = playerPlayCard();

            int result = compareCards(playerCard, aiCard);
            switch (result)
            {
                case 0: 
                    youPlayNext = false;
                    break;
                
                case 1:
                    youPlayNext = true;
                    break;
                
                case 2:
                    youPlayNext = false;
                    break;
                
            }

            if (playerPoints >= pointsToWin)
            {
                printf("\nTU VENCEU!");
                return 0;
            }

            if (aiPoints >= pointsToWin)
            {
                printf("\nOPONENTE VENCEU!");
                return 0;
            }

            if (cardsPlayed == 6 || aiCounter == 2 || playerCounter == 2) 
            {
                
                addPoints();
                sleep(1); 
                showPoints();
                if (playerPoints >= pointsToWin)
                {
                    printf("\nTU VENCEU!");
                    mainLoop = 0;
                }

                if (aiPoints >= pointsToWin)
                {
                    printf("\nOPONENTE VENCEU!");
                    mainLoop = 0;
                }
                youPlayNext = !(whoWentFirst);

                startNewRound();
            }
        }

        if (youPlayNext == true)
        {
            trucoCard playerCard = playerPlayCard();
            trucoCard aiCard = aiPlayCard();

            int result = compareCards(playerCard, aiCard);
            switch (result)
            {
                case 0: 
                    youPlayNext = true;
                    break;
                
                case 1:
                    youPlayNext = true;
                    break;
                
                case 2:
                    youPlayNext = false;
                    break;
            }

            if (cardsPlayed == 6 || aiCounter == 2 || playerCounter == 2)  
            {
                addPoints();
                sleep(1); 
                showPoints();
                if (playerPoints >= pointsToWin)
                {
                    printf("\nTU VENCEU!");
                    mainLoop = 0;
                }

                if (aiPoints >= pointsToWin)
                {
                    printf("\nOPONENTE VENCEU!");
                    mainLoop = 0;
                }

                youPlayNext = !(whoWentFirst);

                startNewRound();
            }
        } 
    } 
   return 0;
}  