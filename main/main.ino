#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();
static const int CHANNEL = 4;
static const int[7] C4major = {72, 74, 76, 77, 79, 81, 83};
static const int[7] C4minor = {72, 74, 75, 77, 79, 80, 82};
static const int pushButton1 = 2;
boolean changeMode = false;

void NoteOn( byte channel, byte pitch, byte velocity){
  boolean inRange = false;
  int notePos = 0;
  for (int i = 0; i < C4major.length(); i++){
    if (byte(C4major[i]) == pitch){
      inRange = true;
      notePos = i;
    }
  }

  if (inRange){
    MIDI.sendNoteOn(byte(C4minor[i]), velocity, channel);
  } else {
    MIDI.sendNoteOn(pitch, velocity, channel);
  }
}

void NoteOff(byte channel, byte pitch, byte velocity){
  boolean inRange = false;
  int notePos = 0;
  for (int i = 0; i < C4major.length(); i++){
    if (byte(C4major[i]) == pitch){
      inRange = true;
      notePos = i;
    }
  }

  if (inRange && changeMode){
    MIDI.sendNoteOff(byte(C4minor[i]), velocity, channel);
  } else {
    MIDI.sendNoteOff(pitch, velocity, channel);
  }  
}

void setup() {
  pinMode(pushButton, INPUT);
  MIDI.setHandleNoteOn(NoteOn);
  MIDI.setHandleNoteOff(NoteOff);
  MIDI.begin(CHANNEL);
}

void loop() {
    MIDI.read();
    if (digitalRead(pushButton) == HIGH){
      changeMode = true;
    } else {
      changeMode = false;
    }
  }

