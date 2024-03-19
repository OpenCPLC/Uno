TARGET = uno

DEBUG = 1

OPT = -Og

BUILD = build

C_SOURCES = inc/int.c inc/startup.c inc/ST/syscalls.c inc/ST/sysmem.c inc/ST/system_stm32g0xx.c \
lib/dev/bash.c lib/dev/dbg.c lib/dev/stream.c lib/ext/buff.c lib/ext/eeprom.c lib/ext/exbit.c \
lib/ext/exmath.c lib/ext/exstring.c lib/ext/file.c lib/ifc/i2c-master.c lib/ifc/i2c.c \
lib/ifc/spi-master.c lib/ifc/spi.c lib/ifc/uart.c lib/per/adc.c lib/per/crc.c lib/per/flash.c \
lib/per/gpio.c lib/per/pwm.c lib/per/pwr.c lib/per/rng.c lib/per/rtc.c lib/per/tim.c \
lib/sys/new.c lib/sys/vrts.c plc/opencplc-uno.c plc/com/modbus-master.c plc/com/modbus-slave.c \
plc/per/ain.c plc/per/din.c plc/per/dout.c plc/per/max31865.c plc/per/pwmi.c plc/utils/cron.c \
plc/utils/timer.c src/main.c

ASM_SOURCES = lib/sys/vrts-pendsv.s 

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

AS_DEFS = -DSTM32G081xx

C_DEFS = -DSTM32G081xx

ASM_INCLUDES =

C_INCLUDES = -Iinc -Iinc/CMSIS -Iinc/ST -Ilib/dev -Ilib/ext -Ilib/ifc -Ilib/per -Ilib/sys -Iplc \
-Iplc/com -Iplc/per -Iplc/utils -Isrc 

ASFLAGS = $(MCU) $(AS_DEFS) $(ASM_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

LDSCRIPT = src/mem.ld

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

clr:
	cmd /c rmdir /s /q $(BUILD)

run: all
	openocd -f interface/stlink.cfg -f target/stm32g0x.cfg -c "program $(BUILD)/$(TARGET).elf verify reset exit"

-include $(wildcard $(BUILD)/*.d)