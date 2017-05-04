#!/bin/sh


MPV="mpv"
MPVSOCK="/tmp/mpvsocket"
MPV_FLAGS="--idle --input-ipc-server=$MPVSOCK"

if [ $# -lt 2 ]
then
    echo "usage: $0 <server> <config-file>" >&2
    exit 1
fi

server=$1
config=$2

rm -f "$MPVSOCK"

# Launch mpv
$MPV $MPV_FLAGS &

# Wait the creation of the socket
while ! [ -e $MPVSOCK ] 
do
    sleep 0.2 
done

# Launch server
$server $config $MPVSOCK

exit 0
