fig3 : fig3.S
	arm-linux-gnueabi-as -g -o fig3.o fig3.S
	arm-linux-gnueabi-ld -g -o fig3 fig3.o
	#arm-linux-gnueabi-gcc -static $^ -o $@
	echo "Run with qemu-arm"

clean:
	rm -f fig3
