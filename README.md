# Overview

Example firmware for the cheap STM8-based W1209 temperature control board.

This simply allows for control of the relay, and displays the raw ADC value from a resistive input. It is intended for calibrating your thermistor/photodiode/etc.

The three button pins are initialized as inputs with pull-ups, and currently the 'Set' button toggles the relay on and off.

THIS FIRMWARE IS UNSAFE FOR USE WITH A HEATING ELEMENT. The relay can be left on indefinitely, which could burn your place down if you're using a 12V heating cartridge. Be careful, and note the 'no warranty of any kind' part of the license.

# Running it

Build with SDCC:

`sdcc -lstm8 -mstm8 --out-fmt-ihx --std-sdcc11 main.c`

Flash with the 'stm8flash' project and a generic STLink/v2 module (substitute stm8s103f3 if appropriate):

`stm8flash -c stlinkv2 -p stm8s003f3 -w main.ihx`

If you run into issues flashing, try resetting the chip to factory defaults first:

`echo "00 00 ff 00 ff 00 ff 00 ff 00 ff" | xxd -r -p > factory_defaults.bin`

`stm8flash -c stlinkv2 -p stm8s003f3 -s opt -w factory_defaults.bin`

I haven't figured out on-chip debugging with OpenOCD yet.
