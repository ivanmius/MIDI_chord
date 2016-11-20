#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

const int BUTTON_1 = 2;
const int BUTTON_2 = 3;
const int BUTTON_3 = 4;

const int POT_1 = 0;
const int POT_2 = 1;

const int LED_GREEN = 6;
const int LED_RED = 7;

bool buttonState1;
bool buttonState2;
bool buttonState3;

bool button1Disabled = false;
bool button2Disabled = false;
bool button3Disabled = false;

unsigned long button1Timer;
unsigned long button2Timer;
unsigned long button3Timer;
unsigned long timeGap;

const unsigned long repressDelay = 50;
const unsigned long maxDelay = 125;

int potState1;
int potState2; 

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
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, HIGH);
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);
  pinMode(POT_1, INPUT);
  pinMode(POT_2, INPUT);

  for (int i = 0; i <= 127; i++ ) {
    noteStates[i] = 0;
  }
  
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
}

  


void loop() {
  buttonState1 = digitalRead(BUTTON_1);
  buttonState2 = digitalRead(BUTTON_2);
  buttonState3 = digitalRead(BUTTON_3);
  potState1 = analogRead(POT_1);
  potState2 = analogRead(POT_2);

  if (buttonState1 == LOW && !button1Disabled){
    if (keySig != 1){
      keySig = 1;
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, HIGH);
    } else {
      keySig = 0;
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, HIGH);
    }
    button1Disabled = true;
  }
  if (buttonState2 == LOW && !button2Disabled){
    if (keySig != 2){
      keySig = 2;
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, LOW);
    } else {
      keySig = 0;
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, HIGH);
    }
    button2Disabled = true;
  }
  if (buttonState3 == LOW && !button3Disabled){
    if (keySig != 3){
      keySig = 3;
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_RED, LOW);
    } else {
      keySig = 0;
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_RED, HIGH);
    }
    button3Disabled = true;
  }

//---------------------------------- re-enabling disabled buttons
  
  if (button1Disabled && buttonState1 == HIGH){
    timeGap = millis() - button1Timer;
    if (timeGap > maxDelay) { // button was just released
      button1Timer = millis();
    } else if (timeGap > repressDelay) { // repress delay has passed
      button1Disabled = false;
    }
  }
  if (button2Disabled && buttonState2 == HIGH){
    timeGap = millis() - button2Timer;
    if (timeGap > maxDelay) {
      button2Timer = millis();
    } else if (timeGap > repressDelay) {
      button2Disabled = false;
    }
  }
  if (button3Disabled && buttonState3 == HIGH){
    timeGap = millis() - button3Timer;
    if (timeGap > maxDelay) {
      button3Timer = millis();
    } else if (timeGap > repressDelay) {
      button3Disabled = false;
    }
  }

  MIDI.read(); 
}
