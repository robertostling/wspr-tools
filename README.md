# wspr-tools
Code for the WSPR protocol

## encode.py

```python3 encode.py callsign locator power```

Currently only simple callsigns and 4-symbol locators are accepted. Power is
in dBm. This can be used directly for producing transmission sequences for the
Arduino/AD9851 based transmitter below, or as a library to encode the WSPR
protocol.

## manual.py

```python3 manual.py callsign locator power```

Used to display the straight-key transmission sequence for a narrow-band
frequency shift transmitter. Please refer to
<a href="http://robos.org/sections/radio/wspr/">my web page on
non-standard WSPR equipment</a> for more information.

## arduino_ad9851.ino

Arduino program to actually transmit the data generated by `encode.py` using
an AD9851 module.

**Note**: if you use the square wave output (QOUT1 or its inverse QOUT2),
make sure to tune the comparator voltage to achieve a reasonable duty cycle.
If followed by a class E amplifier this is about 45% for maximum power output
(anywhere near 50% should be OK). I use a DC voltmeter and approximate the
duty cycle with V/5, so that a measured 2V reading translates into a 40% duty
cycle.

You could of course also take either of the sine wave outputs (ZOUT1, ZOUT2)
and amplify or feed directly into a low-pass filter to the antenna.

## make_table.py and make_sound.py

Code to support completely analog operation. Run `make_table.py` and write
down the printed table on a piece of paper, making sure to include the
horizontal lines after every 9th symbol. I simply fill the indicated
squares with a pencil, and the whole message takes three columns on a piece of
A4 graph paper.

Then run `make_sound.py`, which creates the file `ticks.wav`. This file can be
played (by an analog tape player, perhaps?) starting at an even minute. You
should have no problem following the beat when keying the message, but if you
lose track at some point, the higher tone at the symbol *before* each 9-symbol
block boundary can be used for synchronization.

