#ifndef _VVC_W1209_MAIN_H
#define _VVC_W1209_MAIN_H

#include <stdint.h>

#define F_CPU     2000000UL

// Register definitions.
// GPIO
#define GPIOA_ODR *(volatile uint8_t *)(0x5000)
#define GPIOA_DDR *(volatile uint8_t *)(0x5002)
#define GPIOA_CR1 *(volatile uint8_t *)(0x5003)
#define GPIOB_ODR *(volatile uint8_t *)(0x5005)
#define GPIOB_DDR *(volatile uint8_t *)(0x5007)
#define GPIOB_CR1 *(volatile uint8_t *)(0x5008)
#define GPIOC_ODR *(volatile uint8_t *)(0x500A)
#define GPIOC_IDR *(volatile uint8_t *)(0x500B)
#define GPIOC_DDR *(volatile uint8_t *)(0x500C)
#define GPIOC_CR1 *(volatile uint8_t *)(0x500D)
#define GPIOD_ODR *(volatile uint8_t *)(0x500F)
#define GPIOD_IDR *(volatile uint8_t *)(0x5010)
#define GPIOD_DDR *(volatile uint8_t *)(0x5011)
#define GPIOD_CR1 *(volatile uint8_t *)(0x5012)
// ADC
#define ADC1_CSR  *(volatile uint8_t *)(0x5400)
#define ADC1_CR1  *(volatile uint8_t *)(0x5401)
#define ADC1_CR2  *(volatile uint8_t *)(0x5402)
#define ADC1_DRH  *(volatile uint8_t *)(0x5404)
#define ADC1_DRL  *(volatile uint8_t *)(0x5405)
// GPIO Macros
#define GPIOA_BASE      (0x5000)
#define GPIOB_BASE      (0x5005)
#define GPIOC_BASE      (0x500A)
#define GPIOD_BASE      (0x500F)
#define GPIO_ODR        (0x0000)
#define GPIO_IDR        (0x0001)
#define GPIO_DDR        (0x0002)
#define GPIO_CR1        (0x0003)
#define GPIOx_REG(addr, offset) (*(volatile uint8_t *)(addr + offset))

// Pin mappings.
// (Output)
#define RELAY_PA    (3)
#define SEG_D1_PD   (4)
#define SEG_D2_PB   (5)
#define SEG_D3_PB   (4)
#define SEG_LA_PD   (5)
#define SEG_LB_PA   (1)
#define SEG_LC_PC   (7)
#define SEG_LD_PD   (3)
#define SEG_LE_PD   (1)
#define SEG_LF_PA   (2)
#define SEG_LG_PC   (6)
#define SEG_DP_PD   (2)
// (Input)
#define KEY_SET_PC  (3)
#define KEY_PLS_PC  (4)
#define KEY_MIN_PC  (5)
#define RES_IN_PD   (6)

// Port/pin macro mappings.
#define RELAY_PIN    GPIOA_BASE, RELAY_PA
#define SEG_D1_PIN   GPIOD_BASE, SEG_D1_PD
#define SEG_D2_PIN   GPIOB_BASE, SEG_D2_PB
#define SEG_D3_PIN   GPIOB_BASE, SEG_D3_PB
#define SEG_LA_PIN   GPIOD_BASE, SEG_LA_PD
#define SEG_LB_PIN   GPIOA_BASE, SEG_LB_PA
#define SEG_LC_PIN   GPIOC_BASE, SEG_LC_PC
#define SEG_LD_PIN   GPIOD_BASE, SEG_LD_PD
#define SEG_LE_PIN   GPIOD_BASE, SEG_LE_PD
#define SEG_LF_PIN   GPIOA_BASE, SEG_LF_PA
#define SEG_LG_PIN   GPIOC_BASE, SEG_LG_PC
#define SEG_DP_PIN   GPIOD_BASE, SEG_DP_PD

// Global constants.
#define RELAY_MS    2000
//#define DB_MS       200
#define DB_MS       10

// Some statically-allocated variables.
uint32_t delay_i;
uint16_t adc_r;
volatile uint8_t set_button_down = 0;
//float    temp_f;
//float    temp_c;

static inline void GPIOx_PP_ON(uint16_t gpiox, uint8_t pin) {
  GPIOx_REG(gpiox, GPIO_ODR) |= (1 << pin);
}
static inline void GPIOx_PP_OFF(uint16_t gpiox, uint8_t pin) {
  GPIOx_REG(gpiox, GPIO_ODR) &= ~(1 << pin);
}
static inline void GPIOx_PP_TOGGLE(uint16_t gpiox, uint8_t pin) {
  GPIOx_REG(gpiox, GPIO_ODR) ^= (1 << pin);
}

static inline uint8_t GPIOx_IN_STATE(uint16_t gpiox, uint8_t pin) {
  return GPIOx_REG(gpiox, GPIO_IDR) & (1 << pin);
}

#endif
