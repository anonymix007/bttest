#!/bin/sh
qemu-system-x86_64 \
	-m 2G \
	-smp 2 \
	-kernel linux-6.2.y/arch/x86/boot/bzImage \
	-append "console=ttyS0 root=/dev/sda earlyprintk=serial net.ifnames=0" \
	-drive file=debian-img/bullseye_src.img,format=raw \
	-net user,host=10.0.2.10,hostfwd=tcp:127.0.0.1:10021-:22 \
	-net nic,model=e1000 \
	-device qemu-xhci,id=xhci -device usb-host,bus=xhci.0,vendorid=0x0bda,productid=0x8771 \
	-enable-kvm \
	-nographic \
	-pidfile vm_src.pid \
	2>&1 | tee vm_src.log
