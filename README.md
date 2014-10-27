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
`rake upload[<devicename>, <strategy>]`.

`<devicename>` can be either **reciever** or **transmitter**.
`<strategy>` can be either **production** or **staging**.
