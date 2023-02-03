#!/bin/sh

if [ "$1" = "BR-6208ACv2" ]; then
	cd linux-2.6.36.x_4200/
	./DoLinux.sh clean
	cd -
	./COMPILING-SCRIPT.sh	BR6278AC Edimax		1.02_GPL	4200
elif [ "$1" = "EW-7478AC" ]; then
	cd linux-2.6.36.x_4200/
	./DoLinux.sh clean
	cd -
	./COMPILING-SCRIPT.sh	EW7478AC Edimax		1.07_GPL	4200
elif [ "$1" = "RE11" ]; then
	cd linux-2.6.36.x_4200/
	./DoLinux.sh clean
	cd -
	./COMPILING-SCRIPT.sh	EW7478AC Edimax		1.07_GPL	4200 Romaing
else
	echo "Usage: BUILD.sh {BR-6208ACv2 | EW-7478AC | RE11}"
fi
