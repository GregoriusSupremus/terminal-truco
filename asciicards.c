#include <stdio.h>

int cardNumber = 3;
int cardSuit = 'E';

void showCards() 
{
    printf("\nSuas cartas: \n");
    
    printf("|%d       | |%d       | |%d       |\n|        | |        | |        |\n|   %c    | |   %c    | |   %c    |\n|        | |        | |        |\n|       %d| |       %d| |       %d|\n", cardNumber, cardNumber, cardNumber, cardSuit, cardSuit, cardSuit, cardNumber, cardNumber, cardNumber);
}

int main()
{
    showCards();
}
    
    