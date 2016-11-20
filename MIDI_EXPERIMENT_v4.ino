#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

//const int BUTTON_1 = 2;
//const int BUTTON_2 = 3;
//const int BUTTON_3 = 4;

//const int POT_1 = 0;
//const int POT_2 = 1;

const int LED_GREEN = 6;
const int LED_RED = 7;

const int accelIn1 = 8;
const int accelIn2 = 9;
const int accelIn3 = 10;

bool accelNum1 = 0;
bool accelNum2 = 0;
bool accelNum3 = 0;

//bool buttonState1;
//bool buttonState2;
//bool buttonState3;

//bool canBeDisabled = true;

//int potState1;
//int potState2; 

bool noteStates[127];

// keySig = 
// 0 : major
// 1 : minor
// 2 : dorian
// 3 : mixolydian
int keySig = 0;

bool noteIsInC(int note) {
  bool isInC = 1;
  if ((note-1)%12 == 0 || (note-3)%12 == 0 ||(note-6)%12 == 0 || (note-8)%12 == 0 || (note-10)%12 == 0) {
    isInC = 0;
  }
  return isInC;
}

void handleNoteOn(byte channel, byte pitch, byte velocity) {
  if(noteIsInC(int(pitch))) {
    if (keySig == 1) {
      if ((int(pitch)-4)%12 == 0 || (int(pitch)-9)%12 == 0 || (int(pitch)-11)%12 == 0){
        MIDI.sendNoteOn(int(pitch)-1, velocity, channel);
        noteStates[pitch] = 1;
      } else {
        MIDI.sendNoteOn(pitch, velocity, channel);
        noteStates[pitch] = 1;
      }
    } else if (keySig == 2) {
      if ((int(pitch)-4)%12 == 0 || (int(pitch)-11)%12 == 0){
        MIDI.sendNoteOn(int(pitch)-1, velocity, channel);
        noteStates[pitch] = 1;
      } else {
        MIDI.sendNoteOn(pitch, velocity, channel);
        noteStates[pitch] = 1;
      }
    }else if (keySig == 3) {
      if ((int(pitch)-11)%12 == 0){
        MIDI.sendNoteOn(int(pitch)-1, velocity, channel);
        noteStates[pitch] = 1;
      } else {
        MIDI.sendNoteOn(pitch, velocity, channel);
        noteStates[pitch] = 1;
      }
    } else {
      MIDI.sendNoteOn(pitch, velocity, channel);
      noteStates[pitch] = 1;
    }
  } 
} 

void handleNoteOff(byte channel,byte pitch, byte velocity) {
  if(noteIsInC(int(pitch))) {
    if (keySig == 1) {
      if ((int(pitch)-4)%12 == 0 || (int(pitch)-9)%12 == 0 || (int(pitch)-11)%12 == 0){
        MIDI.sendNoteOff(int(pitch)-1, velocity, channel);
        noteStates[pitch] = 0;
      } else {
        MIDI.sendNoteOff(pitch, velocity, channel);
        noteStates[pitch] = 0;
      }
    } else if (keySig == 2) {
      if ((int(pitch)-4)%12 == 0 || (int(pitch)-11)%12 == 0){
        MIDI.sendNoteOff(int(pitch)-1, velocity, channel);
        noteStates[pitch] = 0;
      } else {
        MIDI.sendNoteOff(pitch, velocity, channel);
        noteStates[pitch] = 0;
      }
    }else if (keySig == 3) {
      if ((int(pitch)-11)%12 == 0){
        MIDI.sendNoteOff(int(pitch)-1, velocity, channel);
        noteStates[pitch] = 0;
      } else {
        MIDI.sendNoteOff(pitch, velocity, channel);
        noteStates[pitch] = 0;
      }
    } else {
      MIDI.sendNoteOff(pitch, velocity, channel);
      noteStates[pitch] = 0;
    }
  } 
} 


void setup() {
  pinMode(accelIn1, INPUT);
  pinMode(accelIn2, INPUT);
  pinMode(accelIn3, INPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, HIGH);
 // pinMode(BUTTON_1, INPUT_PULLUP);
  //pinMode(BUTTON_2, INPUT_PULLUP);
  //pinMode(BUTTON_3, INPUT_PULLUP);
  //pinMode(POT_1, INPUT);
  //pinMode(POT_2, INPUT);

  for (int i = 0; i <= 127; i++ ) {
    noteStates[i] = 0;
  }
  
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
}

  


void loop() {
  accelNum1 = digitalRead(accelIn1);
  accelNum2 = digitalRead(accelIn2);
  accelNum3 = digitalRead(accelIn3);
  //buttonState1 = digitalRead(BUTTON_1);
  //buttonState2 = digitalRead(BUTTON_2);
  //buttonState3 = digitalRead(BUTTON_3);
  //potState1 = analogRead(POT_1);
  //potState2 = analogRead(POT_2);

  if (accelNum1 == 0 && accelNum2 == 0 && accelNum3 == 0) {
    keySig = 0;
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, HIGH);
  } else if (accelNum1 == 1 && accelNum2 == 0 && accelNum3 == 0) {
    keySig = 1;
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);
  } else if (accelNum1 == 0 && accelNum2 == 1 && accelNum3 == 0) {
    keySig = 2;
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
  } else if (accelNum1 == 1 && accelNum2 == 1 && accelNum3 == 0) {
    keySig = 3;
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, LOW);
  }

// changing from different keySig
/*
  if (buttonState1 == LOW && keySig != 1) {
    keySig = 1;
    canBeDisabled = false;
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);
  } else if (buttonState2 == LOW && keySig != 2) {
    keySig = 2;
    canBeDisabled = false;
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
  } else if (buttonState3 == LOW && keySig != 3) {
    keySig = 3;
    canBeDisabled = false;
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, LOW);

//------------------------------------- changing same keySig to 0 (pressed button again)
    
  } else if (buttonState1 == LOW && canBeDisabled){
    keySig = 0;
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, HIGH);
  } else if (buttonState2 == LOW && canBeDisabled){
    keySig = 0;
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, HIGH);
  } else if (buttonState3 == LOW && canBeDisabled){
    keySig = 0;
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, HIGH);
  }
  
  if (!canBeDisabled && buttonState1 == HIGH && buttonState2 == HIGH && buttonState3 == HIGH){
    canBeDisabled = true;
  }*/

  MIDI.read(); 
}
