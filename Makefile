GCCPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -g -fno-stack-protector -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386
CC = gcc
CCASM = as
NAME = kern
objects = 	obj/loader.o \
			obj/kernel/kernel.o \
			obj/gdt.o \
			obj/port.o \
			obj/interrupts.o \
			obj/interruptstubs.o \
			obj/drivers/keyboard.o\
			obj/drivers/mouse.o

GCCINC = -I inc/ -I inc/drivers -I inc/kernel


run: $(NAME).iso 
	(killall qemu-system-x86_64 && sleep 1) || true
	qemu-system-x86_64 -boot d -cdrom $(NAME).iso -m 512

$(NAME).iso: $(NAME).bin
	mkdir -p iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' >iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "My OS" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/kern.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso
	
$(NAME).bin: src/linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

obj: 
	mkdir obj
	mkdir obj/kernel
	mkdir obj/drivers

obj/%.o: src/%.cpp obj
	$(CC) $(GCCPARAMS) $(GCCINC) -c -o $@ $<

obj/%.o: src/%.s obj
	$(CCASM) $(ASPARAMS) -o $@ $<


install: mykernel.bin
	sudo cp $< /boot/mykernel.bin



# Programable interrupt controller 
#  PIC 

.PHONY: clean fclean re

clean : 
	rm -rf obj

fclean : clean
	rm -rf $(NAME).bin
	rm -rf $(NAME).iso

re: fclean run
