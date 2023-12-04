
ifneq ($(KERNELRELEASE),)
include Kbuild
else
# normal makefile
KDIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD
	cp hello1.ko hello1.ko.unstripped
	$(CROSS_COMPILE)strip -g hello1.ko # strip only debugging info
clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
endif
