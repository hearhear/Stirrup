Stirrup
=======

Firmware for the γ version of the FM transmitter (vf0.1 in the board repo)

Has transmitter and reciever code, compile and upload with Ino.
Just do `make build` and `make upload <somedevice>`. Or `make all <somedevice>`. Whatever.
Requires these libraries:

* [Adafruit-Si4713-Library](https://github.com/adafruit/Adafruit-Si4713-Library)
* [Si4703_Breakout](https://github.com/sparkfun/Si4703_FM_Tuner_Evaluation_Board/tree/master/Libraries/Si4703_Breakout)

They're built in to the project in several `lib/` directories.