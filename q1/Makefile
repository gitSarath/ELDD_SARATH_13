obj-m:=q1.o
KERN_DIR= /lib/modules/$(shell uname -r)/build
PWD=$(shell uname -r pwd)
all:
	 make -C $(KDIR)  M=$(shell pwd) modules
	
clean:
	make -C $(KDIR)  M=$(shell pwd) clean
~
~
