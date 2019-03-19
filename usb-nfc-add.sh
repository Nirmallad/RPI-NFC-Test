#!/bin/bash
# /usr/local/bin/usb-nfc-in &
sleep 1
echo "$(date) - NFC Reader Detected" >> /home/pi/nfc_log
echo "USB NFC SCRIPT STARTED" >> /home/pi/nfc_log
#./home/pi/serial_out >> /home/pi/nfc_log
filename="$(dmesg | grep 1DB2:060F. | grep  hidraw | cut --complement -d "," -f 1,7 | cut -b 1-7)"
echo "filename ${filename}"
sudo ./home/pi/serial_out $filename


