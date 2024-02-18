TARGET = vrts

DEBUG = 1

OPT = -Og

BUILD = Build

C_SOURCES = \
Inc/ST/system_stm32g0xx.c Inc/ST/syscalls.c Inc/ST/sysmem.c \
Inc/startup.c Inc/int.c \
Lib/sys/vrts.c \
Lib/per/adc.c Lib/per/crc.c Lib/per/flash.c Lib/per/gpio.c Lib/per/i2c-master.c Lib/per/i2c.c \
Lib/per/pwm.c Lib/per/rng.c Lib/per/spi-master.c Lib/per/spi.c Lib/per/tim.c Lib/per/uart.c \
Lib/mid/buff.c Lib/mid/eeprom.c Lib/mid/file.c Lib/mid/mem.c Lib/mid/queue.c \
Lib/mid/exbit.c Lib/mid/exmath.c Lib/mid/exstring.c \
Lib/dev/dbg.c Lib/dev/stream.c Lib/dev/bash.c \
Src/main.c

ASM_SOURCES = \
Lib/sys/vrts-pendsv.s

PREFIX = arm-none-eabi-

ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
CPU = -mcpu=cortex-m0plus

# MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)
MCU = $(CPU) -mthumb

AS_DEFS = 

C_DEFS = \
-DSTM32G081xx

AS_INCLUDES = 

C_INCLUDES = \
-Iinc/CMSIS -Iinc/ST -Iinc \
-ILib/dev -ILib/mid -ILib/per -ILib/sys \
-IPlc/lib \
-ISrc

ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

LDSCRIPT = Src/flash.ld

LIBS = -lc -lm -lnosys
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD)/$(TARGET).map,--cref -Wl,--gc-sections

all: $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET).hex $(BUILD)/$(TARGET).bin

OBJECTS = $(addprefix $(BUILD)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
OBJECTS += $(addprefix $(BUILD)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD)/%.o: %.c Makefile | $(BUILD) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD)/%.o: %.s Makefile | $(BUILD)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD)/%.hex: $(BUILD)/%.elf | $(BUILD)
	$(HEX) $< $@
	
$(BUILD)/%.bin: $(BUILD)/%.elf | $(BUILD)
	$(BIN) $< $@
	
$(BUILD):
	mkdir $@

clean:
	-rm -fR $(BUILD)

run: all
	openocd -f interface/stlink.cfg -f target/stm32g0x.cfg -c "program $(BUILD)/$(TARGET).elf verify reset exit"

-include $(wildcard $(BUILD)/*.d)