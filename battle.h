#ifndef _BATTLE_H_
#define _BATTLE_H_

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <Windows.h>

#include "mapgen.h"
#include "global.h"
#include "player.h"
#include "inv.h"
#include "util.h"

void battle(HANDLE con, entity_t *player, inventory_t *inv);//전투 이벤트 
void victory(HANDLE con, entity_t *player, entity_t *enemy, inventory_t *inv);//승리 이벤트 
void defeat(HANDLE con);//패배 이벤트 
void player_turn(HANDLE con, entity_t *player, entity_t *enemy);//플레이어의 턴 
int enemy_turn(HANDLE con, entity_t *player, entity_t *enemy);//적의 턴 

#endif
