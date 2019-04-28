Clear-Host;

$exec=$args[0]
$ErrorActionPreference = "Stop"

Write-Host "Building..."

& arm-none-eabi-gcc.exe `
	-mcpu=cortex-a7 `
	-fpic `
	-ffreestanding `
  --specs=nosys.specs `
	-std=gnu99 `
	-T linker/linker.ld  `
	src/hal/hal.S `
	src/boot.s  `
  src/drivers/smartstart/rpi-SmartStart.s `
	src/drivers/stdio/emb-stdio.c `
  src/drivers/sdcard/SDCard.c `
  src/drivers/smartstart/rpi-SmartStart.c `
	src/hal/hal.c `
	src/kernel.c `
	src/uart/uart.c `
	src/console/console.c `
	src/console/command.c `
	src/coreutils/realpath.c `
	src/vector/vector.c `
  -o build/kernel.elf `
	-O0	`
	-lc -lm -lg -lgcc

	# arm-none-eabi-objdump -D .\output\kernel.elf | Out-File -filepath output/kernel.lss -Encoding ASCII
	# arm-none-eabi-objdump -s .\output\kernel.elf | Out-File -filepath output/kernel.dump -Encoding ASCII

if ( $exec -eq "exec=true" )
{
  & qemu-system-arm.exe -m 256 -M raspi2 -serial stdio -kernel build/kernel.elf -drive file=sdcard/sdcard.img,if=sd,format=raw
}
