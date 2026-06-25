/*
 * motor.c
 *
 *  Created on: 2026. 6. 24.
 *      Author: kth59
 */

#include "motor.h"

#include "main.h"
#include "tim.h"
#include "gpio.h"

#define TIM_MOTOR_L &htim1
#define TIM_MOTOR_R &htim8

#define MOTOR_L_IRQ_Handler HAL_TIM1_IRQ_Handler
#define MOTOR_R_IRQ_Handler HAL_TIM8_IRQ_Handler

#define Check_Bit(num, bitMask)	((num & bitMask) ? GPIO_PIN_SET : GPIO_PIN_RESET)

typedef struct{
	GPIO_TypeDef* Port;
	uint16_t Pin;
} Motor_TypeDef;

Motor_TypeDef Motor_L[4] = {
		{.Port = MTR_L1_GPIO_Port, .Pin = MTR_L1_Pin},
		{.Port = MTR_L3_GPIO_Port, .Pin = MTR_L3_Pin},
		{.Port = MTR_L2_GPIO_Port, .Pin = MTR_L2_Pin},
		{.Port = MTR_L4_GPIO_Port, .Pin = MTR_L4_Pin},
};

Motor_TypeDef Motor_R[4] = {
		{.Port = MTR_R1_GPIO_Port, .Pin = MTR_R1_Pin},
		{.Port = MTR_R3_GPIO_Port, .Pin = MTR_R3_Pin},
		{.Port = MTR_R2_GPIO_Port, .Pin = MTR_R2_Pin},
		{.Port = MTR_R4_GPIO_Port, .Pin = MTR_R4_Pin},
};

const uint8_t stepSequence[8] = { 0b0001, 0b0011, 0b0010, 0b0110, 0b0100,
		0b1100, 0b1000, 0b1001 };

void MOTOR_L_IRQ_Handler() {
	static uint8_t index = 0;
	uint8_t outBit = *(stepSequence + index);
	HAL_GPIO_WritePin((Motor_L + 0)->Port, (Motor_L + 0)->Pin, Check_Bit(outBit, 0x1));
	HAL_GPIO_WritePin((Motor_L + 1)->Port, (Motor_L + 1)->Pin, Check_Bit(outBit, 0x2));
	HAL_GPIO_WritePin((Motor_L + 2)->Port, (Motor_L + 2)->Pin, Check_Bit(outBit, 0x4));
	HAL_GPIO_WritePin((Motor_L + 3)->Port, (Motor_L + 3)->Pin, Check_Bit(outBit, 0x8));
	index = (index + 1) & 0x7;
}

void MOTOR_R_IRQ_Handler() {
	static uint8_t index = 0;
	uint8_t outBit = *(stepSequence + index);
	HAL_GPIO_WritePin((Motor_R + 0)->Port, (Motor_R + 0)->Pin, Check_Bit(outBit, 0x1));
	HAL_GPIO_WritePin((Motor_R + 1)->Port, (Motor_R + 1)->Pin, Check_Bit(outBit, 0x2));
	HAL_GPIO_WritePin((Motor_R + 2)->Port, (Motor_R + 2)->Pin, Check_Bit(outBit, 0x4));
	HAL_GPIO_WritePin((Motor_R + 3)->Port, (Motor_R + 3)->Pin, Check_Bit(outBit, 0x8));
	index = (index + 1) & 0x7;
}

__STATIC_INLINE uint16_t Calc_ARR(TIM_TypeDef *htim, float_t velocity){
	UNUSED(htim);
	UNUSED(velocity);
	return UINT16_MAX;
}

void Motor_Start(){
	HAL_TIM_Base_Start_IT(TIM_MOTOR_L);
	HAL_TIM_Base_Start_IT(TIM_MOTOR_R);
}


void Motor_Stop() {
	HAL_TIM_Base_Stop_IT(TIM_MOTOR_L);
	HAL_TIM_Base_Stop_IT(TIM_MOTOR_R);
}
