#!/bin/bash
# /usr/local/bin/usb-nfc-in &
sleep 1
echo "$(date) - NFC Reader Detected" >> /home/pi/nfc_log
echo "USB NFC SCRIPT STARTED" >> /home/pi/nfc_log
./home/pi/serial_out >> /home/pi/nfc_log



