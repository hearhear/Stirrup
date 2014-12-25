#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <pins_arduino.h>
  #include <WProgram.h>
#endif

// To use the library, define a class that subclasses CallBackInterface.
// And also, include a method (C++ talk for "subroutine") called "cbmethod()" in the class.
// Use this class as a template to create your own; it's not hard.  You don't
// even have to understand what you're doing at first.
// How do you subclass?  Like this:
class VolumeButton : public CallBackInterface
{
 public:
   uint8_t count;
   uint8_t pin;
   char *name;

   VolumeButton (uint8_t _pin, char *_name): pin(_pin), name(_name) {
     init();
   };

   void cbmethod() {
     count++;
   };
   
   uint8_t getCount() {
     return count;
   }
   
   char *getName() {
     return name;
   }
   
   uint8_t reset() {
     count=0;
   }

 private:
  void init () {
    pinMode(pin, INPUT_PULLUP);
    PCintPort::attachInterrupt(pin, this, FALLING);
  };
};

