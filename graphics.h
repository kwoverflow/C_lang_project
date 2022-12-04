#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_


#include <stdio.h>
#include <Windows.h>

#include "mapgen.h"
#include "const.h"
#include "global.h"

void draw_border(HANDLE con);//칸에 맞는 경계 만들기 
void console_cursor_hide(HANDLE con);//커서 숨기기 
void console_clear(HANDLE con);//콘솔 초기화 -> 초기상태 
void console_set_size(int width, int height);//글자 사이즈 맞추어 콘솔사이즈 지정 
int display_dialogue_box(HANDLE con, char *text, char **opts, int nopts);//대화상자 노출시키기 
void draw(HANDLE con, world_t *w);//던전 그리기 
void draw_town(HANDLE con, world_t *w);//마을 그리기 

#endif
