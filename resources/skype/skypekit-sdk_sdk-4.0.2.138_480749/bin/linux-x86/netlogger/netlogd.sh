#! /bin/sh
# /etc/init.d/netlogd: start the skypekit log daemon.

PATH=/bin:/usr/bin:/sbin:/usr/sbin

binpath=/usr/sbin/netlogd

test -f $binpath || exit 0
. /lib/lsb/init-functions

case "$1" in
  start)
    log_begin_msg "Starting network logging daemon..."
    $binpath
    ;;
  stop)
    log_begin_msg "Stopping network logging daemon..."
    killall netlogd

    ;;
  restart|force-reload)
    $0 stop
    sleep 1
    $0 start
    ;;
  *)
    log_success_msg "Usage: /etc/init.d/netlogd {start|stop|restart|force-reload}"
    exit 1
esac

exit 0
