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
#define ENVIDO_NOT_PLAYED (aiAskedEnvido == false && aiAskedRealEnvido == false && playerAskedEnvido == false && playerAskedRealEnvido == false)


typedef struct
{
    int cardNumber;
    int hierarchy;
    int envidoValue;
    char cardSuit;
    bool isPlayed;
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

int playerEnvido;
int aiEnvido;

int roundValue = 1;
int cardsPlayed = 0;
int roundsCounter = 1;

bool wasFlorPlayed;

bool playerAskedEnvido;
bool playerAskedRealEnvido;

bool aiAskedEnvido;
bool aiAskedRealEnvido;

bool playerAskedTruco;
bool aiAskedTruco;

bool youPlayNext;

//PROTOTYPES
void clearScreen();
void showCards();
void showPoints();
void showEnvido();
int flipCoin();
int generateHierarchy(int cardNumber, char cardSuit);
int generateEnvidoValues(int number);
void generateDeck();
void shuffleDeck();
void drawCards();
int compareCards(trucoCard playerCard, trucoCard aiCard);
void addPoints();
void resetCards();
void checkFlor();
void startNewRound();
int calculatePlayerEnvido();
int calculateAiEnvido();
trucoCard playerPlayCard();
int playerHighCard();
void playerAskEnvido();
void playerAskRealEnvido();
void playerAskTruco();
void playerSpecialRule();
trucoCard aiPlayCard();
int aiHighCard();
void aiAskEnvido();
void aiAskRealEnvido();
void aiAskTruco();

//UI
void clearScreen()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
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
    printf("\n---------------------------------------");
}

void showPoints()
{
    printf("\n-------------PONTOS-------------");
    printf("\nTu: %d", playerPoints);
    printf("\nOponente: %d", aiPoints);
}

void showEnvido() 
{
    printf("\n------------ENVIDO------------\n");
    printf("TU: %d\n", playerEnvido);
    printf("OPONENTE: %d\n", aiEnvido);
}

//SYSTEM
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
        else if (cardSuit == 'P')
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
        else if (cardSuit == 'E')
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
    
    else if (playerCounter >= 2 && playerCounter == aiCounter)
    {
        if (youPlayNext == true)
        {
            playerPoints += roundValue;
        }
        else {aiPoints += roundValue;}
    }

    else if (aiCounter >= 2 && playerCounter != aiCounter)
    {
        aiPoints += roundValue;
    }
}

void resetCards() 
{
    for (int i = 0; i < 3; i++) 
    {
        hand[i].isPlayed = false;
    }
}

void checkFlor()
{
    if (hand[0].cardSuit == hand[1].cardSuit &&  hand[1].cardSuit == hand[2].cardSuit)
    {
        wasFlorPlayed = true;
        
        printf("\nTU: Que FLOR cheirosa!");
        playerPoints += 3;
        showPoints();
    }
    else if (oppHand[0].cardSuit == oppHand[1].cardSuit &&  oppHand[1].cardSuit == oppHand[2].cardSuit)
    {
        wasFlorPlayed = true;
        
        printf("\nOPONENTE: FLOR... BIP... BOP...");
        aiPoints += 3;
        showPoints();
    }
}

void startNewRound()
{
    printf("\n----------RODADA %d-------------", roundsCounter);
    playerCounter = 0;
    aiCounter = 0;  
    cardsPlayed = 0;
    roundValue = 1;
    aiAskedEnvido = false;
    aiAskedRealEnvido = false;
    wasFlorPlayed = false;
    playerAskedTruco = false;
    aiAskedTruco = false;
    resetCards();
    shuffleDeck();
    drawCards();
    showCards();
    checkFlor();
    roundsCounter++;
}

