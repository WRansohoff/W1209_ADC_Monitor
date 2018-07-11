#include "main.h"

/*
 * Convert a number between 0-9 to a corresponding char.
 * Return 'C' if the number is >9. Maybe 'E' would be better...
 */
char digit_to_char(uint8_t ic) {
  if (ic == 0) { return '0'; }
  else if (ic == 1) { return '1'; }
  else if (ic == 2) { return '2'; }
  else if (ic == 3) { return '3'; }
  else if (ic == 4) { return '4'; }
  else if (ic == 5) { return '5'; }
  else if (ic == 6) { return '6'; }
  else if (ic == 7) { return '7'; }
  else if (ic == 8) { return '8'; }
  else if (ic == 9) { return '9'; }
  else { return 'C'; }
}

/*
 * Set a GPIO pin to push-pull output mode.
 */
static inline void set_gpio_pp(uint16_t gpio_bank, uint8_t pin) {
  // Set pin to output mode.
  GPIOx_REG(gpio_bank, GPIO_DDR) |= (1 << pin);
  // Set pin to push-pull mode.
  GPIOx_REG(gpio_bank, GPIO_CR1) |= (1 << pin);
}

/*
 * Set a GPIO pin to digital input mode, optionally
 * with the internal pull-up resistor enabled.
 */
static inline void set_gpio_in(uint16_t gpio_bank,
                               uint8_t pin,
                               volatile uint8_t pullup_en) {
  // Set pin to input mode.
  GPIOx_REG(gpio_bank, GPIO_DDR) &= ~(1 << pin);
  // Enable internal pull-up resistor if necessary.
  if (pullup_en) {
    GPIOx_REG(gpio_bank, GPIO_CR1) |=  (1 << pin);
  }
  else {
    GPIOx_REG(gpio_bank, GPIO_CR1) &= ~(1 << pin);
  }
}

/*
 * Draw a single 7-segment LED display digit.
 * The WS1209 board has 3 digits, and any combination of them
 * can be addressed. But each selected digit will show
 * the same 7 segments, so it is generally best to cycle
 * through them quickly enough that the eye doesn't notice.
 * For that reason, this method will only draw to one digit
 * at a time. It can also light up a decimal point on each digit.
 * 'c':       The character to display. Mostly just 0-9.
 * 'dig':     The digit to draw to. Must be 0, 1, or 2.
 * 'with_dp': Light up the decimal point? 0 = no.
 */
