/*    tiny10softI2C.h    Software I2C Library for ATTiny10    http://s2jp.com/        Copyright (c) 2012 Futaba. All Rights Reserved.*/#ifndef _S2_TINY10_SOFTI2C_H_#define _S2_TINY10_SOFTI2C_H_#include <avr/io.h>#include <util/delay.h>#define I2C_SDA_PIN            PB0#define I2C_SCL_PIN            PB1#define I2C_SDA_PIN_BV        _BV(I2C_SDA_PIN)#define I2C_SCL_PIN_BV        _BV(I2C_SCL_PIN)#define _I2C_SDA_HIGH        (PORTB |= I2C_SDA_PIN_BV)#define _I2C_SDA_LOW        (PORTB &= ~I2C_SDA_PIN_BV)#define _I2C_SCL_HIGH        (PORTB |= I2C_SCL_PIN_BV)#define _I2C_SCL_LOW        (PORTB &= ~I2C_SCL_PIN_BV)void I2C_clkWait(){    #if F_CPU == 1000000  __asm__ __volatile__("nop");    #elif F_CPU == 1200000  __asm__ __volatile__("nop");    #elif F_CPU == 2000000  __asm__ __volatile__("nop");  __asm__ __volatile__("nop");  __asm__ __volatile__("nop");  __asm__ __volatile__("nop");  __asm__ __volatile__("nop");    #elif F_CPU == 2400000  __asm__ __volatile__("nop");  __asm__ __volatile__("nop");  __asm__ __volatile__("nop");  __asm__ __volatile__("nop");    #elif F_CPU == 4000000  _delay_us(2);    #elif F_CPU == 4800000  _delay_us(2);    #else  _delay_us(4);    #endif}void I2C_clkWait2(){    #if F_CPU == 1000000    #elif F_CPU == 1200000    #elif F_CPU == 2000000    #elif F_CPU == 2400000    #elif F_CPU == 4000000  _delay_us(2);    #elif F_CPU == 4800000  _delay_us(2);    #else  _delay_us(3);    #endif}inline static void I2C_init(){  DDRB |= I2C_SDA_PIN_BV;  DDRB |= I2C_SCL_PIN_BV;  _I2C_SCL_HIGH;  _I2C_SDA_HIGH;}void I2C_start(){  _I2C_SDA_HIGH;  I2C_clkWait();  _I2C_SCL_HIGH;      I2C_clkWait();  _I2C_SDA_LOW;  I2C_clkWait();  _I2C_SCL_LOW;}void I2C_stop(){  _I2C_SDA_LOW;  I2C_clkWait();  _I2C_SCL_HIGH;  I2C_clkWait();  _I2C_SDA_HIGH;  I2C_clkWait();}uint8_t I2C_write(uint8_t data){  uint8_t temp;  for(temp=0x80; temp; temp>>=1)    {      if(data & temp)        _I2C_SDA_HIGH;      else                _I2C_SDA_LOW;      I2C_clkWait2();      _I2C_SCL_HIGH;      I2C_clkWait();      _I2C_SCL_LOW;    }      DDRB &= ~I2C_SDA_PIN_BV;      I2C_clkWait();  temp=PINB;  _I2C_SCL_HIGH;  I2C_clkWait();      _I2C_SCL_LOW;      DDRB |= I2C_SDA_PIN_BV;      if(temp & I2C_SDA_PIN_BV)    {      return 1;    }  return 0;}uint8_t I2C_read(uint8_t ACK){  uint8_t data=0;  uint8_t temp;      DDRB &= ~I2C_SDA_PIN_BV;      for(temp=0x80; temp; temp>>=1)    {      I2C_clkWait();      if(PINB & I2C_SDA_PIN_BV)        data |= temp;      _I2C_SCL_HIGH;              I2C_clkWait();      _I2C_SCL_LOW;    }      DDRB |= I2C_SDA_PIN_BV;      if(ACK)    _I2C_SDA_LOW;  else    _I2C_SDA_HIGH;      I2C_clkWait();  _I2C_SCL_HIGH;  I2C_clkWait();  _I2C_SCL_LOW;  return data;}#endif