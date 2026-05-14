obj-m += mon_clavier.o

all:
	make -C /lib/modules/`uname -r`/build M=$(PWD) modules

clean:
	make -C /lib/modules/`uname -r`/build M=$(PWD) clean

test: all
	dmesg -W > test.out & echo $$! > /tmp/dmesg_pid; \
	udevadm trigger --action=add --subsystem-match=usb --attr-match=bInterfaceClass=03; \
	sleep 1; \
	lsmod | grep mon_clavier || true; \
	udevadm trigger --action=remove --subsystem-match=usb --attr-match=bInterfaceClass=03; \
	sleep 1; \
	kill $$(cat /tmp/dmesg_pid); \
	cat test.out; \
	lsmod | grep mon_clavier || true; \
	rm test.out
