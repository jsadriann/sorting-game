#include "memory.h"
#include "timers.h"
unsigned int game_easy[3][3] = {{0,5,3},
                                {4,2,8},
                                {1,6,7}};

char msn[] = "PARABENS JOGADOR, CONSEGUIU ORDENAR";

bool continua = true, pause = true;
unsigned pos_x = 0, pos_y = 0,pos_x_ant, pos_y_ant;

int ordenou(){

    if((game_easy[0][0] == 1)&(game_easy[0][1] == 2)&(game_easy[0][2] == 3)
    &(game_easy[1][0] == 4)&(game_easy[1][1] == 5)&(game_easy[1][2] == 6)
    &(game_easy[2][0] == 7)&(game_easy[2][1] == 8)&(game_easy[2][2] == 0))
        return 1;
    return 0;
    
}
void print_game(){

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            putCh(game_easy[i][j]+'0');
            putCh(' ');
        }
        putString("\n\r",2);
    }
    
}

void clean(){

    putString("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n",27);

}

void start_game(){

    putString("______|DEIXE NA SEQUENCIA CORRETA|______\n\r",42);
    delay(1000,TIMER7);
    while(continua){
        clean();
        pause = true;
        print_game();
        while(pause);
    }
    clean();
    print_game();
    for(int i = 0; i < 35; i++){
        putCh(msn[i]);
        delay(200,TIMER7);
    }

}

void up(){

    pos_y_ant = pos_y;
    pos_x_ant = pos_x;
    pos_y--;
    if(pos_y < 0) pos_y = 0;

    unsigned int aux = game_easy[pos_y_ant][pos_x_ant];
    game_easy[pos_y_ant][pos_x_ant] = game_easy[pos_y][pos_x];
    game_easy[pos_y][pos_x] = aux;

    if(ordenou()) continua = false;
    pause = false;

}

void down(){

    pos_y_ant = pos_y;
    pos_x_ant = pos_x;
    pos_y++;
    if(pos_y > 2) pos_y = 2;

    unsigned int aux = game_easy[pos_y_ant][pos_x_ant];
    game_easy[pos_y_ant][pos_x_ant] = game_easy[pos_y][pos_x];
    game_easy[pos_y][pos_x] = aux;

    if(ordenou()) continua = false;
    pause = false;

}

void left(){

    pos_y_ant = pos_y;
    pos_x_ant = pos_x;
    pos_x--;
    if(pos_x < 0) pos_x = 0;

    unsigned int aux = game_easy[pos_y_ant][pos_x_ant];
    game_easy[pos_y_ant][pos_x_ant] = game_easy[pos_y][pos_x];
    game_easy[pos_y][pos_x] = aux;

    if(ordenou()) continua = false;
    pause = false;

}

void right(){

    pos_y_ant = pos_y;
    pos_x_ant = pos_x;
    pos_x++;
    if(pos_y > 2) pos_y = 2;

    unsigned int aux = game_easy[pos_y_ant][pos_x_ant];
    game_easy[pos_y_ant][pos_x_ant] = game_easy[pos_y][pos_x];
    game_easy[pos_y][pos_x] = aux;

    if(ordenou()) continua = false;
    pause = false;

}