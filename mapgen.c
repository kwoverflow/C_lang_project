#include "mapgen.h"
#include <Windows.h>
#include <mmsystem.h>
//지정한 열, 행에 맞는 맵을 그린다 
void map_init(char map[ROWS][COLS]) {

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
        	//45%의 확률을 지정하였기 때문에 45%가 벽이다. 
            if (rand_int(1, 100) < INITIAL_PROBABILITY) {
                map[i][j] = WALL;
            }
            else map[i][j] = FLOOR;
        }
    }
}
//벽끼리 얼마나 붙어 있는지 확인하는 로직
//간단하게 벽의 두께를 확인하는 과정이라 칭하자. 
int map_tile_neighbours(char map[ROWS][COLS], int x, int y) {
    int count = 0;
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            int neighbour_x = x + i;
            int neighbour_y = y + j;
            if (i == 0 && j == 0) continue;
            if (neighbour_x < 0 || neighbour_y < 0 || neighbour_x > COLS - 1 || neighbour_y > ROWS - 1) count++;
            if (map[neighbour_y][neighbour_x] == WALL) count++;
        }
    }
    return count;
}
//랜덤으로 구성된 맵이 이상할 수 있으므로 
//좀 동굴처럼 생기도록 조정이 들어가는 로직 
void map_step(char map[ROWS][COLS]) {
    int newmap[ROWS][COLS];
    int n;
	//맵전체 스캔, 만약 지정한 최대 벽 두께 보다 두꺼우면
	//오른쪽이나 아래쪽을 벽타일이 아닌 바닥타일로 대체 
    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            n = map_tile_neighbours(map, x, y);
            if (map[y][x] == WALL) {
                if (n >= STAY_WALL_LIMIT) newmap[y][x] = WALL;
                else newmap[y][x] = FLOOR;
            }
            else {
                if (n >= BECOME_WALL_LIMIT) newmap[y][x] = WALL;
                else newmap[y][x] = FLOOR;
            }
            //가장자리는 전부 벽으로 처리 
            if (y == 0 || y == ROWS - 1) newmap[y][x] = WALL;
            if (x == 0 || x == COLS - 1) newmap[y][x] = WALL;
        }
    }
    //위의 로직으로 생성한 맵을 진짜 맵으로 올린다. 
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            map[i][j] = newmap[i][j];
        }
    }
}
//층에 따라 맵을 전부 점검한다. 
void map_iter(char map[ROWS][COLS], int n) {
    for (int i = 0; i < n; ++i) {
        map_step(map);
    }
}
//바닥타일중 한곳을 랜덤으로 골라 출구 > 문자로 대체한다. 
void map_add_exit(world_t *w) {
    w->exit_x = -1;
    w->exit_y = -1;
    //do while문은 무조건 한번은 돌아가기 때문에 어떠한 상황에서도 
	//출구는 생성된다. 
    do {
        for (int y = 0; y < ROWS; ++y) {
            for (int x = 0; x < COLS; ++x) {
                if (5 >= rand_int(1, ROWS * COLS)) {
                    if (w->map[y][x] == FLOOR) {
                        w->map[y][x] = EXIT;
                        w->exit_x = x;
                        w->exit_y = y;
                        return;
                    }
                }
            }
        }
    } while ((w->exit_x == -1) && (w->exit_y == -1));
}
//적당한 수량으로 서로 붙어 있지 않게 코인을 구석구석 배치한다. 
void map_add_coins(world_t *w) {
    int n;
    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            n = map_tile_neighbours(w->map, x, y);
            if ((n <= 3) && (1 >= rand_int(1, 100))) w->map[y][x] = COIN;
        }
    }
}
//지정된 적의 숫자 만큼 적을 배치한다. 
void map_add_enemies(world_t *w) {
    int x = 0;
    int y = 0;
    for (int i = 0; i < w->enemy_count; ++i) {
    	//벽에 적이 박혀있는 현상 방지 
        do {
            x = rand_int(0, COLS - 1);
            y = rand_int(0, ROWS - 1);
        } while (w->map[y][x] == WALL);
        w->map[y][x] = ENEMY;
    }
}
//? 표시로 표시되는 상자를 추가한다. 
void map_add_chest(world_t *w) {
    int x = 0;
    int y = 0;
    do {
        x = rand_int(0, COLS - 1);
        y = rand_int(0, ROWS - 1);
    } while (map_tile_neighbours(w->map, x, y) != 5);
    w->map[y][x] = CHEST;
}
//무기와 방어구의 개조에 쓰이는 원석을 추가한다. 
void map_add_ore(world_t *w) {
    int x = 0;
    int y = 0;
    int count = rand_int(1, 10);//갯수 랜덤 
    for (int i = 0; i < count; ++i) {
        do {
            x = rand_int(0, COLS - 1);
            y = rand_int(0, ROWS - 1);
        } while (map_tile_neighbours(w->map, x, y) != 8);
        w->map[y][x] = ORE;
    }
}
//전체 맵이 다 보이면 몰입도가 떨어지니 플레이어를 기준으로 제한된 시야를 만들어준다 
void apply_fov(world_t *w, char map[ROWS][COLS], int fov_x, int fov_y) {
    int py = w->player->y;
    int px = w->player->x;
	//월드보더에 있게 된다면 그에 맞게 줄어든다.(안그러면 버그난다.) 
    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            if ((abs(y - py) <= fov_y) && (abs(x - px) <= fov_x)) map[y][x] = w->map[y][x];
            else map[y][x] = ' ';
        }
    }
}
