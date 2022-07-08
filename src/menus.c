#include "uart.h"

void menu_select_game(){
    putString("_|___________|SELECT GAME|__________|_\n\r",40);
    putString(" |                                  |\n\r",39);
    putString(" |    1 - ORDENE A MATRIZ           |\n\r",39);
    putString(" |    2 - NAVAL BATTLE              |\n\r",39);
    putString(" |__________________________________|\n\r",39);
}

void menu_continua(){
    putString("+++DESEJA JOGAR OUTRO JOGO+++\n\r",31);
    putString("|                           |\n\r",31);
    putString("|    1 - SIM                |\n\r",31);
    putString("|    2 - NAO                |\n\r",31);
    putString("|___________________________|\n\r",31);
}