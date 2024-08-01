#include <Multiplexer4067.h>
#include "MIDIUSB.h"
#include "usb_rename.h"

USBRename usbRename = USBRename("Organ Pedalboard", "Igor Franco", "0001");

const int lowestNote = 36;
boolean wasKeyDown[] = {
  false, false, false, false, false, false, false, false,
  false, false, false, false, false, false, false, false,
  false, false, false, false, false, false, false, false,
  false, false, false
};
Multiplexer4067 mplex = Multiplexer4067(4, 5, 6, 7, A1);
Multiplexer4067 mplex2 = Multiplexer4067(4, 5, 6, 7, A2);

void setup() {
  mplex.begin();
  mplex2.begin();
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
}

void loop() {
  for (int i = 0; i < 27; i++) {
    int value;
    if (i < 16) {
      value = mplex.readChannel(i);
    } else {
      value = mplex2.readChannel(i - 16);
    }
    if (value < 20) {
      keyDown(i);
    } else {
      keyUp(i);
    }
  }
}


void keyDown(uint8_t noteIndex) {
  if (!wasKeyDown[noteIndex]) {
    sendMidiNoteOn(0, lowestNote + noteIndex, 64);
    MidiUSB.flush();
    wasKeyDown[noteIndex] = true;
  }
}
void keyUp(uint8_t noteIndex) {
  if (wasKeyDown[noteIndex]) {
    sendMidiNoteOff(0, lowestNote + noteIndex, 64);
    MidiUSB.flush();
    wasKeyDown[noteIndex] = false;
  }
}

void sendMidiNoteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = { 0x09, 0x90 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOn);
}
void sendMidiNoteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = { 0x08, 0x80 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOff);
}
