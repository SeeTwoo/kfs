all: run qemu

build:
	docker build -t kfs .

it: build
	docker run --rm -it -v $$(pwd):/app kfs

run: build
	docker run --rm --name kfs_container -v $$(pwd):/app kfs sh -c "./c_build.sh"

qemu:
	qemu-system-i386 -cdrom kfs.iso

clean:
	rm -f *.o

fclean: clean
	rm -rf *.bin *.iso isodir
