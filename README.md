Stirrup
=======

Firmware for the γ version of the FM transmitter (vf0.1 in the board repo)


Hacking on the code
====================
Has transmitter and reciever code, compile and upload with the Rake scripts.
Requires Ino, so install that: `pip install ino`.
Requires Rake, so install that: `gem install rake`.
If you don't have Ruby, get it with [RVM](http://rvm.io).
You can get it other ways, but RVM is astronomically better.

### To build the code:
`rake build[<devicename>,<strategy>]`.

### To upload to a device:
`rake upload[<devicename>,<strategy>]`.

`<devicename>` can be either **reciever** or **transmitter**.
`<strategy>` can be either **production** or **staging**.

Attributions, licensing, authors, etc.
===========================================

> Stirrup - Firmware for FM transmitting/recieving systems for assistive tech.
> Copyright (C) 2014  Liam Marshall

> This program is free software: you can redistribute it and/or modify
> it under the terms of the GNU General Public License as published by
> the Free Software Foundation, either version 3 of the License, or
> (at your option) any later version.
> 
> This program is distributed in the hope that it will be useful,
> but WITHOUT ANY WARRANTY; without even the implied warranty of
> MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
> GNU General Public License for more details.

Thanks to Adafruit for their Si4713 library.
Thanks to Sparkfun for their Si4703 library.
Thanks to Steven Cogswell for ArduinoSerialCommand.
Thanks to Stefan Rado for Arduino-SerialCommand, a modification and refactor of ArduinoSerialCommand.

Sadly, the only author of the code is myself (as of now).
Please don't use this in commercial devices with no modification - I have to make money too.
But feel free to sell devices which use significantly modified versions of this code.