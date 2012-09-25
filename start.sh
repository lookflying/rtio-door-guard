#!/bin/bash
mkdir /dev/shm/www
./doorGuard
while [ $? != 0 ]
do
	./PureVisualServer
done