int calculatePlayerEnvido() 
{ 
    if (hand[0].cardSuit == hand[1].cardSuit) 
    {
        playerEnvido = hand[0].envidoValue + hand[1].envidoValue + 20;
    }
    else if (hand[0].cardSuit == hand[2].cardSuit) 
    {
        playerEnvido = hand[0].envidoValue + hand[2].envidoValue + 20;
    }
    else if (hand[1].cardSuit == hand[2].cardSuit) 
    {
        playerEnvido = hand[1].envidoValue + hand[2].envidoValue + 20;
    }
    else 
    {
        playerEnvido = playerHighCard();
    }
    return playerEnvido;
}

int calculateAiEnvido()
{
    if (oppHand[0].cardSuit == oppHand[1].cardSuit) 
    {
        aiEnvido = oppHand[0].envidoValue + oppHand[1].envidoValue + 20;
    }
    else if (oppHand[0].cardSuit == oppHand[2].cardSuit) 
    {
        aiEnvido = oppHand[0].envidoValue + oppHand[2].envidoValue + 20;
    }
    else if (oppHand[1].cardSuit == oppHand[2].cardSuit) 
    {
        aiEnvido = oppHand[1].envidoValue + oppHand[2].envidoValue + 20;
    }
    else 
    {
        aiEnvido = aiHighCard();
    }
    return aiEnvido;
}

void envidoResult(int points)
{
    if (calculatePlayerEnvido() > calculateAiEnvido())
    {
        playerPoints += points;
        showEnvido();
        showPoints();
    }
    else if (calculatePlayerEnvido() < calculateAiEnvido())
    {
        aiPoints += points;
        showEnvido();
        showPoints();
    }
    else
    {
        if (youPlayNext) {playerPoints += points;}
        else {aiPoints += points;}
        showEnvido();
        showPoints();
    }
}

//PLAYER
trucoCard playerPlayCard()
{
    char buffer[100];
    int choice;

    while (true)
    {
        printf("\n------------JOGUE UMA CARTA------------\n");
        
        for (int i = 0; i < HAND_SIZE; i++)
        {
            if (hand[i].isPlayed == false)
            {
                printf("|%d- %d%lc| ", (i + 1), hand[i].cardNumber, hand[i].cardSuit);
            }
        }
        printf("|4- Regra Especial|\n");

        if (fgets(buffer, sizeof(buffer), stdin))
        {
            if (sscanf(buffer, "%d", &choice) != 1) 
            {
                printf("VALOR INVALIDO.\n");
            }
            else if (hand[choice - 1].isPlayed == false && choice != 4 && choice < 4 && choice > 0)
            {
                break;
            } 
            else if (choice == 4)
            {
                playerSpecialRule();
            }
        }
    }

    sleep(1);
    printf("\n---------------------------------------\n");
    printf("TU: %d%c", hand[choice - 1].cardNumber, hand[choice - 1].cardSuit);
    printf("\n---------------------------------------");

    hand[choice - 1].isPlayed = true;
    cardsPlayed += 1;

    return hand[choice - 1];
}

int playerHighCard()
{
    int bigger = 0;
    for (int i = 0; i < 3; i++)
    {
        if (hand[i].cardNumber > bigger && hand[i].cardNumber < 10)
        {
            bigger = hand[i].cardNumber;
        }
    }
    return bigger;
}

void playerAskEnvido()
{
    printf("\nTU: ENVIDO.");
    int oppChoice = (rand() % 3) + 1;
    if (oppChoice == 1) 
    {   
        printf("\nOPONENTE: QUERO."); 
        envidoResult(2);
    }
    if (oppChoice == 2) 
    {
        printf("\nOPONENTE: NAO QUERO..."); 
        playerPoints += 1; 
        showPoints();
    }

    if (oppChoice == 3)
    {
        printf("\nOPONENTE: REAL ENVIDO"); 
        printf("\n1. QUERO || 2. NAO QUERO\n");

        char buffer[100];
        int choice;

        while (true)
        {
            if (fgets(buffer, sizeof(buffer), stdin))
            {
                if (sscanf(buffer, "%d", &choice) != 1) 
                {
                    printf("VALOR INVALIDO.\n");
                }
                else if (choice == 1)
                {
                    printf("\nTU: QUERO."); 
                    envidoResult(5);
                    break;
                } 
                else if (choice == 2)
                {
                    playerSpecialRule();
                    printf("\nTU: NAO QUERO..."); 
                    aiPoints += 2;
                    showPoints();
                }
            }
        }
    }
}

