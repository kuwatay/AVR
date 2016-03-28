/*
  Si5351 clock control by tiny10

  2016/3/21
 */

#define F_CPU 1000000

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>

#include "tiny10softI2C.h"
#include "tiny10softI2C_si5351.h"

int main(void) {
  // setup t10 internal clock speed
  CCP = 0xD8;
  CLKMSR = 0x00;  // internal 8MHz
  CCP = 0xD8;
  CLKPSR = 0x03;  // 1/8 prescaler -> 1MHz

  si5351_init();

  CLOCK_DISABLE();  // Power down all output drivers

  SETUP_XTAL_CAP(CAP_10PF);  // Set the load capacitance for the XTAL

  PLL_SOURCE_XTAL();  // input source of PLAA PLLB to xtal

  // PLLA
  // VCO Frequency (MHz) =  696.204000000
  SETUP_PLLA(26UL, 5051UL, 6500UL);  // Input Frequency (MHz) = 26.000000000, Feedback Divider = 26  5051/6500

  // PLLB
  // VCO Frequency (MHz) =  832.000000000
  SETUP_PLLB(32UL, 0UL, 1UL);  // Input Frequency (MHz) = 26.000000000, Feedback Divider = 32

  // Channel 0
  // Output Frequency (MHz) = 4.194000000
  SETUP_DIV0(166UL, 0UL, 1UL, OD_PARAM(1));   // Multisynth Output Frequency (MHz) = 4.194000000,  Multisynth Divider = 166,  R Divider = 1
  CLOCK0_CONTROL(SOURCE_PLLA, MODE_INT);   // PLL source = PLLA

  // Channel 1
  // Output Frequency (MHz) = 12.80000000
  SETUP_DIV1(65UL, 0UL, 1UL, OD_PARAM(1));  // Multisynth Output Frequency (MHz) = 12.800000000, Multisynth Divider = 65,  R Divider = 1
  CLOCK1_CONTROL(SOURCE_PLLB, MODE_INT);   // PLL source = PLLB

  // Channel 2
  // Output Frequency (MHz) = 0.010000000
  SETUP_DIV2(1300UL, 0UL, 1UL, OD_PARAM(64));   // Multisynth Output Frequency (MHz) = 0.640000000, Multisynth Divider = 1300, R Divider = 64
  CLOCK2_CONTROL(SOURCE_PLLB, MODE_INT);   // PLL source = PLLB

  PLL_SOFTRESET();  // Apply PLLA and PLLB soft reset

  ENABLE_OUTPUT();  // Enable all outputs

  while(1) {
    sleep_cpu();
  }
}
