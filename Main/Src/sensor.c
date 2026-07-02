/*
 * sensor.c
 *
 *  Created on: 2026. 6. 24.
 *      Author: kth59
 */

#include "sensor.h"
#include "st7735_lcd.h"
#include "gpio.h"
#include "main.h"
#include "adc.h"
#include "tim.h"

#define SENSOR_NUM 8
#define TIM_IR 		&htim6
#define TIM_SENSOR	&htim3
#define ADC_SENSOR	&hadc1

#define TIM_IR_IRQ_Handler HAL_TIM6_IRQ_Handler
#define SENSOR_IRQ_Handler HAL_ADC1_IRQ_Handler

typedef struct {
	GPIO_TypeDef *Port;
	uint16_t Pin;
} IR_TypeDef;

typedef struct {
	uint8_t index;
	uint8_t sensorRaw[SENSOR_NUM];
	uint8_t sensorBlackMax[SENSOR_NUM];
	uint8_t sensorWhiteMax[SENSOR_NUM];
	uint8_t sensorNormalized[SENSOR_NUM];
	uint8_t sensorState;
	uint8_t sensorThreshold;
} IR_DATA;

IR_DATA irData;

IR_TypeDef IR_Index[SENSOR_NUM] = { { .Port = IR_0_GPIO_Port, .Pin = IR_0_Pin },
		{ .Port = IR_1_GPIO_Port, .Pin = IR_1_Pin }, { .Port = IR_2_GPIO_Port,
				.Pin = IR_2_Pin }, { .Port = IR_3_GPIO_Port, .Pin = IR_3_Pin },
		{ .Port = IR_4_GPIO_Port, .Pin = IR_4_Pin }, { .Port = IR_5_GPIO_Port,
				.Pin = IR_5_Pin }, { .Port = IR_6_GPIO_Port, .Pin = IR_6_Pin },
		{ .Port = IR_7_GPIO_Port, .Pin = IR_7_Pin }, };

volatile uint32_t tim6_cnt = 0;
volatile uint32_t adc_cnt = 0;

__STATIC_INLINE void IR_Enable(uint8_t idx) {
	HAL_GPIO_WritePin((IR_Index + idx)->Port, (IR_Index + idx)->Pin,
			GPIO_PIN_SET);
}

__STATIC_INLINE void IR_Disable(uint8_t idx) {
	HAL_GPIO_WritePin((IR_Index + idx)->Port, (IR_Index + idx)->Pin,
			GPIO_PIN_RESET);
}

void TIM_IR_IRQ_Handler() {
	IR_Enable(irData.index);
	__HAL_TIM_SET_COUNTER(TIM_SENSOR, 0);
	__HAL_TIM_CLEAR_FLAG(TIM_SENSOR, TIM_FLAG_UPDATE);
	__HAL_TIM_ENABLE(TIM_SENSOR);
	tim6_cnt++;
//	HAL_ADC_Start(ADC_SENSOR);
}

void SENSOR_IRQ_Handler() {
	uint8_t idx = irData.index;
	adc_cnt++;
	uint16_t adc_raw = HAL_ADC_GetValue(ADC_SENSOR);
	IR_Disable(irData.index);

	//calc sensorNormalized, sensorState

	*(irData.sensorRaw + idx) = (uint8_t) (adc_raw >> 4);

	irData.index = (irData.index + 1) & 0x07;
}

void Sensor_Start() {
	HAL_ADCEx_Calibration_Start(ADC_SENSOR, ADC_SINGLE_ENDED);
	HAL_ADC_Start_IT(ADC_SENSOR);
	HAL_Delay(10);
	irData.index = 0;
	__HAL_TIM_CLEAR_FLAG(TIM_IR, TIM_FLAG_UPDATE);
	HAL_TIM_Base_Start_IT(TIM_IR);
}

void Sensor_Stop() {
	HAL_TIM_Base_Stop_IT(TIM_IR);
	HAL_ADC_Stop(ADC_SENSOR);
}

void Sensor_Calibration() {

}

void Sensor_Test_Raw() {
	while (1) {


	}
}

void Sensor_Test_Normalized() {

}

void Sensor_Test_State() {

}