void playerAskRealEnvido()
{
    printf("\nTU: REAL ENVIDO.");
    int oppChoice = (rand() % 2) + 1;
    if (oppChoice == 1) 
    {   
        printf("\nOPONENTE: QUERO."); 
        envidoResult(3);
    }
    if (oppChoice == 2) 
    {
        printf("\nOPONENTE: NAO QUERO..."); 
        playerPoints += 1; 
        showPoints();
    }
}

void playerAskTruco() 
{
    int oppChoice = (rand() % 2) + 1;
    if (oppChoice == 1) 
    {
        printf("\nOPONENTE: QUERO."); 
        roundValue = 2;
    }
    if (oppChoice == 2) 
    {
        clearScreen();
        printf("\nOPONENTE: NAO QUERO..."); 
        playerPoints += 1; 
        showPoints();
        startNewRound();
    }
}

void playerSpecialRule()
{
    char buffer[100];
    int choice = 0;

    printf("| 1- Truco | \n");
    printf("| 4- Envido | 5- Real Envido |\n");

    while (true)
    {
        if (fgets(buffer, sizeof(buffer), stdin))
        {
            if (sscanf(buffer, "%d", &choice) != 1) 
            {
                printf("VALOR INVALIDO.\n");
            }
            else if (choice == 1)
            {
                if (playerAskedTruco || aiAskedTruco)
                {
                    printf("TRUCO JA CANTADO. TENTE PEDIR RETRUCO.");
                }
                else
                {
                    playerAskTruco();
                    playerAskedTruco = true;
                }
                break;
            } 
            else if (choice == 4)
            {
                if (wasFlorPlayed) 
                {
                    printf("\nERRO. FLOR CANTADA.");
                }
                else if (aiAskedEnvido || aiAskedRealEnvido || playerAskedEnvido || playerAskedRealEnvido)
                {
                    printf("\nENVIDO JA CANTADO\n");
                }
                else 
                {
                    playerAskEnvido();
                    aiAskedEnvido = true;
                    break;
                }
                break;
            }

            else if (choice == 5)
            {
                if (wasFlorPlayed == true) 
                {
                    printf("\nERRO. FLOR CANTADA.");
                }
                else if (playerAskedEnvido || aiAskedRealEnvido || aiAskedEnvido)
                {
                    printf("\nERRO. TENTE ALGO DIFERENTE.");
                }
                else 
                {
                    playerAskRealEnvido();
                    playerAskedRealEnvido = true;
                }
                break;
            }
        }
    }
}

//AI
trucoCard aiPlayCard()
{
    int chanceToAskRule = rand() % 1000;

    if (chanceToAskRule <= 200 && wasFlorPlayed == false && ENVIDO_NOT_PLAYED)
    {
        aiAskRealEnvido();
        aiAskedRealEnvido = true;
    }
    else if (chanceToAskRule > 201 && chanceToAskRule <= 250 && wasFlorPlayed == false && ENVIDO_NOT_PLAYED)
    {
        aiAskRealEnvido();
        aiAskedRealEnvido = true;
    }

    else if (chanceToAskRule > 251 && chanceToAskRule <= 600 && aiAskedTruco == false && playerAskedTruco == false)
    {
        aiAskTruco();
        aiAskedTruco = true;
    }

    int randomIndex = RANDOM_HAND_CARD;

    while (oppHand[randomIndex].isPlayed)
    {
        randomIndex = RANDOM_HAND_CARD;
    }

    sleep(1);
    printf("\n---------------------------------------\n");
    printf("OPONENTE: %d", oppHand[randomIndex].cardNumber);
    printf("%lc", oppHand[randomIndex].cardSuit);
    printf("\n---------------------------------------");

    oppHand[randomIndex].isPlayed = true;
    cardsPlayed += 1;

    return oppHand[randomIndex];
}

