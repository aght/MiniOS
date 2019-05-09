SHELL = cmd

MAKEDIR = if not exist "$(1)" mkdir "$(1)"
RM = rmdir /S /Q "$(1)"
FIND = dir /a-D /S /B "$(1)"

.PHONY: all
all: kernel app run

.PHONY: kernel
kernel:
	+@$(call MAKEDIR,$(KERNEL_OUTDIR))
	+@$(CC) $(KERNEL_CFLAGS) $(KERNEL_CSOURCE) $(KERNEL_ASMSOURCE) -o $(KERNEL_OUTDIR)\$(KERNEL_PROJECT).elf -T $(KERNEL_LINKERFILE) $(KERNEL_LDFLAGS)

.PHONY: app
app:
	+@$(call MAKEDIR,$(APP_OUTDIR))
	+@$(CC) $(APP_CFLAGS) $(APP_CSOURCE) $(APP_ASMSOURCE) -o $(APP_OUTDIR)/$(APP_PROJECT).elf -T $(APP_LINKERFILE) $(APP_LDFLAGS)
	+@$(OBJCPY) $(APP_OUTDIR)\$(APP_PROJECT).elf -O binary $(APP_OUTDIR)\$(APP_PROJECT).bin

.PHONY: clean
clean:
	+@$(call RM,$(KERNEL_OUTDIR))
	+@$(call RM,$(APP_OUTDIR))

.PHONY: run
run:
	+@$(QEMU) $(QEMUFLAGS)

CC = arm-none-eabi-gcc.exe
OBJCPY = arm-none-eabi-objcopy.exe

APP_PROJECT = MiniApp
KERNEL_PROJECT = MiniOS

APP_CSOURCE = $(shell $(call FIND,app\src\*.c))
APP_ASMSOURCE = $(shell $(call FIND,app\src\*.S))
KERNEL_CSOURCE = $(shell $(call FIND,kernel\src\*.c))
KERNEL_ASMSOURCE = $(shell $(call FIND,kernel\src\*.S))

APP_LINKERFILE = app\linker\linker.ld
KERNEL_LINKERFILE = kernel\linker\linker.ld

APP_OUTDIR = app\build
KERNEL_OUTDIR = kernel\build

KERNEL_CFLAGS += -w 
KERNEL_CFLAGS += -O0 
KERNEL_CFLAGS += -mfpu=neon-vfpv4 
KERNEL_CFLAGS += -march=armv7-a 
KERNEL_CFLAGS += -mcpu=cortex-a7 
KERNEL_CFLAGS += -fpic 
KERNEL_CFLAGS += -ffreestanding 
KERNEL_CFLAGS += --specs=nosys.specs 
KERNEL_CFLAGS += -std=gnu99

APP_CFLAGS += -w
APP_CFLAGS += -O0
APP_CFLAGS += -mcpu=cortex-a7
APP_CFLAGS += -fpic
APP_CFLAGS += -fpie
APP_CFLAGS += -ffreestanding
APP_CFLAGS += --specs=nosys.specs
APP_CFLAGS += -std=gnu99

KERNEL_LDFLAGS += -lc
KERNEL_LDFLAGS += -lm
KERNEL_LDFLAGS += -lg
KERNEL_LDFLAGS += -lgcc

APP_LDFLAGS += -s
APP_LDFLAGS += -lc
APP_LDFLAGS += -lm
APP_LDFLAGS += -lg
APP_LDFLAGS += -lgcc

QEMU = qemu-system-arm.exe

SDCARD = sdcard\sdcard.img

QEMUFLAGS += -m 256
QEMUFLAGS += -M raspi2
QEMUFLAGS += -serial stdio
QEMUFLAGS += -kernel $(KERNEL_OUTDIR)\$(KERNEL_PROJECT).elf
QEMUFLAGS += -drive file=$(SDCARD),if=sd,format=raw

