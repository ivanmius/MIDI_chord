#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();
const int CHANNEL = 4;

void NoteOn( byte channel, byte pitch, byte velocity){
  
}

void NoteOff(byte channel, byte pitch, byte velocity){
  
}

void setup() {
  MIDI.setHandleNoteOn(NoteOn);
  MIDI.setHandleNoteOff(NoteOff);
  MIDI.begin(CHANNEL);
}

void loop() {
    MIDI.read();
  }

