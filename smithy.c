#include "smithy.h"

#include <Windows.h>
#include <mmsystem.h>
//길지만 별거 없음.
//플레이어의 선택에 따라 재료가 충분하면 업그레이드를 진행해줌 
void upgrade_menu(HANDLE con, entity_t *player, inventory_t *inv, int choice){
    char buffer[CONSOLE_COLS];
    char buffer1[CONSOLE_COLS];
    int arm_p_g = 10 + 3 * player->armour_lvl;
    int arm_p_o = 3 + player->armour_lvl;
    int weap_p_g = 10 + 3 * player->weapon_lvl;
    int weap_p_o = 3 + player->weapon_lvl;

    sprintf(buffer, "Upgrade armour (DEF: %d -> %d) for %d ores and %d gold coins", player->armour, (int)(player->armour * 1.2 + 2), arm_p_o, arm_p_g);
    sprintf(buffer1, "Upgrade weapon (BASE ATK: %d -> %d) for %d ores, %d gold coins", player->dmg, (int)(player->dmg * 1.2 + 3), weap_p_o, weap_p_g);

    choice = display_dialogue_box(con, "So - what will it be? A sharp blade or a hard plate?\n", (char *[]){buffer, buffer1, "Leave"}, 2);
    if(choice == 0) {//무기 업그레이드. 	
	//재료가 충분하면 업그레이드 함수 호출. 아니라면 뒤로 
        if(player->inv->items[6].quantity < arm_p_o || player->coins < arm_p_g){
            choice = display_dialogue_box(con, "You know blacksmiths need metal t' work with. Come back once you have some and I'll gladly help.\n", (char *[]){"Go Back", "Leave"}, 2);
            if(choice == 0) upgrade_menu(con, player, inv, choice);
            else return;
        }
        else upgrade_armour(con, player, inv, choice);
    }
    else if(choice == 1) {//방어구 업그레이드
	//재료가 충분하면 업그레이드 함수 호출. 아니라면 뒤로 
        if(player->inv->items[6].quantity < weap_p_o || player->coins < arm_p_g){
            choice = display_dialogue_box(con, "You know blacksmiths need metal t' work with. Come back once you have some and I'll gladly help.\n", (char *[]){"Go Back", "Leave"}, 2);
            if(choice == 0) upgrade_menu(con, player, inv, choice);
            else return;
        }
        else upgrade_weapon(con, player, inv, choice);
    }
}
//수치로 업그레이드가 아닌 %업그레이드 진행 
void upgrade_weapon(HANDLE con, entity_t *player, inventory_t *inv, int choice){
    inv->items[6].quantity -= 3 + 2*player->weapon_lvl;
    player->coins -= 10 + 2.5 * player->weapon_lvl;
    player->weapon_lvl++;
    player->dmg += (int)(0.4 * player->dmg);
    player->dmg_vary++;
    choice = display_dialogue_box(con, "There ya go..all sharpened and ready to cleave!\n", (char *[]){"Go Back", "Leave"}, 2);
    if(choice == 0) upgrade_menu(con, player, inv, choice);
    else return;
}
//수치로 업그레이드가 아닌 %업그레이드 진행 
void upgrade_armour(HANDLE con, entity_t *player, inventory_t *inv, int choice){
    inv->items[6].quantity -= 3 + 2*player->armour_lvl;
    player->coins -= 10 + 2.5 * player->armour_lvl;
    player->armour_lvl++;
    player->armour += (int)(0.5 * player->armour);
    choice = display_dialogue_box(con, "There we go - the sturdiest in town, and it'll only get better!\n", (char *[]){"Go Back", "Leave"}, 2);
    if(choice == 0) upgrade_menu(con, player, inv, choice);
    else return;
}
//지정된 좌표에 플레이어 도착시 나오는 기본 멘트 
void smithy(HANDLE con, entity_t *player, inventory_t *inv){
    int choice = display_dialogue_box(con, "Welcome to my smithy, traveller, I hope I can be of service to ya.\n", (char *[]) {"Continue...", "Leave"}, 2);
    if(choice == 0) upgrade_menu(con, player, inv, choice);
    else return;
}
