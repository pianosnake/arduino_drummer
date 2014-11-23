#include <MIDI.h>

byte pedal = 4;
byte bassDrum = 8;
byte snareDrum1 = 9;  
byte snareDrum2 = 10;
byte snareAlternate = true;

byte bassDrumPitch = 36;
byte snareDrumPitch = 38;

unsigned long now = 0;
unsigned long bassDrumStart = 0;
unsigned long snareDrum1Start = 0;
unsigned long snareDrum2Start = 0; 

// hold down the motors for this long so that there's enough current going through the motor to cause it to smack down on the drum
// this method ignores the note=OFF/velocity=0 messages from the MIDI controller
byte holdDuration = 40; 
byte pedalState = 0; 

// This function will be called by the Midi Library
// when a MIDI NOTE ON message is received.
void MyHandleNoteOn(byte channel, byte pitch, byte velocity) {
   if (velocity > 0) {
      if(pitch == snareDrumPitch){
        if(snareAlternate == true){
          digitalWrite(snareDrum1, HIGH);
          snareDrum1Start = millis();       
          snareAlternate= false;
        }else{
          digitalWrite(snareDrum2, HIGH);
          snareDrum2Start = millis();  
          snareAlternate = true; 
        } 
      }else if (pitch == bassDrumPitch){
        digitalWrite(bassDrum, HIGH);
        bassDrumStart = millis();
    }
  }
}


void turnOffNotes(){
  now = millis();
  if (now - snareDrum1Start >  holdDuration){
     digitalWrite(snareDrum1, LOW);
  }
   if (now - snareDrum2Start >  holdDuration){
     digitalWrite(snareDrum2, LOW); 
  }
  if (now - bassDrumStart >  holdDuration){
     digitalWrite(bassDrum, LOW);
  }
}

void setup() {
  pinMode(pedal, INPUT_PULLUP); 
  pinMode(snareDrum1, OUTPUT);
  pinMode(snareDrum2, OUTPUT);
  pinMode(bassDrum, OUTPUT);
  
  MIDI.begin(MIDI_CHANNEL_OMNI); // MIDI_CHANNEL_OMNI = all
  MIDI.setHandleNoteOn(MyHandleNoteOn);
}

void loop() { 
    pedalState = digitalRead(pedal);
    if(pedalState == LOW){
      MIDI.read(); 
    }
    turnOffNotes();
}
