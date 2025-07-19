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
#define POINTS_TO_WIN 5
#define DEFAULT_ROUND_VALUE 1

typedef struct
{
    int number;
    int hierarchy;
    char suit;
    bool isPlayed;
} Card;

typedef struct Player
{
    int points;
    int counter;
    bool playsNext;
} Player;

const int possibleNumbers[DIFFERENT_CARDS] = {1, 2, 3, 4, 5, 6, 7, 10, 11, 12};

const char possibleSuits[4] = {'P', 'O', 'E', 'C'};

Card deck[DECK_SIZE] = {};

void decideFirstToPlay(Player *player, Player *opponent) 
{
    int coin = (rand() % 2) + 1;
    if (coin == 1) 
    {
        printf("\nTU JOGA PRIMEIRO.\n");
        player->playsNext = true;
        opponent->playsNext = false;
    } 
    
    else if (coin == 2) 
    {
        printf("\nOPONENTE JOGA PRIMEIRO.\n");
        player->playsNext = false;
        opponent->playsNext = true;
    }
}

int generateHierarchy(int number, char suit)
{
    int hierarchy = 0;

    if (number == 3)
    {
        hierarchy = 10;
    }

    else if (number == 2)
    {
        hierarchy = 9;
    }

    else if (number == 1)
    {
        hierarchy = 8;

        if (suit == 'E')
        {
            hierarchy = 14;
        }
        else if (suit == 'P')
        {
            hierarchy = 13;
        }
    }

    else if (number == 12)
    {
        hierarchy = 7;
    }

    else if (number == 11)
    {
        hierarchy = 6;
    }

    else if (number == 10)
    {
        hierarchy = 5;
    }

    else if (number == 7)
    {
        hierarchy = 4;
        if (suit == 'O')
        {
            hierarchy = 11;
        }
        else if (suit == 'E')
        {
            hierarchy = 12;
        }   
    }

    else if (number == 6)
    {
        hierarchy = 3;
    }

    else if (number == 5)
    {
        hierarchy = 2;
    }

    else if (number == 4)
    {
        hierarchy = 1;
    }
    return hierarchy;
}

void generateDeck()
{
    for (int i = 0; i < 10; i++)
    {
        deck[i].number = possibleNumbers[i];
        deck[i].suit = possibleSuits[0];
        deck[i].hierarchy = generateHierarchy(deck[i].number, deck[i].suit);
        deck[i].isPlayed = false;
    }

    for (int i = 10; i < 20; i++)
    {
        deck[i].number = possibleNumbers[i - 10];
        deck[i].suit = possibleSuits[1];
        deck[i].hierarchy = generateHierarchy(deck[i].number, deck[i].suit);
        deck[i].isPlayed = false;
    }

    for (int i = 20; i < 30; i++)
    {
        deck[i].number = possibleNumbers[i - 20];
        deck[i].suit = possibleSuits[2];
        deck[i].hierarchy = generateHierarchy(deck[i].number, deck[i].suit);
        deck[i].isPlayed = false;
    }

    for (int i = 30; i < 40; i++)
    {
        deck[i].number = possibleNumbers[i - 30];
        deck[i].suit = possibleSuits[3];
        deck[i].hierarchy = generateHierarchy(deck[i].number, deck[i].suit);
        deck[i].isPlayed = false;
    }
}