void draw_7s_digit(volatile char c,
                   volatile int dig,
                   volatile uint8_t with_dp) {
  uint32_t delay_i;
  // Set which digit to draw to.
  // For now, only one at a time. Default to 'all off.'
  if (dig == 0) {
      GPIOx_PP_OFF(SEG_D1_PIN);
      GPIOx_PP_ON(SEG_D2_PIN);
      GPIOx_PP_ON(SEG_D3_PIN);
  }
  else if (dig == 1) {
      GPIOx_PP_ON(SEG_D1_PIN);
      GPIOx_PP_OFF(SEG_D2_PIN);
      GPIOx_PP_ON(SEG_D3_PIN);
  }
  else if (dig == 2) {
      GPIOx_PP_ON(SEG_D1_PIN);
      GPIOx_PP_ON(SEG_D2_PIN);
      GPIOx_PP_OFF(SEG_D3_PIN);
  }
  else {
      GPIOx_PP_ON(SEG_D1_PIN);
      GPIOx_PP_ON(SEG_D2_PIN);
      GPIOx_PP_ON(SEG_D3_PIN);
  }

  // Draw the actual 7-segment digit.
  // LA: Top
  // LB: Top-Left
  // LC: Bottom-Right
  // LD: Bottom
  // LE: Bottom-Left
  // LF: Top-Right
  // LG: Middle
  if (c == '0') {
      GPIOx_PP_ON(SEG_LA_PIN);
      GPIOx_PP_ON(SEG_LB_PIN);
      GPIOx_PP_ON(SEG_LC_PIN);
      GPIOx_PP_ON(SEG_LD_PIN);
      GPIOx_PP_ON(SEG_LE_PIN);
      GPIOx_PP_ON(SEG_LF_PIN);
      GPIOx_PP_OFF(SEG_LG_PIN);
  }
  else if (c == '1') {
      GPIOx_PP_OFF(SEG_LA_PIN);
      GPIOx_PP_OFF(SEG_LB_PIN);
      GPIOx_PP_ON(SEG_LC_PIN);
      GPIOx_PP_OFF(SEG_LD_PIN);
      GPIOx_PP_OFF(SEG_LE_PIN);
      GPIOx_PP_ON(SEG_LF_PIN);
      GPIOx_PP_OFF(SEG_LG_PIN);
  }
  else if (c == '2' || c == 'Z') {
      GPIOx_PP_ON(SEG_LA_PIN);
      GPIOx_PP_OFF(SEG_LB_PIN);
      GPIOx_PP_OFF(SEG_LC_PIN);
      GPIOx_PP_ON(SEG_LD_PIN);
      GPIOx_PP_ON(SEG_LE_PIN);
      GPIOx_PP_ON(SEG_LF_PIN);
      GPIOx_PP_ON(SEG_LG_PIN);
  }
  else if (c == '3') {
      GPIOx_PP_ON(SEG_LA_PIN);
      GPIOx_PP_OFF(SEG_LB_PIN);
      GPIOx_PP_ON(SEG_LC_PIN);
      GPIOx_PP_ON(SEG_LD_PIN);
      GPIOx_PP_OFF(SEG_LE_PIN);
      GPIOx_PP_ON(SEG_LF_PIN);
      GPIOx_PP_ON(SEG_LG_PIN);
  }
  else if (c == '4') {
      GPIOx_PP_OFF(SEG_LA_PIN);
      GPIOx_PP_ON(SEG_LB_PIN);
      GPIOx_PP_ON(SEG_LC_PIN);
      GPIOx_PP_OFF(SEG_LD_PIN);
      GPIOx_PP_OFF(SEG_LE_PIN);
      GPIOx_PP_ON(SEG_LF_PIN);
      GPIOx_PP_ON(SEG_LG_PIN);
  }
  else if (c == '5' || c == 'S') {
      GPIOx_PP_ON(SEG_LA_PIN);
      GPIOx_PP_ON(SEG_LB_PIN);
      GPIOx_PP_ON(SEG_LC_PIN);
      GPIOx_PP_ON(SEG_LD_PIN);
      GPIOx_PP_OFF(SEG_LE_PIN);
      GPIOx_PP_OFF(SEG_LF_PIN);
      GPIOx_PP_ON(SEG_LG_PIN);
  }
  else if (c == '6') {
      GPIOx_PP_ON(SEG_LA_PIN);
      GPIOx_PP_ON(SEG_LB_PIN);
      GPIOx_PP_ON(SEG_LC_PIN);
      GPIOx_PP_ON(SEG_LD_PIN);
      GPIOx_PP_ON(SEG_LE_PIN);
      GPIOx_PP_OFF(SEG_LF_PIN);
      GPIOx_PP_ON(SEG_LG_PIN);
  }
  else if (c == '7') {
      GPIOx_PP_ON(SEG_LA_PIN);
      GPIOx_PP_ON(SEG_LB_PIN);
      GPIOx_PP_ON(SEG_LC_PIN);
      GPIOx_PP_OFF(SEG_LD_PIN);
      GPIOx_PP_OFF(SEG_LE_PIN);
      GPIOx_PP_ON(SEG_LF_PIN);
      GPIOx_PP_OFF(SEG_LG_PIN);
  }
  else if (c == '8') {
      GPIOx_PP_ON(SEG_LA_PIN);
      GPIOx_PP_ON(SEG_LB_PIN);
      GPIOx_PP_ON(SEG_LC_PIN);
      GPIOx_PP_ON(SEG_LD_PIN);
      GPIOx_PP_ON(SEG_LE_PIN);
      GPIOx_PP_ON(SEG_LF_PIN);
      GPIOx_PP_ON(SEG_LG_PIN);
  }
  else if (c == '9') {
      GPIOx_PP_ON(SEG_LA_PIN);
      GPIOx_PP_ON(SEG_LB_PIN);
      GPIOx_PP_ON(SEG_LC_PIN);
      GPIOx_PP_ON(SEG_LD_PIN);
      GPIOx_PP_OFF(SEG_LE_PIN);
      GPIOx_PP_ON(SEG_LF_PIN);
      GPIOx_PP_ON(SEG_LG_PIN);
  }
  else if (c == 'V' || c == 'U') {
      GPIOx_PP_OFF(SEG_LA_PIN);
      GPIOx_PP_ON(SEG_LB_PIN);
      GPIOx_PP_ON(SEG_LC_PIN);
      GPIOx_PP_ON(SEG_LD_PIN);
      GPIOx_PP_ON(SEG_LE_PIN);
      GPIOx_PP_ON(SEG_LF_PIN);
      GPIOx_PP_OFF(SEG_LG_PIN);
  }
  else if (c == 'C') {
      GPIOx_PP_ON(SEG_LA_PIN);
      GPIOx_PP_ON(SEG_LB_PIN);
      GPIOx_PP_OFF(SEG_LC_PIN);
      GPIOx_PP_ON(SEG_LD_PIN);
      GPIOx_PP_ON(SEG_LE_PIN);
      GPIOx_PP_OFF(SEG_LF_PIN);
      GPIOx_PP_OFF(SEG_LG_PIN);
  }
  // (TODO)
  else if (c == 'c') {
  }
  else {
  }

  // Draw the decimal point if appropriate.
  if (with_dp != 0) {
    GPIOx_PP_ON(SEG_DP_PIN);
  }
  else {
    GPIOx_PP_OFF(SEG_DP_PIN);
  }

  // Very brief delay to allow digit to 'set'.
  for (delay_i = 0; delay_i < (F_CPU / 18000) * 2; ++delay_i) {
    __asm__("NOP");
  }
}

