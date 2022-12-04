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
//�÷��̾��� �⺻ �������ͽ� 
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
//������ �⺻ �������ͽ� 
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
    �ʻ�����
	�ʿ��� �� 45%������ ����.
	�������� �ٴ�. 
*/
void map_init(char map[ROWS][COLS]);

/*
	�־��� ���������� �󸶳� ���� ���� �ִ��� Ȯ�� 
    X X X
    X O X
    X X X
    O�� �������� �󸶳� ���� X�� �ִ����� �Ǻ� 
*/
int map_tile_neighbours(char map[ROWS][COLS], int x, int y);

//�� 1ȸ ������, �ش� ������ �ݺ� 
void map_step(char map[ROWS][COLS]);

//n����ŭ map_step�� �ݺ� 
void map_iter(char map[ROWS][COLS], int n);

void map_add_exit(world_t *w);
void map_add_coins(world_t *w);
void map_add_enemies(world_t *w);
void map_add_chest(world_t *w);
void map_add_ore(world_t *w);
void apply_fov(world_t *w, char map[ROWS][COLS], int fov_x, int fov_y);

#endif
