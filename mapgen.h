#ifndef _MAPGEN_H_
#define _MAPGEN_H_

#include <stdio.h>
#include <stdlib.h>

#include "const.h"
#include "inv.h"
#include "util.h"

#define STATUS_ROAM 0
#define STATUS_INFIGHT 1
#define STATUS_INMENU 2
#define STATUS_INTOWN 3
#define STATUS_CHEST 4
//플레이어의 기본 스테이터스 
typedef struct _entity {
    int hp;
    int max_hp;
    
    int dmg;
    int dmg_vary;
    
    int coins;
    int armour;
    
    int escaped;
    int defending;
    int x;
    int y;
    
    int weapon_lvl;
    int armour_lvl;

    struct _inventory *inv;
} entity_t;
//적들의 기본 스테이터스 
typedef struct _enemy {
	int hp;
	int max_hp;
	
	int dmg;
	int dmh_vary;

	int coins;	
	int armour;
	
	int defending;
}enemy_t;

typedef struct _world {
    char map[ROWS][COLS];
    char town[CONSOLE_ROWS][COLS];
    entity_t *player;
    int enemy_count;
    int depth;
    int exit_x;
    int exit_y;
    int status;
} world_t;

/*
    맵생성시
	맵에서 약 45%비율이 벽임.
	나머지는 바닥. 
*/
void map_init(char map[ROWS][COLS]);

/*
	주어진 범위내에서 얼마나 많은 벽이 있는지 확인 
    X X X
    X O X
    X X X
    O를 기준으로 얼마나 많은 X가 있는지를 판별 
*/
int map_tile_neighbours(char map[ROWS][COLS], int x, int y);

//룰 1회 적용후, 해당 사항을 반복 
void map_step(char map[ROWS][COLS]);

//n번만큼 map_step을 반복 
void map_iter(char map[ROWS][COLS], int n);

void map_add_exit(world_t *w);
void map_add_coins(world_t *w);
void map_add_enemies(world_t *w);
void map_add_chest(world_t *w);
void map_add_ore(world_t *w);
void apply_fov(world_t *w, char map[ROWS][COLS], int fov_x, int fov_y);

#endif
