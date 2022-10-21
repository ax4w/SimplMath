#include <curses.h>
#include "./sclui/sclui.h" 
#include <random>
#define TO_SOLVE 5

using namespace sclui;

static std::vector<int> results = {};

TextBox *minimum = nullptr;
TextBox *maximum = nullptr;
Screen *game = nullptr;

bool tbFilter(int c) {
    return isdigit(c) || c == '*' || c == '+' || c == '-';
}

void doSolve() {
    for(int i = 0; i <= TO_SOLVE; i++) {
        TextBox *tb = (TextBox*) game->getItemAt(i);

        int r = -999;
        if(tb->getValueLength() > 0)
            int r = std::stoi(tb->getValue());
        if(r == results.at(i)) {
            tb->setColor(COLOR_GREEN);
        }else{
            tb->setColor(COLOR_RED);
            tb->setText(std::to_string(results.at(i)));
        }
        tb->setInteractable(false);
        tb->setColorFocus(COLOR_WHITE);
    }
    game->getItemByName("Solve")->setVisible(false);
    game->update();
}

void gameScreen() {
    if(minimum->getValueLength() == 0 || maximum->getValueLength() == 0) return;

    int minVal = std::stoi(minimum->getValue());
    int maxVal = std::stoi(maximum->getValue());

    if(minVal >= maxVal) return;

    game = new Screen("Game",40,20,'-','|');
    Button *quit = new Button("Quit",1,19,COLOR_WHITE,COLOR_RED);
    quit->onButtonPress = &doQuit;
    Button *solve = new Button("Solve",1,16,COLOR_WHITE,COLOR_GREEN);
    solve->onButtonPress = &doSolve;


    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> nums(minVal, maxVal); // define the range
    std::uniform_int_distribution<> choices(1, 3); // define the range

    int num1,num2,choice,res,yOffet = 2;
    
    for(int i = 0; i <= TO_SOLVE; i++) {
        
        std::string textBoxText = "";
        num1 = nums(gen);
        num2 = nums(gen);
        choice = choices(gen);
        textBoxText.append(std::to_string(num1));
        switch(choice) {
            case 1:
                res = num1 + (num2);
                textBoxText.append("+");
                break;
            case 2:
                res = num1 - (num2);
                textBoxText.append("-");
                break;
            case 3:
                res = num1 * (num2);
                textBoxText.append("*");
                break;
        }
        textBoxText.append(std::to_string(num2));
        results.push_back(res);
        TextBox *tb = new TextBox(textBoxText,1,1+yOffet,7,COLOR_WHITE,COLOR_BLUE,&tbFilter, '=');
        yOffet += 2;
        game->addItem(tb);
        game->centerItem(game->X,tb);
    }
    game->addItem(solve);
    game->addItem(quit);
    game->centerItem(game->X,solve);
    game->centerItem(game->X,quit);
    game->draw();
}


int main(void) {
    
    initSclui();
    Screen *menu = new Screen("Menu",40,14,'-','|');

    Text *text = new Text("Welcome to SimplMath",1,1,COLOR_BLANK);
    minimum = new TextBox("Minimum",1,3,4,COLOR_WHITE,COLOR_BLUE,&TextBoxFilterIsNumber, '|');
    maximum = new TextBox("Maximum",1,5,4,COLOR_WHITE,COLOR_BLUE,&TextBoxFilterIsNumber, '|');
    Button *play = new Button("Play",1,1,COLOR_WHITE,COLOR_GREEN);
    play->onButtonPress = &gameScreen;
    Button *quit = new Button("Quit",1,13,COLOR_WHITE,COLOR_RED);
    quit->onButtonPress = &doQuit;


    menu->addItem(text);
    menu->addItem(minimum);
    menu->addItem(maximum);
    menu->addItem(play);
    menu->addItem(quit);
    menu->centerItem(menu->X,text);
    menu->centerItem(menu->X,minimum);
    menu->centerItem(menu->X,maximum);
    menu->centerItem(menu->XY,play);
    menu->centerItem(menu->X,quit);
    menu->draw();

    return 0;
}