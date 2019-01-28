#!/bin/sh

### BEGIN INIT INFO
# Provides:
# Required-Start:
# Required-Stop:
# Default-Start:
# Default-Stop:
# Short-Description:     runcontroller.sh pi to SpaceShip system Service
### END INIT INFO

#  runcontroller.sh should be in /usr/local/bin

case "$1" in
start)
    ~/runcontroller.sh &
    echo $!>/var/run/runcontroller.pid
    ;;
stop)
    kill `cat /var/run/runcontroller.pid`
    rm /var/run/runcontroller
    ;;
restart)
    $0 stop
    $0 start
    ;;
status)
    if [ -e /var/run/runcontroller.pid ]; then
        echo runcontroller.sh is running, pid=`cat /var/run/runcontroller.pid`
    else
        echo runcontroller.sh is not running
        exit 1
    fi
    ;;
*)
    echo "Usage: $0 {start|stop|status|restart}"
esac