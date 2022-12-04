#include <Windows.h>
#include <MMSystem.h>

#include "battle.h"
//���鸶�� ���� ����°��� ��ȿ����
//struct�� ü��ȭ �ؼ� ȣ�� 
enemy_t *make_enemy() {
    entity_t *e = (enemy_t*)malloc(sizeof(enemy_t));
    e->dmg = 3 + rand_int(0, 3);
    e->dmg_vary = 3;
    e->max_hp = 20 + rand_int(0, 10);
    e->hp = e->max_hp;
    e->coins = rand_int(5, 10);
    return e;
} 
//���� �÷��̾�� �ִ� ������ ��� 
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
//�¸� �̺�Ʈ 
void victory(HANDLE con, entity_t *player, entity_t *enemy, inventory_t *inv){
    DWORD written;
    Sleep(2000);
    player->coins += enemy->coins; //�÷��̾� ����ǰ���� ���� �߰� 
	WriteConsoleA(con, "���� ª�� �ܸ����� �Բ� ��������, ����� �ĵ�Ÿ��� �ٸ��� ���ֽ��ϴ�.�¸��Դϴ�!\n\n", 83, &written, NULL);
    drops(con, inv); //����ǰ ��Ÿ���� 
    Sleep(6000);
}
//�й� �̺�Ʈ 
void defeat(HANDLE con){
    DWORD written;
	WriteConsoleA(con, "������ �߸��� �Ǵ��� �־��� ����� �����Խ��ϴ�. ���� �̻��� ����� ���� ������� ����� ���������ϴ�...\n\n", 106, &written, NULL);
	Sleep(6000);
}
//�÷��̾� ���ʷ� ���� 
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
        if (opt < '1' || opt > '4') WriteConsoleA(con, "��ȿ���� ����!\n", 14, &written, NULL);
    } while (opt < '1' || opt > '4');

    switch(opt){
        case '1':
            if(rand_int(1, 7) == 1){// 1/7 Ȯ���� ������ ������ 
                WriteConsoleA(con, "����� ���� �ֵ鷶����, ���� ��θ� �а� ���߽��ϴ�...\n", 80, &written, NULL);
                Sleep(1400);
            }
            else{//�������� Ÿ���� �� 
                p_dmg = player_attack(player, enemy);
                sprintf(buffer, "����� ���⸦ �ֵѷ� %d �� ���ظ� �־����ϴ�.\n", p_dmg);
                WriteConsoleA(con, buffer, strlen(buffer), &written, NULL);
                Sleep(1400);
            }
            break;

        case '2'://���: �޴� ������ �氨 
            player->defending = 1;//����: ����� 
            WriteConsoleA(con, "����� ���� ������ �����Ͽ� ����ڼ��� ���߽��ϴ�,\n������ ���� �� �����մϴ�.\n", 82, &written, NULL);
            Sleep(3000);
            break;

        case '3':
		    WriteConsoleA(con, "����� �»��� ������ ���������� ������,\n������ ���� ���̸� �޾Ƴ����ϴ�...\n", 74, &written, NULL);
            Sleep(2000);
            if(rand() % 2 == 0){//50%Ȯ���� ���� ���� 
				Sleep(4000);
			}
			else{//���н� ������ 1�� ���� 
				WriteConsoleA(con, "\n...������ ������ ����� ġ�� ���Ͽ� ������ ��ȿŸ�� ����߽��ϴ�...\n", 69, &written, NULL);

	            sprintf(buffer, "���� ��ſ��� %d �� ���ظ� �ݴϴ�..\n", enemy_turn(con, player, enemy));
                WriteConsoleA(con, buffer, strlen(buffer), &written, NULL);
                Sleep(5000);
			}
			player->escaped = 1;//�÷��̾� ����: ������ -> �� ȭ������ ��ȯ 
			break;
        
        case '4'://�κ��丮. �̱��� 
            WriteConsoleA(con, "W.I.P\n", 6, &written, NULL);
            break;
    }

}
//������ �����(X�� ���� ��) 
void battle(HANDLE con, entity_t *player, inventory_t *inv){
    DWORD written;
    char buffer[CONSOLE_COLS];
    entity_t *enemy = make_enemy();
    player->defending = 0;
    int m_dmg;
    WriteConsoleA(con, "���� ����� �޽��߽��ϴ�!\n", 27, &written, NULL);

    while(1){//���� => ���� ó�� ������ �ݺ� 
        player_turn(con, player, enemy);
        //�÷��̾� ü�� ����
        if(player->hp <= 0){ 
            defeat(con);
            return;
        }
		//�÷��̾� ���� 
        if(player->escaped == 1){
            return;
        }
		//���� �̱� 
        if(enemy->hp <= 0){
            victory(con, player, enemy, inv);
            return;
        }
        //���� �÷��̾�� �� ���� ��� 
        m_dmg = enemy_turn(con, player, enemy);
	    sprintf(buffer, "���� ��ſ��� %d �� ���ظ� �ݴϴ�.\n", m_dmg);
        WriteConsoleA(con, buffer, strlen(buffer), &written, NULL);
	    Sleep(2500);
	    //ȭ�� �ʱ�ȭ 
	    system("cls");

        if(player->hp <= 0){
            defeat(con);
            return;
        }
    }
}
