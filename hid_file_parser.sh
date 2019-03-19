#!/bin/bash

filename="$(dmesg | grep 1DB2:060F. | grep  hidraw | cut --complement -d "," -f 1,7 | cut -b 1-7)"
echo "filename ${filename}"
sudo ./hid_parser $filename
echo "script complete"
