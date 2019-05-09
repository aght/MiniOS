ifeq ($(shell echo $$OS),$$OS)
	MAKEDIR = if not exist "$(1)" mkdir "$(1)"
	RM = rmdir /S /Q "$(1)"

	# Will not work with '/' beacause Windows
	FIND = dir /a-D /S /B "$(1)"
else
	MAKEDIR = '$(SHELL)' -c "mkdir -p \"$(1)\""
	RM = '$(SHELL)' -c "rm -rf \"$(1)\""
	FIND = '$(SHELL)' -c "find -iname \"$(1)\""
endif

.PHONY: all
all: build run

.PHONY: build
build:
	+@$(call MAKEDIR,$(OUTDIR))
	+@$(CC) $(CFLAGS) $(CSOURCE) $(ASMSOURCE) -o $(OUTDIR)/$(PROJECT).elf -T $(LINKERFILE) $(LDFLAGS)

.PHONY: clean
clean:
	+@$(call RM,$(OUTDIR))

.PHONY: run
run:
	+@$(QEMU) $(QEMUFLAGS)

CC = arm-none-eabi-gcc.exe

PROJECT = MiniOS

CSOURCE = $(shell $(call FIND,*.c))
ASMSOURCE = $(shell $(call FIND,*.S))

LINKERFILE = linker/linker.ld

OUTDIR = build

CFLAGS += -w 
CFLAGS += -O0 
CFLAGS += -mfpu=neon-vfpv4 
CFLAGS += -march=armv7-a 
CFLAGS += -mcpu=cortex-a7 
CFLAGS += -fpic 
CFLAGS += -ffreestanding 
CFLAGS += --specs=nosys.specs 
CFLAGS += -std=gnu99

LDFLAGS += -lc
LDFLAGS += -lm
LDFLAGS += -lg
LDFLAGS += -lgcc

QEMU = qemu-system-arm.exe

SDCARD = sdcard/sdcard.img

QEMUFLAGS += -m 256
QEMUFLAGS += -M raspi2
QEMUFLAGS += -serial stdio
QEMUFLAGS += -kernel $(OUTDIR)/$(PROJECT).elf
QEMUFLAGS += -drive file=$(SDCARD),if=sd,format=raw

