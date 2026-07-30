/*
 * motor.c
 *
 *  Created on: 2026. 6. 24.
 *      Author: kth59
 */

#include "motor.h"

#include "button.h"
#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "dac.h"

#define TIM_MOTOR_L &htim1
#define TIM_MOTOR_R &htim8

#define MOTOR_L_IRQ_Handler HAL_TIM1_IRQ_Handler
#define MOTOR_R_IRQ_Handler HAL_TIM8_IRQ_Handler

#define Check_Bit(num, bitMask)	((num & bitMask) ? GPIO_PIN_SET : GPIO_PIN_RESET)

typedef struct {
	GPIO_TypeDef *Port;
	uint16_t Pin;
} Motor_TypeDef;

Motor_TypeDef Motor_L[4] = { { .Port = MTR_L1_GPIO_Port, .Pin = MTR_L1_Pin }, {
		.Port = MTR_L3_GPIO_Port, .Pin = MTR_L3_Pin }, { .Port =
MTR_L2_GPIO_Port, .Pin = MTR_L2_Pin }, { .Port = MTR_L4_GPIO_Port, .Pin =
MTR_L4_Pin }, };

Motor_TypeDef Motor_R[4] = { { .Port = MTR_R1_GPIO_Port, .Pin = MTR_R1_Pin }, {
		.Port = MTR_R3_GPIO_Port, .Pin = MTR_R3_Pin }, { .Port =
MTR_R2_GPIO_Port, .Pin = MTR_R2_Pin }, { .Port = MTR_R4_GPIO_Port, .Pin =
MTR_R4_Pin }, };

const uint8_t stepSequence[8] = { 0b0001, 0b0011, 0b0010, 0b0110, 0b0100,
		0b1100, 0b1000, 0b1001 };

void MOTOR_L_IRQ_Handler() {
	static uint8_t index = 0;
	uint8_t outBit = *(stepSequence + index);
	HAL_GPIO_WritePin((Motor_L + 0)->Port, (Motor_L + 0)->Pin,
			Check_Bit(outBit, 0x1));
	HAL_GPIO_WritePin((Motor_L + 1)->Port, (Motor_L + 1)->Pin,
			Check_Bit(outBit, 0x2));
	HAL_GPIO_WritePin((Motor_L + 2)->Port, (Motor_L + 2)->Pin,
			Check_Bit(outBit, 0x4));
	HAL_GPIO_WritePin((Motor_L + 3)->Port, (Motor_L + 3)->Pin,
			Check_Bit(outBit, 0x8));
	index = (index + 1) & 0x7;
}

void MOTOR_R_IRQ_Handler() {
	static uint8_t index = 0;
	uint8_t outBit = *(stepSequence + index);
	HAL_GPIO_WritePin((Motor_R + 0)->Port, (Motor_R + 0)->Pin,
			Check_Bit(outBit, 0x1));
	HAL_GPIO_WritePin((Motor_R + 1)->Port, (Motor_R + 1)->Pin,
			Check_Bit(outBit, 0x2));
	HAL_GPIO_WritePin((Motor_R + 2)->Port, (Motor_R + 2)->Pin,
			Check_Bit(outBit, 0x4));
	HAL_GPIO_WritePin((Motor_R + 3)->Port, (Motor_R + 3)->Pin,
			Check_Bit(outBit, 0x8));
	index = (index + 1) & 0x7;
}

__STATIC_INLINE uint16_t Calc_ARR(TIM_TypeDef *htim, float_t velocity) {
	UNUSED(htim);
	UNUSED(velocity);
	return UINT16_MAX;
}

__STATIC_INLINE void Motor_Vref_Set(uint16_t value) {
	// value 값은 최대 12-bit => 4095 ~ 0 => 3.3 ~ 0 V
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, value);

}

void Motor_Start() {
	// Vref 전압 활성화
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
	// Vref 전압 설정 (value 값 조절해야 함)
//	Motor_Vref_Set(value);

	HAL_TIM_Base_Start_IT(TIM_MOTOR_L);
	HAL_TIM_Base_Start_IT(TIM_MOTOR_R);
}

void Motor_Stop() {
	Motor_Vref_Set(0);
	HAL_DAC_Stop(&hdac1, DAC_CHANNEL_2);
	HAL_TIM_Base_Stop_IT(TIM_MOTOR_L);
	HAL_TIM_Base_Stop_IT(TIM_MOTOR_R);
}

void Motor_Test_Phase() {
	uint8_t index = 0;

	// Vref 전압 활성화
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
	// Vref 1.65 V 지정 (이후 값 변경해도 됨 - 전류 과다 -> 값 내리기, 전류 필요 -> 값 올리기)
	Motor_Vref_Set(2048);

	// 버튼 입력
	UserInput_t btn = INPUT_CMD_NONE;

	while (1) {
		// 버튼 입력 있을 때까지 고정
		// 만약 버튼 입력이 될 때까지 버튼 입력 고정을 안하면 모터는 계속 상을 잡기 때문에,
		// 버튼 입력이 있을 때까지 무한 구문에서 안벗어남
		while ((btn = Button_Get_Input()) == INPUT_CMD_NONE)
			;

		// 버튼에 따른 event 작성

		// 모터 상 바꾸기
		uint8_t outBit = *(stepSequence + index);
		HAL_GPIO_WritePin((Motor_L + 0)->Port, (Motor_L + 0)->Pin,
				Check_Bit(outBit, 0x1));
		HAL_GPIO_WritePin((Motor_L + 1)->Port, (Motor_L + 1)->Pin,
				Check_Bit(outBit, 0x2));
		HAL_GPIO_WritePin((Motor_L + 2)->Port, (Motor_L + 2)->Pin,
				Check_Bit(outBit, 0x4));
		HAL_GPIO_WritePin((Motor_L + 3)->Port, (Motor_L + 3)->Pin,
				Check_Bit(outBit, 0x8));
		HAL_GPIO_WritePin((Motor_R + 0)->Port, (Motor_R + 0)->Pin,
				Check_Bit(outBit, 0x1));
		HAL_GPIO_WritePin((Motor_R + 1)->Port, (Motor_R + 1)->Pin,
				Check_Bit(outBit, 0x2));
		HAL_GPIO_WritePin((Motor_R + 2)->Port, (Motor_R + 2)->Pin,
				Check_Bit(outBit, 0x4));
		HAL_GPIO_WritePin((Motor_R + 3)->Port, (Motor_R + 3)->Pin,
				Check_Bit(outBit, 0x8));

		// 100 ms 지연
		HAL_Delay(100);

		HAL_GPIO_WritePin((Motor_L + 0)->Port, (Motor_L + 0)->Pin, 0);
		HAL_GPIO_WritePin((Motor_L + 1)->Port, (Motor_L + 1)->Pin, 0);
		HAL_GPIO_WritePin((Motor_L + 2)->Port, (Motor_L + 2)->Pin, 0);
		HAL_GPIO_WritePin((Motor_L + 3)->Port, (Motor_L + 3)->Pin, 0);
		HAL_GPIO_WritePin((Motor_R + 0)->Port, (Motor_R + 0)->Pin, 0);
		HAL_GPIO_WritePin((Motor_R + 1)->Port, (Motor_R + 1)->Pin, 0);
		HAL_GPIO_WritePin((Motor_R + 2)->Port, (Motor_R + 2)->Pin, 0);
		HAL_GPIO_WritePin((Motor_R + 3)->Port, (Motor_R + 3)->Pin, 0);
	}
}
