/*
	vf0.1 RECIEVER for gamma-FM
	copyright 2014 Liam Marshall (@archimedespi on Github)

	Thanks to Adafruit for the Adafruit-Si4713-Library.
	Thanks to Steven Cogswell (@scogswell) for ArduinoSerialCommand, and @kroimon for
	their modifications to that library.

	This file is part of Stirrup.

	Stirrup is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Stirrup is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Stirrup.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Wire.h>
#include <Si4703_Breakout.h>
#include <SerialCommand.h>
#include <Logging.h>
#include <ooPinChangeInt.h>
#include <SimpleTimer.h>

#include "volumebutton.h"

/*  Structured Arduino EEPROM access macros
	from https://projectgus.com/2010/07/eeprom-access-with-arduino/
*/
#include <avr/eeprom.h>
#define eeprom_read_to(dst_p, eeprom_field, dst_size) eeprom_read_block(dst_p, (void *)offsetof(__eeprom_data, eeprom_field), MIN(dst_size, sizeof((__eeprom_data*)0)->eeprom_field))
#define eeprom_read(dst, eeprom_field) eeprom_read_to(&dst, eeprom_field, sizeof(dst))
#define eeprom_write_from(src_p, eeprom_field, src_size) eeprom_write_block(src_p, (void *)offsetof(__eeprom_data, eeprom_field), MIN(src_size, sizeof((__eeprom_data*)0)->eeprom_field))
#define eeprom_write(src, eeprom_field) { typeof(src) x = src; eeprom_write_from(&x, eeprom_field, sizeof(x)); }
#define MIN(x,y) ( x > y ? y : x )

#include "constants.h"

//#define FAKE_RADIO // For testing. Turns off all radio calls.

#define RESETPIN 12
#define SDIO 2
#define SCLK 3

#define VOLUME_UP		8
#define VOLUME_DOWN		9

#ifndef FAKE_RADIO
Si4703_Breakout radio(RESETPIN, SDIO, SCLK);
#endif

VolumeButton upButton = VolumeButton(VOLUME_UP, "volumeUp");
VolumeButton downButton = VolumeButton(VOLUME_DOWN, "volumeDown");

SimpleTimer updateTaskTimer;

long recieverChannel = 974;
int recieverVolume = 0;

SerialCommand sCmd;

// Data stored in the EEPROM
struct __eeprom_data {
	int frequencyKHz;
	int volume;
	int v;
};

void setup()
{
	// Start up serial
	Serial.begin(19200);

	// Print out some license stuff
	Serial.println(F("γ-FM :: Reciever"));
	Serial.println(F("γ-FM/HearHear copyright (C) 2014 Liam Marshall"));
	Serial.println(F("This program comes with ABSOLUTELY NO WARRANTY."));
	Serial.println(F("This is free software, and you are welcome to redistribute it"));
	Serial.println(F("under certain conditions."));
	Serial.println();

	Logging::info("Powering radio on");
	radio.powerOn();


	Logging::info("Loading settings from EEPROM");
	loadCommand();
	Logging::info("Current settings are: ");
	settingsCommand();

	Logging::info_nonl("Tuning to ");
	Serial.print(recieverChannel);
	Serial.println(F(" KHz"));
	#ifndef FAKE_RADIO
	radio.setChannel(recieverChannel);
	#endif

	digitalWrite(13, HIGH);

	updateTaskTimer.setInterval(1500, updateTask);

	setupCommands();
}

void eeprom_save_all() {
	eeprom_write(recieverChannel, frequencyKHz);
	eeprom_write(recieverVolume, volume);
}

void setupCommands() {
	sCmd.addCommand("tune", tuneCommand);
	sCmd.addCommand("volume", volumeCommand);
	sCmd.addCommand("save", saveCommand);
	sCmd.addCommand("load", loadCommand);
	sCmd.addCommand("settings", settingsCommand);
	sCmd.setDefaultHandler(unrecognizedCommand);
}

void tuneCommand() {
	char *arg;

	arg = sCmd.next();
	if (arg == NULL) {
		Logging::warning("`tune` must have arguments!");
		return;
	}
	recieverChannel = strtol(arg, NULL, 0);
	Logging::info_nonl("Tuning radio to ");
	Serial.print(recieverChannel);
	Serial.println("KHz");
	#ifndef FAKE_RADIO
	radio.setChannel(recieverChannel);
	#endif
}

void volumeCommand() {
	char *arg;

	arg = sCmd.next();
	if (arg == NULL) {
		Logging::warning("`volume` must have arguments!");
		return;
	}
	recieverVolume = strtol(arg, NULL, 0);
	Logging::info_nonl("Setting volume to ");
	Serial.println(recieverVolume);
	#ifndef FAKE_RADIO
	radio.setVolume(recieverVolume);
	#endif
}

void saveCommand() {
	Logging::info("Saving settings to EEPROM");
	eeprom_save_all();
}

void loadCommand() {
	Logging::info("Loading settings from EEPROM");
	eeprom_read(recieverChannel, frequencyKHz);
	eeprom_read(recieverVolume, volume);
	Logging::info("Updating settings from loaded data...");
	#ifndef FAKE_RADIO
	radio.setChannel(recieverChannel);
	radio.setVolume(recieverVolume);
	#endif
}

void settingsCommand() {
	Logging::info("Frequency (KHz): " + String(recieverChannel));
	Logging::info("Volume: " + String(recieverVolume));
}

void unrecognizedCommand(const char *command) {
	Logging::warning("Unrecognized command: " + String(command) + ".");
}

void updateTask() { // Runs every 1500ms.
	recieverVolume = recieverVolume + (upButton.getCount() - downButton.getCount());
	int oldRecieverVolume = 0; eeprom_read(oldRecieverVolume, volume);
	if (recieverVolume < 0) {
		recieverVolume = 0;
	} else if (recieverVolume > 14) {
		recieverVolume = 14;
	}
	if (recieverVolume != oldRecieverVolume) {
		eeprom_write(recieverVolume, volume);
		#ifndef FAKE_RADIO
		radio.setVolume(recieverVolume);
		#endif
		upButton.reset(); downButton.reset();
	}
}

void loop()
{
	sCmd.readSerial();
	updateTaskTimer.run();
}
