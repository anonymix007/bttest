#!/bin/sh
qemu-system-x86_64 \
	-m 2G \
	-smp 2 \
	-kernel linux-6.2.11/arch/x86/boot/bzImage \
	-append "console=ttyS0 root=/dev/sda earlyprintk=serial net.ifnames=0" \
	-drive file=debian-img/bullseye_snk.img,format=raw \
	-net user,host=10.0.2.10,hostfwd=tcp:127.0.0.1:10031-:22 \
	-net nic,model=e1000 \
	--bt-dev=hci1 \
	-enable-kvm \
	-nographic \
	-pidfile vm_snk.pid \
	2>&1 | tee vm_snk.log
