avrdude -c avrispmkII -p ATtiny85 -P usb -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
