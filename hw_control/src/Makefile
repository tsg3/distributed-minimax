## 'obj-m': Kernel module object
obj-m += push_button_mod.o

all:	## 'all': compiles the kernel module files
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:	## 'clean': deletes the compiled module files
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
