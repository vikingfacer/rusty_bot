#!/bin/sh

### BEGIN INIT INFO
# Provides:
# Required-Start:
# Required-Stop:
# Default-Start:
# Default-Stop:
# Short-Description:    TheSpaceShip pi to arduino com system Service
### END INIT INFO

# TheSpaceShip should be in /usr/local/bin

case "$1" in
start)
    /usr/local/TheSpaceShip/TheSpaceShip &
    echo $!>/var/run/TheSpaceShip.pid
    ;;
stop)
    kill `cat /var/run/TheSpaceShip.pid`
    rm /var/run/sax-rest.pid
    ;;
restart)
    $0 stop
    $0 start
    ;;
status)
    if [ -e /var/run/TheSpaceShip.pid ]; then
        echo sax-rest is running, pid=`cat /var/run/TheSpaceShip.pid`
    else
        echo TheSpaceShip is not running
        exit 1
    fi
    ;;
*)
    echo "Usage: $0 {start|stop|status|restart}"
esac