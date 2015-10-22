//
// Rotary Encoder handler
// Copyright (C) 2015 by Yoshitaka Kuwata, morecat_lab
//
// Tiny 10 with avr-gcc
// Rotary Encoder(A,B) -> DSO138 ('+' and '-' button)
//

#include <avr/io.h>
#include <util/delay.h>

#define PULSE_WIDTH_US 120
#define SAMPLING_INTERVAL_US 6
#define ENC_A PB0
#define ENC_B PB1
#define OUT1 PB2
#define OUT2 PB3

int main()
{
  uint8_t dataRead = 0;

  // initialize hardware
  CCP = 0xD8;		// write signature for change enable of protected I/O register
  CLKMSR = 0x00;  	// -> internal 8MHz
  CCP = 0xD8;		// write signature for change enable of protected I/O register
  CLKPSR = 0x08; 	// -> prescale = 256

  DDRB  = 0; // all input
  PORTB = (1 << OUT1) | (1 << OUT2);	// OUT1, OUT2 = HIGH
  PUEB  = (1 << ENC_A) | ( 1 << ENC_B);	// ENC_A, ENC_B = PULL-UP

  while(1) {
    dataRead = ((dataRead << 2) + (PINB & 0x03)) & 0x0f;  // calculate index

    if (dataRead == 0x7) {	// 1 -> 3
      DDRB = (1 << OUT1);	// OUT1 = OUTPUT
      PORTB = ~(1 << OUT1);	// OUT1 = LOW
      _delay_us(PULSE_WIDTH_US);
      DDRB = 0;			// OUT1, OUT2 = input
      PORTB = (1 << OUT1) | (1 << OUT2);	// OUT1, OUT2 = HIGH
    } else if (dataRead == 0xd) { // 3 -> 1
      DDRB = (1 << OUT2);	// OUT2 = output
      PORTB = ~(1 << OUT2);	// OUT2 = LOW
      _delay_us(PULSE_WIDTH_US);
      DDRB = 0;			// OUT1, OUT2 = input
      PORTB = (1 << OUT1) | (1 << OUT2);	// OUT1, OUT2 = HIGH
    } else {
      _delay_us(SAMPLING_INTERVAL_US);
    }
  }
}
// EOF
