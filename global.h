#ifndef _GLOBAL_H_
#define _GLOBAL_H_
//플레이어 설정 
static int player_max_hp = 100;
static int player_init_dmg = 5;
static int player_init_dmg_vary = 3;
static int player_inv_cap = 10;
static int inventory_initial_quantities[ITEM_COUNT] = {0, 0, 0, 0, 5, 1, 0};
//마을 내부에서 각각 이벤트의 위치 지정 
static int cave[3][2] = {{0, 16}, {1, 16}, {2, 16}};
static int inn[2][2] = {{20, 10}, {21, 10}};
static int inn_exit[2] = {20, 11};
static int church[2] = {78, 11};
static int church_exit[2] = {78, 12};
static int blacksmith[2][2] = {{46, 23}, {47, 23}};
static int blacksmith_exit[2] = {46, 24};
//시야 기능 
static char fov_map[ROWS][COLS];
extern int cheat; //실험을 위한 치트 활성화 추후 삭제 예정 

#endif