/*
 * Read a value from the ADC, which must have been
 * previously initialized.
 * (Thanks for the ADC and STM8 info, lujji!)
 */
uint16_t ADC_read() {
  uint8_t adcH, adcL;
  ADC1_CR1 |=  (0x01);
  while (!(ADC1_CSR & (0x80)));
  adcL = ADC1_DRL;
  adcH = ADC1_DRH;
  // (Clear the EOC flag manually)
  ADC1_CSR &= ~(0x80);
  return (adcL | (adcH << 8));
}

/*
 * Main method.
 */
void main() {
  // Setup GPIO pins.
  // Relay control pin.
  set_gpio_pp(RELAY_PIN);
  // 7-Segment LED display pins.
  set_gpio_pp(SEG_D1_PIN);
  set_gpio_pp(SEG_D2_PIN);
  set_gpio_pp(SEG_D3_PIN);
  set_gpio_pp(SEG_LA_PIN);
  set_gpio_pp(SEG_LB_PIN);
  set_gpio_pp(SEG_LC_PIN);
  set_gpio_pp(SEG_LD_PIN);
  set_gpio_pp(SEG_LE_PIN);
  set_gpio_pp(SEG_LF_PIN);
  set_gpio_pp(SEG_LG_PIN);
  set_gpio_pp(SEG_DP_PIN);
  // Button input pins.
  set_gpio_in(GPIOC_BASE, KEY_SET_PC, 1);
  set_gpio_in(GPIOC_BASE, KEY_PLS_PC, 1);
  set_gpio_in(GPIOC_BASE, KEY_MIN_PC, 1);
  // The resistive input pin uses the ADC;
  // leave that pin in its reset state.

  // Start up the ADC. (Pin D6 is channel 6)
  ADC1_CSR  &=  (0xF0);
  ADC1_CSR  |=  (0x06);
  // Configure the ADC to store data in LSB format.
  ADC1_CR2  |=  (0x08);
  // Turn the ADC on.
  ADC1_CR1  |=  (0x01);

  // Main loop.
  while (1) {
    // Toggle the relay if the 'Set' button is newly pressed.
    // TODO: Use an interrupt.
    if ((!set_button_down) && (!(GPIOx_IN_STATE(GPIOC_BASE, KEY_SET_PC)))) {
      set_button_down = 1;
      // Toggle the LED/Relay pin's ODR register bit.
      GPIOx_PP_TOGGLE(RELAY_PIN);
      // Delay a bit.
      for (delay_i = 0; delay_i < (F_CPU / 18000) * DB_MS; ++delay_i) {
        __asm__("NOP");
      }
    }
    else if ((GPIOx_IN_STATE(GPIOC_BASE, KEY_SET_PC))) {
      // Register when the button is not pressed any more.
      set_button_down = 0;
    }

    // Read the raw ADC value and print it to the 3-digit display.
    adc_r = ADC_read();
    if (adc_r >= 999) {
      draw_7s_digit('9', 0, 0);
      draw_7s_digit('9', 1, 0);
      draw_7s_digit('9', 2, 0);
    }
    else {
      draw_7s_digit(digit_to_char((adc_r / 100) % 10), 0, 0);
      draw_7s_digit(digit_to_char((adc_r / 10) % 10), 1, 0);
      draw_7s_digit(digit_to_char((adc_r) % 10), 2, 0);
    }
  }
}
