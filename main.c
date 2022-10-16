#include <curses.h>
#define SCLUI_C
#include "sclui/sclui.h"
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define RESLEN 5

int results[RESLEN];

bool isNum(char c) {
    return isdigit(c) || c == '-';
}

void solveGame() {

    color *right = calloc(1,sizeof(color));
    right->b = COLOR_GREEN;
    right->f = COLOR_BLACK;
    color *wrong = calloc(1,sizeof(color));
    wrong->b = COLOR_RED;
    wrong->f = COLOR_BLACK;

    for(int i = 0; i < RESLEN; i++) {
        int r = atoi(getTextboxUserInput(getInteractableItem(currentScreen,i)));
        char *c = calloc(8,sizeof(char));
        sprintf(c, "%d", results[i]);
        setTextBoxUserInput(getInteractableItem(currentScreen,i),c);
        getInteractableItem(currentScreen,i)->enabled = false;
        if(results[i] == r) {
            getInteractableItem(currentScreen,i)->bcolor = right;
        }else{
            getInteractableItem(currentScreen,i)->bcolor = wrong;
        }
    }
    getInteractableItemByText(currentScreen,"Solve")->enabled = false;
    updateCurrentScreen();
}

void gameScreen() {

    color menuScreenColor = {
        .b = COLOR_WHITE,
        .f = COLOR_BLACK
    };
    sclui_screen *gameScreen = initScreen("Game",RESLEN+2,0,40,20,KEY_UP,KEY_DOWN,&menuScreenColor);
    

    int min = getTextboxTextLength(getInteractableItemByText(currentScreen,"Minimum"));
    int max = getTextboxTextLength(getInteractableItemByText(currentScreen,"Maximum"));
    
    if(min == 0 || max == 0) return;

     color itemColor = {
        .b = COLOR_WHITE,
        .f = COLOR_BLACK
    };

    color itemColorFocus = {
        .b = COLOR_BLUE,
        .f = COLOR_BLACK
    };

    char *minTxt = getTextboxUserInput(getInteractableItemByText(currentScreen,"Minimum"));
    char *maxTxt = getTextboxUserInput(getInteractableItemByText(currentScreen,"Maximum"));

    int s = atoi(minTxt);
    int m = atoi(maxTxt);
    char *op = NULL;
    int r1 = 0, r2 = 0, y = 3,choice = 0,res = 0;
    srand(time(NULL));
    for(int i = 0; i < RESLEN; i++) {
        r1 = (rand() % (m - s + 1)) + s;
        r2 = (rand() % (m - s + 1)) + s;
        choice = (rand() % (3 - 1 + 1)) + 1;
        res = 0;
        op = calloc(1,sizeof(char));
        switch(choice) {
            case 1:
                op = "+";
                res = r1 + (r2);
                break;
            case 2:
                op = "-";
                res = r1- (r2);
                break;
            case 3:
                op = "*";
                res = r1* (r2);
                break;
        }
        char *s1 = calloc(4,sizeof(char));
        char *s2 = calloc(4,sizeof(char));
        char *s3 = calloc(7,sizeof(char));
        sprintf(s1, "%d", r1);
        sprintf(s2, "%d", r2);
        strcat(s3,s1);
        strcat(s3,op);
        strcat(s3,s2);
        results[i] = res;

        sclui_interactable_item *b = 
            createTextBox(s3,&isNum,1,y,6,&itemColor,&itemColorFocus);
        addInteractableItem(gameScreen,b);
        b->center(b,X);
        y += 2;
    }
   

    sclui_interactable_item *start =
        createButton("Solve",&solveGame,1,y,&itemColor,&itemColorFocus);
        
    y += 2;

    sclui_interactable_item *quit =
        createButton("Quit",&doQuit,1,y,&itemColor,&itemColorFocus);

    addInteractableItem(gameScreen,start);
    addInteractableItem(gameScreen,quit);

    start->center(start,X);
    quit->center(quit,X);

    freeScreen(currentScreen);
    runScreen(gameScreen);
}

void mainMenu() {
    /*
        Screen
    */
    color menuScreenColor = {
        .b = COLOR_WHITE,
        .f = COLOR_BLACK
    };
    sclui_screen *menuScreen = initScreen("Menu",4,0,40,20,KEY_UP,KEY_DOWN,&menuScreenColor);

    /*
        Items
    */
    color itemColor = {
        .b = COLOR_WHITE,
        .f = COLOR_BLACK
    };

    color itemColorFocus = {
        .b = COLOR_BLUE,
        .f = COLOR_BLACK
    };
    sclui_interactable_item *minVals = 
        createTextBox("Minimum",&isNum,1,6,3,&itemColor,&itemColorFocus);

    sclui_interactable_item *maxVals = 
        createTextBox("Maximum",&isNum,1,8,3,&itemColor,&itemColorFocus);
    
    sclui_interactable_item *start =
        createButton("Start",&gameScreen,1,10,&itemColor,&itemColorFocus);
    
    sclui_interactable_item *quit =
        createButton("Quit",&doQuit,1,12,&itemColor,&itemColorFocus);

    addInteractableItem(menuScreen,minVals);
    addInteractableItem(menuScreen,maxVals);
    addInteractableItem(menuScreen,start);
    addInteractableItem(menuScreen,quit);

    minVals->center(minVals,X);
    maxVals->center(maxVals,X);
    start->center(start,X);
    quit->center(quit,X);

    runScreen(menuScreen);

    
}

int main(void) {
    WINDOW *w = initscr();
    if(w == NULL) exit(-1);
    setup();
    setConfig(10, 10);
    mainMenu();
}