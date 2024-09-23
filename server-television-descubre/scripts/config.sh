#!/bin/bash
# Configuring Arduino

echo "Creating Conexion"
chmod 777 /dev/ttyACM0
stty -F /dev/ttyACM0 cs8 9600 ignbrk -brkint -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts
cat /dev/ttyACM0