void shuffleDeck()
{
    for (int i = DECK_SIZE - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void drawCards(Card * handA, Card * handB)
{
    for (int i = 0; i < 3; i++)
    {
        handA[i] = deck[i];
    }
    for (int j = 3; j < 6; j++)
    {
        handB[j - 3] = deck[j];
    }
}

void showCards(Card * playerHand, Card * opponentHand) 
{
    printf("\nSuas cartas: ");
    for (int i = 0; i < HAND_SIZE; i++)
    {
        printf("%d%c ", playerHand[i].number, playerHand[i].suit);
    }
    
    printf("\nCartas do oponente (para testes): ");
    for (int j = 0; j < HAND_SIZE; j++)
    {
        printf("%d%c ", opponentHand[j].number, opponentHand[j].suit);
    }
    printf("\n---------------------------------------");
}

int getCard(Card * hand)
{
    int cardIndex;
    while (true)
    {
        scanf("%d", &cardIndex);
        if (hand[cardIndex - 1].isPlayed)
        {
            printf("\nJogue outra carta.");
        }
        else {break;}
    }
    return cardIndex;
}

Card playerPlaysCard(Card * playerHand)
{
    printf("\nJogue uma carta: ");
    for (int i = 0; i < HAND_SIZE; i++)
    {
        if (playerHand[i].isPlayed == false)
        {
            printf("%d.|%d%c| ", (i + 1), playerHand[i].number, playerHand[i].suit);
        }
    }
    printf("\n");

    int cardIndex = getCard(playerHand);
    playerHand[cardIndex - 1].isPlayed = true;
    printf("\nTU: %d%c", playerHand[cardIndex - 1].number, playerHand[cardIndex - 1].suit);
    return playerHand[cardIndex - 1];
}

Card aiPlaysCard(Card * aiHand)
{
    int randomIndex = RANDOM_HAND_CARD;

    while (aiHand[randomIndex].isPlayed)
    {
        randomIndex = RANDOM_HAND_CARD;
    }
    
    printf("\nOPONENTE: %d%c", aiHand[randomIndex].number, aiHand[randomIndex].suit);
    aiHand[randomIndex].isPlayed = true;
    return aiHand[randomIndex];
}

void compareCards(Player * playerA, Player * playerB, Card * playerACard, Card * playerBCard)
{
    if (playerACard->hierarchy > playerBCard->hierarchy)
    {
        playerA->counter += 1;
        playerA->playsNext = true;
    }
    else if (playerACard->hierarchy < playerBCard->hierarchy)
    {
        playerB->counter += 1;
        playerB->playsNext = true;
    }
    else if (playerACard->hierarchy == playerBCard->hierarchy)
    {
        playerA->counter += 1;
        playerB->counter += 1;
        playerA->playsNext = true;
    }
}

void addPoints(Player * playerA, Player * playerB, int roundValue)
{
    if (playerA->counter >= 2 && playerA->counter != playerB->counter)
    {
        playerA->points += roundValue;
    }
    
    else if (playerA->counter >= 2 && playerA->counter == playerB->counter)
    {
        if (playerA->playsNext == true)
        {
            playerA->points += roundValue;
        }
        else if (playerB->playsNext)
        {
            playerB->points += roundValue;
        }
    }

    else if (playerB->counter >= 2 && playerA->counter != playerB->counter)
    {
        playerB->points += roundValue;
    }
}

void showPoints(Player * player, Player * opponent)
{
    printf("\nTu: %d", player->points);
    printf("\nOponente: %d", opponent->points);
}

int main()
{
    Player player, ai;
    Card playerHand[HAND_SIZE];
    Card aiHand[HAND_SIZE];
    Card currentPlayerCard;
    Card currentAiCard;

    player.points = 0;
    player.counter = 0;

    ai.points = 0;
    ai.counter = 0;

    int cardsPlayed = 0;
    int roundValue = DEFAULT_ROUND_VALUE;
    int pointsToWin = POINTS_TO_WIN;
    int roundsPlayed = 1;

    srand(time(NULL));
    generateDeck();
    shuffleDeck();

    drawCards(playerHand, aiHand);
    showCards(playerHand, aiHand);

    decideFirstToPlay(&player, &ai);

    printf("\nRodada %d", roundsPlayed);

    while (true)
    {
        if (cardsPlayed == 6 || player.counter >= 2 || ai.counter >= 2)
        {
            roundsPlayed += 1;
            if (player.points >= pointsToWin)
            {
                printf("\nTU VENCEU!");
                break;
            }

            else if (ai.points >= pointsToWin)
            {
                printf("\nOPONENTE VENCEU.");
                break;
            }

            addPoints(&player, &ai, roundValue);
            showPoints(&player, &ai);

            printf("\nRodada %d", roundsPlayed);
            player.counter = 0;
            ai.counter = 0;
            cardsPlayed = 0;
            roundValue = DEFAULT_ROUND_VALUE;
            shuffleDeck();
            drawCards(playerHand, aiHand);
            showCards(playerHand, aiHand);
        }

        else if (player.playsNext)
        {
            currentPlayerCard = playerPlaysCard(playerHand);
            cardsPlayed += 1;

            currentAiCard = aiPlaysCard(aiHand);
            cardsPlayed += 1;

            compareCards(&player, &ai, &currentPlayerCard, &currentAiCard);
        }

        else if (ai.playsNext) 
        {
            currentAiCard = aiPlaysCard(aiHand);
            cardsPlayed += 1;

            currentPlayerCard = playerPlaysCard(playerHand);
            cardsPlayed += 1;

            compareCards(&ai, &player, &currentAiCard, &currentPlayerCard);
        } 
    }
}