#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <unistd.h>
#define ROWS 25
#define COLUMNS 80

struct Node
{
    struct Node* nextNode;
    char x;
    char y;
};

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void drawSnake(struct Node* snake){
    struct Node* tail = snake;
    while (tail != NULL) 
    {
        //move to position
        gotoxy(tail->x + 1, tail->y + 1);
        printf("s"); 
        tail = tail->nextNode;
    } 
    gotoxy(0, 0);
}

void clearSnake(struct Node* snake, char* delx, char* dely){
    struct Node* tail = snake;
    
    while (tail->nextNode != NULL) 
    {
        tail = tail->nextNode;
    }

    *delx = tail->x;
    *dely = tail->y;
}

void moveSnake(struct Node* snake, char dirx, char diry){
    struct Node* tail = snake;
    
    //save location of previous node
    char prevx = snake->x, prevy = snake->y, temp = 0;
    //move snake head
    tail->x += dirx;
    tail->y += diry;
    if (tail->nextNode == NULL)
    {
        return;
    }
    
    tail = tail->nextNode;   
    while (tail != NULL)
    {
        //move next tail node
        temp = tail->x;
        tail->x = prevx;
        prevx = temp;
    
        temp = tail->y;
        tail->y = prevy;
        prevy = temp;

        

        tail = tail->nextNode;
    }

}

void hidecursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void showcursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void clrscr() {
    system("cls");
}

void drawBorder() {
    gotoxy(0, 0);
    //draw border
    for (int i = 0; i < ROWS + 2; i++)
    {
        //printf("Row number: %d ", i);
        for (int j = 0; j < COLUMNS + 2; j++)
        {
            if (i == 0 || i > ROWS)
                printf("o");

            else if (j == 0 || j > COLUMNS)
                printf("o");

            else 
                printf("\e[%iC", 1);
                
        }
        printf("\n");
    }
}

void drawCords(struct Node* head) {
    do
    {
        printf("%d-%d = ", head->x, head->y);
        head = head->nextNode;
    } while (head != NULL);
}

void addTail(struct Node* tail) {
    struct Node* newTail = (struct Node*) malloc(sizeof(struct Node));
    struct Node* head = tail;

    while (tail->nextNode != NULL)
    {
        tail = tail->nextNode;
    }
    
    newTail->x = head->x;
    newTail->y = head->y;
    newTail->nextNode = NULL;
    tail->nextNode = newTail;
}

void deleteSnake(struct Node* head) {
    struct Node* tail = head;
    do
    {
        if (tail->nextNode == NULL)
        {
            free(tail);
            break;
        }
        
        head = head->nextNode;
        free(tail);
        tail = head;
    } while (head->nextNode != NULL);
    
}

void spawnFruit(struct Node* head, char* fruitx, char* fruity) {
    if(rand() % 2) {
        *fruitx = (rand() % (COLUMNS - head->x)) + head->x;
    }  else {
        *fruitx = rand() % head->x;
    }
    if(rand() % 2) {
        *fruity = (rand() % (ROWS - head->y)) + head->y;
    }  else {
        *fruity = rand() % head->y;
    }
}

void main() {
    srand(time(NULL));
    
    start: hidecursor();

    //starting info
    clrscr();
    gotoxy(0, ROWS / 2);
    printf("\t\t\tSnake game\n\t\t\tControls W S A D\n\t\t\tto quit press Q\n\n\t\t\tTo start press any button");
    getch();
    clrscr();
    gotoxy(0, 0);
    
    char quit = 0, snakeLen = 3;
    char dirx = 1, diry = 0, delx = 0, dely = 0; 
    // make snake of length 3
    struct Node* head = (struct Node*) malloc(sizeof(struct Node));
    head->x = 40;
    head->y = 13;
    head->nextNode = NULL;
    addTail(head);
    addTail(head);

    //init fruit
    char fx = 0, fy = 5;
    spawnFruit(head, &fx, &fy);


    //initialize framerate
    int frame_rate = 5;
    int frameTime = 1000 / frame_rate;
    DWORD startTime, endTime;
    startTime = GetTickCount();

    clrscr();
    
    char input = -1;

    drawBorder();

    while (!quit)
    {
        // handle game quit
        if (kbhit())
        {
            input = getch();
        }

        if (input == 'q')
        {
            gotoxy(0, ROWS + 2);
            printf("quit using q button");
            quit = 1;
            break;
        }

        // handle move input
        if (input != -1)
        {
            if (input == 'd' && dirx != -1)
            {
                dirx = 1;
                diry = 0;
                // clear input
                input = -1;
            }
            if (input == 'a' && dirx != 1)
            {
                dirx = -1;
                diry = 0;
                // clear input
                input = -1;
            }
            if (input == 'w' && diry != 1)
            {
                dirx = 0;
                diry = -1;
                // clear input
                input = -1;
            }
            if (input == 's' && diry != -1)
            {
                dirx = 0;
                diry = 1;
                // clear input
                input = -1;
            }
            if (input == 'i')
            {
                input = -1;
                
            }
        }

        moveSnake(head, dirx, diry);
        gotoxy(0, ROWS + 2);

        //drawCords(head);
        printf("                                                           ");
        gotoxy(0, ROWS + 2);
        printf("Score: %d", snakeLen-3);

        //clear last snake tail node
        gotoxy(delx + 1, dely + 1);
        printf(" ");

        drawSnake(head);

        clearSnake(head, &delx, &dely);

        
        //snake collision

        struct Node* current = head->nextNode;

        while (current != NULL)
        {
            if (head->x == current->x && head->y == current->y)
            {
                quit = 1;
            }
            
            current = current->nextNode;
        }
        
        free(current);
        
        //border logic

        if (head->x < 0 || head->y < 0 || head->x >= COLUMNS || head->y > ROWS )
        {
            quit = 1;
        }
        
        //fruit logic
        gotoxy(fx + 1, fy + 1);
        printf("F");
        if (head->x == fx && head->y == fy)
        {
            spawnFruit(head, &fx, &fy);
            addTail(head);
            snakeLen++;
            if (snakeLen % 5 == 0)
            {
                frame_rate++;
                frameTime = 1000 / frame_rate;
            }
            
        }
        
        

        //game speed logic
        endTime = GetTickCount();
        int elapsedTime = endTime - startTime;

        if (elapsedTime < frameTime) {
            Sleep(frameTime - elapsedTime);
        }

        startTime = GetTickCount();
        
    }

    Sleep(850);
    //game end info
    clrscr();
    drawBorder();
    gotoxy(0, ROWS / 2);
    printf("\t\t\tYour score: %d\n\t\t\tto restart press R\n\t\t\tpress any button to quit", snakeLen-3);
    if(getch() == 'r') {
        goto start;
    }
    clrscr();
    gotoxy(0, 0);

    showcursor();

    deleteSnake(head);
    
}