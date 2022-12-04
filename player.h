#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "mapgen.h"
#include "const.h"
#include "global.h"
#include "util.h"

//���� ��ġ�� �÷��̾� ��ġ 
void spawn_player(world_t *w);
void move_up(world_t *w);
void move_down(world_t *w);
void move_left(world_t *w);
void move_right(world_t *w);
//# -> % -> . ������ �μ��� 
void break_wall(world_t *w, int dir);
void chest_event(world_t *w);
//���� 
void fight_enemy(world_t *w);
int player_attack(entity_t *player, entity_t *enemy);
//������ ��� (�̱���) 
void player_use(entity_t *player);

void player_town_move_up(world_t *w);
void player_town_move_down(world_t *w);
void player_town_move_left(world_t *w);
void player_town_move_right(world_t *w);

int player_town_check_inn(world_t *w);
int player_town_check_cave(world_t *w);
int player_town_check_church(world_t *w);
int player_town_check_blacksmith(world_t *w);

#endif
