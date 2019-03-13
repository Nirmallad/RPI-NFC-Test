#!/bin/bash
# /usr/local/bin/usb-nfc-out &
sleep 1
echo "$(date) - NFC reader ejected" >> /home/pi/nfc_log
echo "USB NFC SCRIPT TERMINATED"
killall serial_out inotifywait

