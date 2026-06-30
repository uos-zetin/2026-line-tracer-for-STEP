#include "custom_lcd.h"
#include <stdio.h>

/* ST7735 및 ST7789 드라이버에 전역으로 공통 선언된 변수와 함수를 가져옴 */
extern uint16_t LCD_POINT_COLOR;
extern void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode);
extern void LCD_Clear(void);
extern void LCD_SetBrightness(uint32_t Brightness);

static Custom_LCD_Type_t current_lcd_type = LCD_TYPE_ST7789;

static uint16_t posX = 0;
static uint16_t posY = 0;
static uint16_t color = LCD_COLOR_WHITE;
static uint8_t size = 12; // 기본 사이즈 12 (6x12)

void Custom_LCD_Init(Custom_LCD_Type_t type) {
    current_lcd_type = type;
}

void Custom_LCD_Clear(void) {
    LCD_Clear();
    posX = 0;
    posY = 0;
    color = LCD_COLOR_WHITE;
    size = 12;
}

void Custom_LCD_SetBrightness(uint32_t brightness) {
    LCD_SetBrightness(brightness);
}

void Custom_LCD_Printf(const char *format, ...) {
    char buffer[512];

    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    int cursor = 0;

    while (buffer[cursor]) {
        if (buffer[cursor] == '/') {
            char nextChar = buffer[cursor + 1];

            if (nextChar == '/') {
                cursor += 1;
            } else {
                switch (nextChar) {
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                    posX = 0;
                    // 줄 바꿈: 이전 줄의 글자 크기(size)에 영향을 받지 않도록 고정된 줄 높이(16)를 사용합니다.
                    // 최대 폰트 크기가 16이므로 16 픽셀 간격으로 줄을 나누면 겹치지 않고 일정한 간격이 유지됩니다.
                    posY = 16 * (nextChar - '0'); 
                    break;
                // 색상 변경 서식
                case 'w': color = LCD_COLOR_WHITE;   break;
                case 'r': color = LCD_COLOR_RED;     break;
                case 'g': color = LCD_COLOR_GREEN;   break;
                case 'b': color = LCD_COLOR_BLUE;    break;
                case 'y': color = LCD_COLOR_YELLOW;  break;
                case 'c': color = LCD_COLOR_CYAN;    break;
                case 'm': color = LCD_COLOR_MAGENTA; break;
                case 'o': color = LCD_COLOR_ORANGE;  break;
                case 'l': color = LCD_COLOR_LIME;    break;
                case 't': color = LCD_COLOR_MINT;    break;
                case 's': color = LCD_COLOR_SEA;     break;
                case 'v': color = LCD_COLOR_VIOLET;  break;
                case 'p': color = LCD_COLOR_ROSE;    break;
                case 'K': color = LCD_COLOR_GRAY;    break;
                case 'k': color = LCD_COLOR_BLACK;   break;
                // 크기 변경 서식
                case 'A': size = 16;                 break;
                case 'a': size = 12;                 break;
                case 'C': Custom_LCD_Clear();        break; // 화면 지우기 및 커서 초기화
                // HEX 색상 변경
                case '#':
                    {
                        uint32_t colordata = 0;
                        for(uint32_t i = 0; i < 6; i++){
                            if(buffer[cursor + i + 2] >= 'A' && buffer[cursor + i + 2] <= 'F'){
                                colordata *= 16;
                                colordata += buffer[cursor + i + 2] - 'A' + 10;
                            }
                            else if(buffer[cursor + i + 2] >= 'a' && buffer[cursor + i + 2] <= 'f'){
                                colordata *= 16;
                                colordata += buffer[cursor + i + 2] - 'a' + 10;
                            }
                            else if(buffer[cursor + i + 2] >= '0' && buffer[cursor + i + 2] <= '9'){
                                colordata *= 16;
                                colordata += buffer[cursor + i + 2] - '0';
                            }
                        }
                        color = ((colordata >> 8) & 0xF800) | ((colordata >> 5) & 0x07E0) | ((colordata >> 3) & 0x001F);
                    }
                    cursor += 6;
                    break;
                }

                cursor += 2;
                continue;
            }
        }

        // 실제 문자 출력
        uint16_t old_color = LCD_POINT_COLOR;
        LCD_POINT_COLOR = color;
        
        LCD_ShowChar(posX, posY, buffer[cursor], size, 0);
        
        LCD_POINT_COLOR = old_color;

        // 글자 폭만큼 X 위치 이동 (12사이즈는 6폭, 16사이즈는 8폭)
        posX += (size / 2) + 1; 
        cursor++;
    }
}