int aiHighCard()
{
    int bigger = 0;
    for (int i = 0; i < 3; i++)
    {
        if (oppHand[i].cardNumber > bigger)
        {
            bigger = oppHand[i].cardNumber;
        }
    }
    if (bigger > 9)
    {
        bigger-= 10;
    }
    return bigger;
}

void aiAskEnvido()
{
    printf("\nOPONENTE: ENVIDO.");
    printf("\n1. QUERO || 2. NAO QUERO || 3. REAL ENVIDO\n");

    int choice;
    char buffer[100];

    while (true)
    {
        if (fgets(buffer, sizeof(buffer), stdin))
        {
            if (sscanf(buffer, "%d", &choice) != 1) 
            {
                printf("VALOR INVALIDO.\n");
            }
            else if (choice == 1)
            {
                printf("\nTU: QUERO."); 
                envidoResult(2);
                break;
            } 
            else if (choice == 2)
            {
                printf("\nTU: NAO QUERO..."); 
                aiPoints += 1;
                showPoints();
                break;
            }
            else if (choice == 3)
            {
                printf("\nTU: REAL ENVIDO.");
                int oppChoice = (rand() % 2) + 1;
                if (oppChoice == 1) 
                {   
                    printf("\nOPONENTE: QUERO."); 
                    envidoResult(5);
                    break;
                }
                if (oppChoice == 2) 
                {
                    printf("\nOPONENTE: NAO QUERO..."); 
                    playerPoints += 2; 
                    showPoints();
                    break;
                }
            }
        }
    }
}

void aiAskRealEnvido()
{
    printf("\nOPONENTE: REAL ENVIDO.");
    printf("\n1. QUERO || 2. NAO QUERO\n");

    int choice;
    char buffer[100];

    while (true)
    {
        if (fgets(buffer, sizeof(buffer), stdin))
        {
            if (sscanf(buffer, "%d", &choice) != 1) 
            {
                printf("VALOR INVALIDO.\n");
            }
            else if (choice == 1)
            {
                printf("\nTU: QUERO."); 
                envidoResult(3);
                break;
            } 
            else if (choice == 2)
            {
                printf("\nTU: NAO QUERO..."); 
                aiPoints += 1;
                showPoints();
                break;
            }
        }
    }
}

void aiAskTruco() 
{
    printf("\nOPONENTE: TRUCO.");
    printf("\n1. QUERO || 2. NAO QUERO\n");

    int choice;
    char buffer[100];

    while (true)
    {
        if (fgets(buffer, sizeof(buffer), stdin))
        {
            if (sscanf(buffer, "%d", &choice) != 1) 
            {
                printf("VALOR INVALIDO.\n");
            }
            else if (choice == 1)
            {
                printf("\nTU: QUERO."); 
                roundValue = 2;
                break;
            } 
            else if (choice == 2)
            {
                printf("\nTU: NAO QUERO..."); 
                aiPoints += 1;
                showPoints();
                startNewRound();
                break;
            }
        }
    }
}

//MAIN
int main()
{   
    srand(time(NULL));
    generateDeck();

    int pointsToWin = 15;
    int coin = flipCoin();

    if (coin == 1) 
    {
        printf("\nTU JOGA PRIMEIRO.\n");
        youPlayNext = true;
    } else if (coin == 2) 
    {
        printf("\nOPONENTE JOGA PRIMEIRO.\n");
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
                clearScreen();
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
                clearScreen();         
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