#include <Windows.h>
#include <MMSystem.h>

#include "battle.h"
//적들마다 따로 만드는것은 비효율적
//struct로 체계화 해서 호출 
enemy_t *make_enemy() {
    entity_t *e = (enemy_t*)malloc(sizeof(enemy_t));
    e->dmg = 3 + rand_int(0, 3);
    e->dmg_vary = 3;
    e->max_hp = 20 + rand_int(0, 10);
    e->hp = e->max_hp;
    e->coins = rand_int(5, 10);
    return e;
} 
//적이 플레이어에게 주는 데미지 계산 
int enemy_turn(HANDLE con, entity_t *player, entity_t *enemy){
    int dmg_final = enemy->dmg + rand_int(0, enemy->dmg_vary) - player->armour;
    if(player->defending){
        dmg_final /= 2;
        player->defending = 0;
    }
    if(player->armour > dmg_final)
        dmg_final = 0;
    player->hp -= dmg_final;
    return dmg_final;
}
//승리 이벤트 
void victory(HANDLE con, entity_t *player, entity_t *enemy, inventory_t *inv){
    DWORD written;
    Sleep(2000);
    player->coins += enemy->coins; //플레이어 소지품에서 코인 추가 
	WriteConsoleA(con, "적은 짧은 단말마와 함께 스러졌고, 당신은 후들거리는 다리로 서있습니다.승리입니다!\n\n", 83, &written, NULL);
    drops(con, inv); //전리품 나타내기 
    Sleep(6000);
}
//패배 이벤트 
void defeat(HANDLE con){
    DWORD written;
	WriteConsoleA(con, "순간의 잘못된 판단이 최악의 결과를 가져왔습니다. 적의 이빨이 당신의 살을 꿰뚫으며 당신은 쓰러졌습니다...\n\n", 106, &written, NULL);
	Sleep(6000);
}
//플레이어 차례로 시작 
void player_turn(HANDLE con, entity_t *player, entity_t *enemy){
    DWORD written;
    char buffer[CONSOLE_COLS];

    int opt;
    player->escaped = 0;
    int p_dmg;

    sprintf(buffer, "\nPlayer: HP: %d/%d ATK: %d-%d\tEnemy: HP: %d/%d ATK: %d-%d\n", player->hp, player_max_hp, player->dmg, player->dmg + player->dmg_vary, enemy->hp, enemy->max_hp, enemy->dmg, enemy->dmg + enemy->dmg_vary);
    WriteConsoleA(con, buffer, strlen(buffer), &written, NULL);
    WriteConsoleA(con, "1.Attack\n2.Defend\n3.Run\n4.Item\n", 31, &written, NULL);

    do {
        opt = getch();
        if (opt < '1' || opt > '4') WriteConsoleA(con, "유효하지 않음!\n", 14, &written, NULL);
    } while (opt < '1' || opt > '4');

    switch(opt){
        case '1':
            if(rand_int(1, 7) == 1){// 1/7 확률로 공격이 빗나감 
                WriteConsoleA(con, "당신은 검을 휘들렀지만, 적이 경로를 읽고 피했습니다...\n", 80, &written, NULL);
                Sleep(1400);
            }
            else{//정상적인 타격이 들어감 
                p_dmg = player_attack(player, enemy);
                sprintf(buffer, "당신은 무기를 휘둘러 %d 의 피해를 주었습니다.\n", p_dmg);
                WriteConsoleA(con, buffer, strlen(buffer), &written, NULL);
                Sleep(1400);
            }
            break;

        case '2'://방어: 받는 데미지 경감 
            player->defending = 1;//상태: 방어중 
            WriteConsoleA(con, "당신은 적의 공격을 생각하여 방어자세를 취했습니다,\n다음턴 동안 방어가 증가합니다.\n", 82, &written, NULL);
            Sleep(3000);
            break;

        case '3':
		    WriteConsoleA(con, "당신은 승산이 없음을 본능적으로 느끼고,\n적에게 등을 보이며 달아났습니다...\n", 74, &written, NULL);
            Sleep(2000);
            if(rand() % 2 == 0){//50%확률로 도망 성공 
				Sleep(4000);
			}
			else{//실패시 적에게 1대 맞음 
				WriteConsoleA(con, "\n...하지만 도망도 제대로 치지 못하여 적에게 유효타를 허용했습니다...\n", 69, &written, NULL);

	            sprintf(buffer, "적이 당신에게 %d 의 피해를 줍니다..\n", enemy_turn(con, player, enemy));
                WriteConsoleA(con, buffer, strlen(buffer), &written, NULL);
                Sleep(5000);
			}
			player->escaped = 1;//플레이어 상태: 도망감 -> 맵 화면으로 전환 
			break;
        
        case '4'://인벤토리. 미구현 
            WriteConsoleA(con, "W.I.P\n", 6, &written, NULL);
            break;
    }

}
//적과의 조우시(X에 접근 시) 
void battle(HANDLE con, entity_t *player, inventory_t *inv){
    DWORD written;
    char buffer[CONSOLE_COLS];
    entity_t *enemy = make_enemy();
    player->defending = 0;
    int m_dmg;
    WriteConsoleA(con, "적이 당신을 급습했습니다!\n", 27, &written, NULL);

    while(1){//전투 => 별도 처리 전까지 반복 
        player_turn(con, player, enemy);
        //플레이어 체력 소진
        if(player->hp <= 0){ 
            defeat(con);
            return;
        }
		//플레이어 도망 
        if(player->escaped == 1){
            return;
        }
		//적을 이김 
        if(enemy->hp <= 0){
            victory(con, player, enemy, inv);
            return;
        }
        //적이 플레이어에게 준 피해 계산 
        m_dmg = enemy_turn(con, player, enemy);
	    sprintf(buffer, "적이 당신에게 %d 의 피해를 줍니다.\n", m_dmg);
        WriteConsoleA(con, buffer, strlen(buffer), &written, NULL);
	    Sleep(2500);
	    //화면 초기화 
	    system("cls");

        if(player->hp <= 0){
            defeat(con);
            return;
        }
    }
}
