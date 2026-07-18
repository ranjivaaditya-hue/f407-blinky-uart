TARGET  = blinky
BUILD   = build

CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

CPU     = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16

INC = -Iinc

CFLAGS  = $(CPU) $(INC) -Wall -Wextra -O0 -g3 -ffunction-sections -fdata-sections
LDFLAGS = $(CPU) -Tlinker/STM32F407VG_FLASH.ld -Wl,--gc-sections -nostdlib

OBJ = $(BUILD)/main.o $(BUILD)/startup_stm32f407xx.o
vpath %.c src
vpath %.s startup

all: $(BUILD)/$(TARGET).elf $(BUILD)/$(TARGET).bin

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: %.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.s | $(BUILD)
	$(CC) $(CPU) -c $< -o $@

$(BUILD)/$(TARGET).elf: $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@
	$(SIZE) $@

$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

flash: $(BUILD)/$(TARGET).bin
	STM32_Programmer_CLI -c port=SWD -w $(BUILD)/$(TARGET).bin 0x08000000 -v -rst

clean:
	rm -rf $(BUILD)

.PHONY: all clean flash
