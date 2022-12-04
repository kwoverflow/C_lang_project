#include "graphics.h"

char *concat(char *str1, char *str2) {
    char *output = (char*)malloc(strlen(str1) + strlen(str2) + 1);
    strcpy(output, str1);
    strcat(output, str2);
    return output;
}
//플레이중 커서가 깜빡여 방해하는 것 방지 
void console_cursor_hide(HANDLE con) {
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(con, &info);
}
//화면 전환시 화면 초기화 
void console_clear(HANDLE con) {
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;
    GetConsoleScreenBufferInfo(con, &screen);
    FillConsoleOutputCharacterA(con, ' ', screen.dwSize.X * screen.dwSize.Y, (COORD){0, 0}, &written);
    SetConsoleCursorPosition(con, (COORD){0, 0});
}
//const에서 가져온 사이즈로 지정하기
//C언어 char로 만든 map이라 임의로 확장시 깨지기 때문에 
void console_set_size(int width, int height) {
    HWND window = GetConsoleWindow();
    RECT rect;
    GetWindowRect(window, &rect);
    MoveWindow(window, rect.left, rect.top, width, height, TRUE);
}
//상위에 노출되는 npc와의 대화 항목 
int display_dialogue_box(HANDLE con, char *text, char **opts, int nopts) {
	//초기화 
    int len = 0, cur_len = 0, max_len = 0;
    int selected = 0;
    DWORD written;
    CONSOLE_SCREEN_BUFFER_INFO screen;
    char *buffer = (char*)malloc(CONSOLE_COLS * nopts);
    SetConsoleCursorPosition(con, (COORD){0, 1});
    WriteConsoleA(con, text, strlen(text), &written, NULL);
    GetConsoleScreenBufferInfo(con, &screen);
    max_len = strlen(text);
    //대화 노출시키기 
    for (int i = 0; i < nopts; ++i) {
        len += sprintf(buffer + len, " %s\n", opts[i]);
        cur_len = snprintf(NULL, 0, " %s\n", opts[i]);
        if (cur_len > max_len) max_len = cur_len;
    }
    for (int i = 1; i < nopts + screen.dwCursorPosition.Y; ++i) {
        FillConsoleOutputCharacterA(con, ' ', max_len, (COORD){0, i}, &written);
    }
    SetConsoleCursorPosition(con, (COORD){0, 1});
    WriteConsoleA(con, text, strlen(text), &written, NULL);
    WriteConsoleA(con, buffer, strlen(buffer), &written, NULL);
    //사이즈 맞추어 대화상자 만들기 
    FillConsoleOutputCharacterA(con, '-', max_len, (COORD){0, nopts + screen.dwCursorPosition.Y}, &written);
    for (int i = 1; i <= nopts + screen.dwCursorPosition.Y; ++i) FillConsoleOutputCharacterA(con, '|', 1, (COORD){max_len, i}, &written);
    FillConsoleOutputCharacterA(con, '+', 1, (COORD){max_len, nopts + screen.dwCursorPosition.Y}, &written);
	//플레이어 선택지 표시 
    FillConsoleOutputCharacterA(con, '>', 1, (COORD){0, screen.dwCursorPosition.Y}, &written);
    while (1) {
        getch();
        FillConsoleOutputCharacterA(con, ' ', 1, (COORD){0, selected + screen.dwCursorPosition.Y}, &written);

        if ((GetAsyncKeyState(0x57) & 0x8000) != 0) selected--;
        else if ((GetAsyncKeyState(0x53) & 0x8000) != 0) selected++;
        else if ((GetAsyncKeyState(0x45) & 0x8000) != 0) break;

        if (selected < 0) selected = 0;
        if (selected > nopts - 1) selected = nopts - 1;
        FillConsoleOutputCharacterA(con, '>', 1, (COORD){0, selected + screen.dwCursorPosition.Y}, &written);
    }
    free(buffer);
    return selected;
}

void draw(HANDLE con, world_t *w) {
    DWORD written;
    char status[COLS + 1];
    char *frame;
    //맨 윗1줄에 간단한 정보 표시 
    sprintf(status, "HP: %d/%d | DMG: %d | COINS: %d | LVL: %d | ENEMIES: %d ", w->player->hp, player_max_hp, w->player->dmg, w->player->coins, w->depth, w->enemy_count);
    for (int i = strlen(status); i < COLS + 1; ++i) {
        status[i] = '=';
    }
    status[COLS] = '\0';
    if (cheat == 0) {
        apply_fov(w, fov_map, 10, 5);
        frame = concat(status, fov_map);
    }
    else frame = concat(status, w->map);
    WriteConsoleOutputCharacterA(con, frame, CONSOLE_ROWS * CONSOLE_COLS, (COORD){0, 0}, &written);
}
void draw_town(HANDLE con, world_t *w) {
    DWORD written;
    WriteConsoleOutputCharacterA(con, town_art, CONSOLE_ROWS * CONSOLE_COLS, (COORD){0, 0}, &written);
    FillConsoleOutputCharacterA(con, PLAYER, 1, (COORD){w->player->x, w->player->y}, &written);
}
//경계선 그리기 
void draw_border(HANDLE con) {
    DWORD written;
    FillConsoleOutputCharacterA(con, 201, 1, (COORD){0, 0}, &written);
    FillConsoleOutputCharacterA(con, 187, 1, (COORD){CONSOLE_COLS - 1, 0}, &written);
    FillConsoleOutputCharacterA(con, 200, 1, (COORD){0, CONSOLE_ROWS - 1}, &written);
    FillConsoleOutputCharacterA(con, 188, 1, (COORD){CONSOLE_COLS - 1, CONSOLE_ROWS - 1}, &written);
    FillConsoleOutputCharacterA(con, 205, CONSOLE_COLS - 2, (COORD){1, 0}, &written);
    FillConsoleOutputCharacterA(con, 205, CONSOLE_COLS - 2, (COORD){1, CONSOLE_ROWS - 1}, &written);

    for (int i = 1; i <= CONSOLE_ROWS - 2; ++i) {
        FillConsoleOutputCharacterA(con, 186, 1, (COORD){0, i}, &written);
        FillConsoleOutputCharacterA(con, 186, 1, (COORD){CONSOLE_COLS - 1, i}, &written);
    }

    SetConsoleCursorPosition(con, (COORD){1, CONSOLE_ROWS - 2});
    FillConsoleOutputCharacterA(con, 205, 45, (COORD){1, CONSOLE_ROWS - 3}, &written);

    FillConsoleOutputCharacterA(con, 204, 1, (COORD){0, CONSOLE_ROWS - 3}, &written);
    FillConsoleOutputCharacterA(con, 202, 1, (COORD){46, CONSOLE_ROWS - 1}, &written);
    FillConsoleOutputCharacterA(con, 187, 1, (COORD){46, CONSOLE_ROWS - 3}, &written);
    FillConsoleOutputCharacterA(con, 186, 1, (COORD){46, CONSOLE_ROWS - 2}, &written);
}
