#include <avr/sleep.h>
#include <avr/interrupt.h>

const int switchPin = 3;
const int analogPin = A1;
const int statusLED = 0; 
const int maxBrightness = 240;
unsigned long timerA;
unsigned long timerB;
int ledFlag;
int ledOn;
int dim;
void setup() {

    pinMode(switchPin, INPUT);
    pinMode(statusLED, OUTPUT);
    ledFlag = 0;
    delay(500);  //think about it
    ledOn = 0;
    dim = 0;
    timerB = millis();
} 



void loop() {

    if (digitalRead(switchPin)==HIGH) { //elegxos diakopti selas
      digitalWrite(statusLED, LOW); //prin to sleep na sbino oti einai anameno
      sleep(); 
    } else {
      //edo na balo delay mexri na anoiksei to kapaki gia na parei sosto fos o photoresistor
      if (millis()-timerB>500) { //edo perimenei miso deyterolepto mexri na anoiksei ligo i sela kai na parei sosti metrisi toy photos
        int photoResistor = analogRead(analogPin);
        if (!ledFlag && photoResistor<30)  
          turnOnTheLed();
        if (ledOn &&(millis()-timerA>60000) && !dim) { //10secs apo tin ora poy ksekinise na anabei to led
          dimLED();
        } else if(ledOn &&((millis()-timerA>90000))) {// && ledFlag
          turnOffTheLed();
          sleep(); 
        }
          //digitalWrite(statusLED,LOW);
      }
    }
}


void dimLED() {
  for (int i=maxBrightness;i>=45;i--) {
    analogWrite(statusLED,i);
    delay(10);
  } 
  dim = 1;
}


void turnOffTheLed() {
  ledOn = 0;
  for (int i=45;i>=0;i--) {
    analogWrite(statusLED,i);
    delay(10);
  } 
  digitalWrite(statusLED,LOW);
}

void turnOnTheLed() {
  timerA = millis();
  ledFlag = 1;
  ledOn = 1;
  for (int i=0;i<maxBrightness;i+=5) {  //ayto to exo
    analogWrite(statusLED,i);           //gia na anoigei
    delay(15);                         //dimmarismeno
  } 
  analogWrite(statusLED,maxBrightness);
}

void sleep() {
  GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
  PCMSK |= _BV(PCINT3);                   // Use PB4 as interrupt pin
  ADCSRA &= ~_BV(ADEN);                   // ADC off
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement
  
  sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
  sei();                                  // Enable interrupts
  sleep_cpu();                            // sleep
  
  cli();                                  // Disable interrupts
  PCMSK &= ~_BV(PCINT3);                  // Turn off PB4 as interrupt pin
  sleep_disable();                        // Clear SE bit
  ADCSRA |= _BV(ADEN);                    // ADC on
  
  sei();                                  // Enable interrupts
} // sleep


//ISR(PCINT3_vect) {
      //something interesting
//}


