/*FUNCTIONS
   Pre/Clear Function
   ~Send rising edge every minute
   ~Magic Bit
   ~Demo Mode

   TODO:
   Test millis()
   Test Demo Pin
   Test Interrupt

   OPTIONS:
   trigger signal to other board to trigger ground on/off
   run final layer on test code w/ other board
*/


#include <Time.h>
//"IT IS" and "O' CLOCK" are always on

//PIN NUMBER SETUP
const int serialIn = 0;
const int serialOut = 1;
const int clockPin = 2; //sends a signal every minute
const int clearPin1 = 4; //level 1
const int prePin1 = 5; //level 1
const int clearPin2 = 6; //level 2
const int prePin2 = 7; //level 2
const int MRPin1 = 8; //Master Reset level 1
const int MRPin2 = 9; //Master Reset Level 2
const int Reset12Pin = 10;
const int s1Pin = 11; //also mux load, also controls Past(high when backwards)
const int s0Pin = 12; //also controls To (high when forwards)
const int demoPin = 13; //when HIGH, enter Demo Mode (accelerated clock rate to show clock functioning)

//COUNTERS
int demoCounter = 0; // counts 0.3 seconds until clock ticks in demo mode
int minuteCounter = 0; // counter number of minutes to change past and to

//OTHER VARIABLES
int currentMin;
bool minSignal = false; // changes to true when arduino notices minute has changed
int demoModeState; //tells when demo mode is on
int SOState; //serial out state
bool reset = true;

void setup() {
  // put your setup code here, to run once:
  pinMode(serialIn, OUTPUT);
  pinMode(serialOut, INPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(prePin1, OUTPUT);
  pinMode(clearPin1, OUTPUT);
  pinMode(prePin2, OUTPUT);
  pinMode(clearPin2, OUTPUT);
  pinMode(MRPin1, OUTPUT);
  pinMode(MRPin2, OUTPUT);
  pinMode(Reset12Pin, OUTPUT);
  pinMode(s1Pin, OUTPUT);
  pinMode(s0Pin, OUTPUT);
  pinMode(demoPin, INPUT_PULLUP); //high when not plugged in, low when at ground
  //  Serial.begin(9600);

  digitalWrite(Reset12Pin, LOW); //reset 12bit
  digitalWrite(Reset12Pin, HIGH);

  Serial.println("Setting up the first 2 layers the clock...");
  digitalWrite(prePin1, HIGH);
  digitalWrite(clearPin1, HIGH);

  digitalWrite(prePin2, LOW);
  digitalWrite(clearPin2, LOW);

  digitalWrite(clearPin1, LOW);

  digitalWrite(MRPin1, LOW);
  digitalWrite(MRPin1, HIGH);

  digitalWrite(clearPin1, HIGH);

  digitalWrite(prePin2, HIGH);

  digitalWrite(prePin1, LOW);

  digitalWrite(prePin1, HIGH);

  digitalWrite(clearPin1, LOW);

  digitalWrite(clearPin1, HIGH);

  digitalWrite(prePin1, LOW);

  digitalWrite(MRPin2, LOW);
  digitalWrite(MRPin2, HIGH);

  digitalWrite(prePin2, LOW);

  digitalWrite(clearPin2, HIGH);

  digitalWrite(prePin1, HIGH);

  digitalWrite(clearPin2, LOW);

  Serial.println("done!");
  digitalWrite(s1Pin, LOW);
  digitalWrite(s0Pin, HIGH);
  digitalWrite(serialIn, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  demoModeState = digitalRead(demoPin);
  minSignal = checkMin(demoModeState); //returns true if minute has changed (will change every 3 seconds for demo mode)
  if (minuteCounter > 59) {
    minuteCounter = 0; //Reset minute counter once it reaches 60
  }
  if (minSignal == true) {
    if (reset == true) {
      for (int j = 0; j < 2; j++) {
        if (j == 1) {
          digitalWrite(s1Pin, LOW);
          digitalWrite(s0Pin, HIGH);
          //delay(500);
        }
        digitalWrite(clockPin, HIGH); //simulate rising edge
        digitalWrite(clockPin, LOW);
        if (reset == true) {
          for (int i = 0; i < 59; i++) {
            digitalWrite(clockPin, HIGH); //simulate rising edge
            digitalWrite(clockPin, LOW);
            //delay(500);
            if (i == 28) {
              digitalWrite(s1Pin, HIGH);
              digitalWrite(s0Pin, LOW);
            }
          }
          Serial.println("Done looping!");
        }
      }
    }

    if (reset == false) {
      SOState = !digitalRead(serialOut);
      digitalWrite(serialIn, SOState);
      //digitalWrite(gbar, HIGH);
      //digitalWrite(gbar, LOW);
      Serial.print("S: ");
      Serial.println(serialIn);
      digitalWrite(clockPin, HIGH); //simulate rising edge
      digitalWrite(clockPin, LOW);
    }

    Serial.print("M:");
    Serial.println(minuteCounter);
    reset = false;
    minuteCounter++;
  }
  if (pastOrTo(minuteCounter) == 0) { //4 bit runs forwards for 0-30

    digitalWrite(s1Pin, LOW);
    digitalWrite(s0Pin, HIGH);

  } else if (pastOrTo(minuteCounter) == 1) { //4 bit runs backwards for 30-59
    digitalWrite(s1Pin, HIGH);
    digitalWrite(s0Pin, LOW);
  }
}

bool checkMin(int state) {
  if (state == LOW) { //if low, function as normal
    int min1 = millisToMinutes();
    delay(1000);
    int min2 = millisToMinutes();
    if (min2 > min1) {
      return true;
    } else {
      return false;
    }
  }
  if (state == HIGH) { //if high, function at accelerated rate
    while (true) {
      delay(100);
      demoCounter++;
      if (demoCounter == 3) {
        demoCounter = 0;
        return true;
      }
      else {
        return false;
      }
    }
  }
}
int pastOrTo (int currentMin) {
  if (currentMin <= 30) {
    return 0;
  } else if (currentMin > 30) {
    return 1;
  }
}

int millisToMinutes() {
  const long milsInMinute = 60000;
  long currentTime = millis();
  int numMinutes = currentTime / milsInMinute;
  return numMinutes;
}

