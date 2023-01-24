obj-m:=q2.o

KERN_DIR= /lib/modules/$(shell uname -r)/build


all:
	make -C $(KDIR)  M=$(shell pwd) modules
        
clean:
	make -C $(KDIR)  M=$(shell pwd) clean
~

