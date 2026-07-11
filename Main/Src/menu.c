/*
 * menu.c
 *
 *  Created on: 2026. 6. 24.
 *      Author: kth59
 */

#include "menu.h"
#include "sensor.h"
#include "motor.h"
#include "drive.h"
#include "custom_lcd.h"
#include "button.h"

typedef struct {
	char name[16];
	void (*func)(void);
} Menu_TypeDef;

Menu_TypeDef MENU = {

};

void Main_Menu(){
    static uint8_t init_done = 0;

    // 1. 초기 1회만 실행되는 화면 그리기
    if (!init_done) {
        // 실제 사용 중인 LCD 타입에 맞게 설정 (ST7735 또는 ST7789)
        Custom_LCD_Init(LCD_TYPE_ST7735); 
        Custom_LCD_Clear();
        
        init_done = 1;
    }

    // 2. 메인 메뉴 로직 구현
    UserInput_t btn_input = Button_Get_Input();

    if (btn_input != INPUT_CMD_NONE) {
        // 버튼 입력에 따른 처리 로직 작성
    }
}
