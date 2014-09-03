#!/bin/sh
### BEGIN INIT INFO
# Provides:          arduino
# Required-Start:    $network
# Required-Stop:     $network
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Arduino Light Control Interface
### END INIT INFO

setModeOn() {
	arduinoctl 1
}

setModeOff() {
	arduinoctl 0
}

setModeNight() {
	arduinoctl 2
}

case "$1" in
	start)
		setModeOn
		;;
	stop)
		setModeNight
		;;
esac
