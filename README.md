Stirrup
=======

Firmware for the γ version of the FM transmitter (vf0.1 in the board repo)

Has transmitter and reciever code, compile and upload with Ino.
`cd` to one of the code directories (`reciever/`, `transmitter/`), and run `ino build`, then
`ino upload /dev/<yourdevice>`.
Requires these libraries:

* [Adafruit-Si4713-Library](https://github.com/adafruit/Adafruit-Si4713-Library)
* [Si4703_Breakout](https://github.com/sparkfun/Si4703_FM_Tuner_Evaluation_Board/tree/master/Libraries/Si4703_Breakout)

They're built in to the project in several `lib/` directories.