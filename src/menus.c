#include "menus.h"

void menu_select_game(){
    putString(UART0,"_|_________|WELCOME TO GAME|________|_\n\r",40);
    putString(UART0,"_|___________|SELECT GAME|__________|_\n\r",40);
    putString(UART0," |                                  |\n\r",39);
    putString(UART0," |    1 - ORDENE A MATRIZ           |\n\r",39);
    putString(UART0," |    2 - NAVAL BATTLE              |\n\r",39);
    putString(UART0," |__________________________________|\n\r",39);
}

void menu_continua(){
    putString(UART0,"+++DESEJA JOGAR OUTRO JOGO+++\n\r",31);
    putString(UART0,"|                           |\n\r",31);
    putString(UART0,"|    1 - SIM                |\n\r",31);
    putString(UART0,"|    2 - NAO                |\n\r",31);
    putString(UART0,"|___________________________|\n\r",31);
}