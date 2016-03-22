#ifndef _TINY10SOFTI2C_SI5351_H_
#define _TINY10SOFTI2C_SI5351_H_


/* Si5351 setup macro & constants */

#define SI5351_BUS_BASE_ADDR		0xC0

#define SOURCE_PLLA 0
#define SOURCE_PLLB 1
#define MODE_INT 0
#define MODE_FLAC 1
#define CLOCK0_CONTROL(pll, intmode) CLOCK_CONTROL(16, pll, intmode)
#define CLOCK1_CONTROL(pll, intmode) CLOCK_CONTROL(17, pll, intmode)
#define CLOCK2_CONTROL(pll, intmode) CLOCK_CONTROL(18, pll, intmode)

#define CLOCK_CONTROL(addr, pll, intmode) si5351_write(addr, 0x0f | ((pll == SOURCE_PLLB) ? (1 << 5) : 0) | ((intmode == MODE_INT) ? (1 << 6): 0))

/* Power down all output drivers */
#define CLOCK_DISABLE() { \
    si5351_write(3, 0xFF); \
    si5351_write(16, 0x80); \
    si5351_write(17, 0x80); \
    si5351_write(18, 0x80); \
    si5351_write(19, 0x80); \
    si5351_write(20, 0x80); \
    si5351_write(21, 0x80); \
    si5351_write(22, 0x80); \
    si5351_write(23, 0x80); \
    si5351_write(24, 0x00); \
    si5351_write(25, 0x00); \
  }

// PLLA_SRC=XTAL, PLLB_SRC=XTAL
#define PLL_SOURCE_XTAL() {   si5351_write(15,0x00); }

// Apply PLLA and PLLB soft reset
#define PLL_SOFTRESET() {  si5351_write(177,0xAC); }

// enable CH0 - CH2
#define ENABLE_OUTPUT() { si5351_write(3, 0x00); }
#define ENABLE_OUTPUT0() { si5351_write(3, 0xfe); }
#define ENABLE_OUTPUT1() { si5351_write(3, 0xfd); }
#define ENABLE_OUTPUT2() { si5351_write(3, 0xfb); }
#define ENABLE_OUTPUT01() { si5351_write(3, 0xfc); }
#define ENABLE_OUTPUT12() { si5351_write(3, 0xf9); }
#define ENABLE_OUTPUT012() { si5351_write(3, 0xf8); }

// Set the load capacitance for the XTAL
#define CAP_6PF 1
#define CAP_8PF 2
#define CAP_10PF 3
#define SETUP_XTAL_CAP(cap) { si5351_write(183, cap << 6); }

#define SETUP_PLLA(mult, num, denom) SETUP_PAR(26, mult, num, denom, 0)
#define SETUP_PLLB(mult, num, denom) SETUP_PAR(34, mult, num, denom, 0)

#define SETUP_DIV0(div, num, denom, od) SETUP_PAR(42, div, num, denom, od)
#define SETUP_DIV1(div, num, denom, od) SETUP_PAR(50, div, num, denom, od)
#define SETUP_DIV2(div, num, denom, od) SETUP_PAR(58, div, num, denom, od)


#define PAR_P1(mult, num, denom) ((num == 0) ? (uint32_t)(128 * mult - 512) : (uint32_t)(128 * (uint32_t)mult + floor(128 * ((float)num/(float)denom)) - 512))
#define PAR_P2(mult, num, denom) ((num == 0) ? 0 : (uint32_t)(128 * (uint32_t)num - denom * floor(128 * ((float)num/(float)denom))))
#define PAR_P3(mult, num, denom) (denom)

#define SETUP_PAR(baddr, mult, num, denom, od) { \
    si5351_write( baddr, (PAR_P3(mult, num, denom) & 0x0000FF00) >> 8); \
    si5351_write( baddr+1, (PAR_P3(mult, num, denom) & 0x000000FF)); \
    si5351_write( baddr+2, ((PAR_P1(mult, num, denom) & 0x00030000) >> 16) | ((od & 0x7) << 4)); \
    si5351_write( baddr+3, (PAR_P1(mult, num, denom) & 0x0000FF00) >> 8); \
    si5351_write( baddr+4, (PAR_P1(mult, num, denom) & 0x000000FF)); \
    si5351_write( baddr+5, ((PAR_P3(mult, num, denom) & 0x000F0000) >> 12) | ((PAR_P2(mult, num, denom) & 0x000F0000) >> 16) ); \
    si5351_write( baddr+6, (PAR_P2(mult, num, denom) & 0x0000FF00) >> 8); \
    si5351_write( baddr+7, (PAR_P2(mult, num, denom) & 0x000000FF)); \
  }

#define OD_PARAM(div) ((div == 1) ? 0 : ((div == 2) ? 1 : ((div == 4) ? 2 : ((div == 8) ? 3 : ((div == 16) ? 4 : ((div == 32) ? 5 : ((div == 64) ? 6 : ((div == 128) ? 7 : 0 ))))))))

void si5351_init() {
  I2C_init();

  _delay_ms(50);
}

void si5351_write(uint8_t addr, uint8_t data) {
  I2C_start();
  I2C_write(SI5351_BUS_BASE_ADDR);
  I2C_write(addr);
  I2C_write(data);
  I2C_stop();

  _delay_us(50);
}


#endif /* _TINY10SOFTI2C_SI5351_H_ */

