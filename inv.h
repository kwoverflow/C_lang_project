#ifndef _INV_H_
#define _INV_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>

#include "mapgen.h"
#include "util.h"
#include "const.h"

//인벤토리와 인벤토리 항목이름 구조체선언 
static char *item_names[] = {
    "Potion of healing",
    "Potion of strength",
    "Potion of defense",
    "Potion of vision",
    "Apple",
    "Pickaxe",
    "Ore"
};
#define ITEM_COUNT sizeof(item_names) / sizeof(char*)

#define USAGE_CONSUMABLE 0xf0
#define USAGE_WEAPON 0xf1
#define USAGE_POTION 0xf2
//인벤토리 아이템의 사용 
typedef struct _item {
    char *name;
    int quantity;
    int dmg;
    int hp;
    int usage;
} item_t;
//인벤토리 장착 아이템 구현 
typedef struct _inventory {
    item_t items[ITEM_COUNT];
    int equipped;
} inventory_t;

//현재 프레임 위에 인벤토리 화면 그리기 
void inventory_draw(HANDLE con, inventory_t *inv);
int inventory_calc_size(inventory_t *inv);
void inventory_init(inventory_t *inv, int quantities[]);

//몹에게 이겼을 시 드롭되는 아이템 획득 
void drops(HANDLE con, inventory_t *inv);
//인벤토리 아이템 사용 (버그로 전면 삭제) 
void use(HANDLE con, inventory_t *inv);

#endif
