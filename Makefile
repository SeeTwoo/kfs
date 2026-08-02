build:
	docker build -t kfs .

it: build
	docker run --rm -it -v $$(pwd):/app kfs

run: build
	docker run --rm -v $$(pwd):/app kfs

qemu:
	qemu-system-i386 -cdrom kfs.iso

clean:
	rm -f *.o
