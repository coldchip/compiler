#!/bin/sh
rep='.o'

for d in ./*.c; do
	echo  gcc -c -Ofast -s -o bin/$(echo $(basename "$d") | sed "s/\.c/$rep/") $d
    gcc -c -Ofast -s -o bin/$(echo $(basename "$d") | sed "s/\.c/$rep/") $d
done

echo gcc -o bin/chipcode bin/*.o

gcc -o bin/chipcode bin/*.o