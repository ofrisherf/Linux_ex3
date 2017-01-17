obj-m += hello-1.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test: clean all
	sudo insmod hello-1.ko
	sudo mknod /dev/cardsdev c 246 0
	dd if=/dev/cardsdev bs=2 count=1 2>/dev/null ; echo
	sudo rm /dev/cardsdev
	sudo rmmod hello_1
