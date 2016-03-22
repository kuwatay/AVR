###############################################################################
# Makefile for the project SI5351 synthe
###############################################################################

## General Flags
PROJECT = clk
MCU = attiny10
TARGET = $(PROJECT).elf
DEBUG =
CC = avr-gcc
AVRDUDE = avrdude -c usbasp -p$(MCU)

FUSE = 0xfe
# Configuration Section
# 0xfe = . . . .   . 1 1 0 <-- RSTDISBL (External Reset disable)
#                    ^ ^ 
#                    | + ------ WDTON (Watchdog Timer always on)
#                    +-------- CKOUT (System Clock Output (Unprogrammed)

## Options common to compile, link and assembly rules
COMMON = -g -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -Os $(DEBUG)

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += -x assembler-with-cpp -Wa,

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=$(PROJECT).map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom


## Include Directories
INCLUDES =

## Objects that must be built in order to link
OBJECTS = clk.o

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) $(PROJECT).hex $(PROJECT).lss size

$(OBJECTS): Makefile

clk.o: clk.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $*.hex
#	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $*_raw.hex
#	fix_lds -v < $*_raw.hex > $@

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) $(PROJECT).o $(PROJECT).map $(PROJECT).hex $(PROJECT).elf $(PROJECT).lss *~


.PHONY: flash
flash:	all
	$(AVRDUDE) -U flash:w:$(PROJECT).hex


.PHONY: fuse
fuse:
	$(AVRDUDE) -U fuse:w:$(FUSE):m

