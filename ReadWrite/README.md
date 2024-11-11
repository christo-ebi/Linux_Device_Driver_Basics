sudo insmod devNum.ko
sudo mknod /dev/myDevice c 90 0

gcc demoApp.c -o demoApp
dmesg | tail -n 10

sudo rmmod devNum
