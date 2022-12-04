#include "inv.h"
#include "util.h"
//인벤토리에 있는 아이템 수량 가져오기 
int inventory_calc_size(inventory_t *inv) {
    int size = 0;
    for (int i = 0; i < ITEM_COUNT; ++i) {
        size += inv->items[i].quantity;
    }
    return size;
}
//인벤토리 그리기 
void inventory_draw(HANDLE con, inventory_t *inv) {
    DWORD written;
    char buffer[CONSOLE_COLS * ITEM_COUNT];
    int key;
    int selected = 0;
    int items = 0;
    int skipped = 0;
    int true_selected = 0;
    int len = 0;
    int current_len = 0;
    int max_len = 0;
    SetConsoleCursorPosition(con, (COORD){0, 1}); //윗칸의 상태장과 conflict 안나게 하기 

    max_len = sprintf(buffer, "Equipped: %s\n", (inv->equipped == -1) ? "nothing" : inv->items[inv->equipped].name);

    WriteConsoleA(con, buffer, strlen(buffer), &written, NULL);

    //인벤토리의 아이템이름 길이에 맞추어 개수 표시하기 
    for (int i = 0; i < ITEM_COUNT; ++i) {
        if (inv->items[i].quantity >= 0) {
            len += sprintf(buffer + len, " %s - %d\n", inv->items[i].name, inv->items[i].quantity);
            current_len = snprintf(NULL, 0, " %s - %d\n", inv->items[i].name, inv->items[i].quantity);
            if (current_len > max_len) max_len = current_len;
            items++;
        }
        else skipped++;
    }
	//비어있는 상태 
    for (int i = 0; i < items; ++i)
        FillConsoleOutputCharacterA(con, ' ', max_len, (COORD){0, items + i + 1}, &written);
    WriteConsoleA(con, buffer, strlen(buffer), &written, NULL);
	//인벤토리창 테두리 그리기 
    FillConsoleOutputCharacterA(con, '-', max_len, (COORD){0, items + 2}, &written);
    for (int i = 1; i <= items + 2; ++i) FillConsoleOutputCharacterA(con, '|', 1, (COORD){max_len, i}, &written);
    FillConsoleOutputCharacterA(con, '+', 1, (COORD){max_len, items + 2}, &written);
    //플레이어 선택 항목 
    FillConsoleOutputCharacterA(con, '>', 1, (COORD){0, selected + 2}, &written);
    while (1) {
        key = getch();
        FillConsoleOutputCharacterA(con, ' ', 1, (COORD){0, selected + 2}, &written);
		//선택시 -1, 해제시 +1 
        if ((GetAsyncKeyState(0x57) & 0x8000) != 0) {
            selected--;
        }
        else if ((GetAsyncKeyState(0x53) & 0x8000) != 0) {
            selected++;
        }
        else if ((GetAsyncKeyState(VK_TAB) & 0x8000) != 0) {
            return;
        }
        else if ((GetAsyncKeyState(0x45) & 0x8000) != 0) {
            inv->equipped = selected;
        }
        if (selected < 0) selected = 0;
        if (selected > items - 1) selected = items - 1;
        FillConsoleOutputCharacterA(con, '>', 1, (COORD){0, selected + 2}, &written);
        //장착한 항목 표시하기. 
		//idle상태는 nothing으로 표기 
        SetConsoleCursorPosition(con, (COORD){0, 1});
        len = sprintf(buffer, "Equipped: %s\n", (inv->equipped == -1) ? "nothing" : inv->items[inv->equipped].name);
        FillConsoleOutputCharacterA(con, ' ', max_len, (COORD){0, 1}, &written);
        for (int i = 1; i <= items + 2; ++i) FillConsoleOutputCharacterA(con, ' ', 1, (COORD){max_len, i}, &written);
        if (len > max_len) max_len = len;
        
        WriteConsoleA(con, buffer, strlen(buffer), &written, NULL);

        FillConsoleOutputCharacterA(con, '-', max_len, (COORD){0, items + 2}, &written);
        
        for (int i = 1; i <= items + 2; ++i) FillConsoleOutputCharacterA(con, '|', 1, (COORD){max_len, i}, &written);
        FillConsoleOutputCharacterA(con, '+', 1, (COORD){max_len, items + 2}, &written);
    }
}
//인벤토리 변동사항 저장하기 
void inventory_init(inventory_t *inv, int quantities[]) {
    for (int i = 0; i < ITEM_COUNT; ++i) {
        inv->items[i].quantity = quantities[i];
        inv->items[i].name = item_names[i];
        inv->items[i].dmg = 0;
    }
    inv->equipped = -1;
}
//아이템 드랍 확인 
void drops(HANDLE con, inventory_t *inv){
    DWORD written;
    char buffer[CONSOLE_COLS];

    int drop = rand() % 6;
    int quan;
	//아이템 랜덤 드랍 
    if(drop < 4){
        inv->items[drop].quantity++;
        sprintf(buffer, "적이 %s를 떨어트렸습니다!\n", inv->items[drop].name);
        WriteConsoleA(con, buffer, strlen(buffer), &written, NULL);
    }
	//랜덤 개수 드랍 
    else if(drop == 4){
        quan = rand_int(2, 4);
        inv->items[6].quantity += quan;
        sprintf(buffer, "적이 %d 개의 원석조각을 떨어뜨렸습니다!\n", quan);
        WriteConsoleA(con, buffer, strlen(buffer), &written, NULL);
    }
}

void use(HANDLE con, inventory_t *inv){
	//int usage = selected;
	
}

