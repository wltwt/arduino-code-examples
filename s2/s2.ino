// Økt 2
// PWM pin har ~ forran

#define COMMON_ANODE

const int l_pin = 11;
struct PWM 
{
    int pin;
    int value;
    int change;
};

struct Usertime {
  unsigned long now = 0;
  unsigned long earlier = 100;
};

struct State {
  int led = LOW;
  int A0_Value = 0;
  const int photocellPin = A0;
  unsigned long count = 0;
};

struct RGB {
    byte pins[3] = {6,3,5};
    int r;
    int g;
    int b;
};

struct Button {
  int pin;
  int lastState;
  unsigned long lastDebounceTime;
  unsigned long debounceDelay;
  int toggle;
  int state;
  int count;
  int holdTime;
};

Button button = {7, HIGH, 0, 25, HIGH};
PWM pwm = {11, 0, 11}; // kanskje ikke beste variabelnavn
RGB rgb;
Usertime t, photocell, blink;
State state, ledRGB;

void setup() {
  Serial.begin(9600);
  pinMode(pwm.pin, OUTPUT); // definert utgang på PWM pin 11
  for (int i = 0; i<sizeof(rgb.pins);i++){
      pinMode(rgb.pins[i], OUTPUT); 
  }
  pinMode(button.pin, INPUT_PULLUP);
}

void loop() {
    debounceButton();

    blinkRGB(button.holdTime);
    readPhotocell(2);
}

void setColor(int red, int green, int blue) {
    #ifdef COMMON_ANODE
        red = 255 - red;
        green = 255 - green;
        blue = 255 - blue;
    #endif

    analogWrite(rgb.pins[0], red);
    analogWrite(rgb.pins[1], green);
    analogWrite(rgb.pins[2], blue);
}

void updatePhotocell(int pin) {
  state.A0_Value = map(analogRead(pin), 200, 0, 0, 255);
}

void blinkRGB(int timer) {
  blink.now = millis();
  if (blink.now - blink.earlier >= timer) {
    blink.earlier=blink.now;
    if (ledRGB.led) {
      ledRGB.led = LOW;
    } else {
      ledRGB.led = HIGH;
    }
    setColor(ledRGB.led, ledRGB.led, ledRGB.led);
  } 
}
/*
  Ikke-blokkerende metode som leser verdien til fotocellen hvert 2. millisekund.
  Ser ut som man bør gjøre det slik at analog til digital konvertoren får litt tid
  mellom hver lesing.
*/
void readPhotocell(int timer) {
  photocell.now = millis();
  if (photocell.earlier == t.earlier) {
    photocell.earlier = timer;
  }
  if (photocell.now - photocell.earlier >= timer) {
    photocell.earlier = photocell.now;
    updatePhotocell(state.photocellPin);
  }
}

void debounceButton() {
  int reading = digitalRead(button.pin);

  if (reading != button.lastState) {          // knapp trykket inn: kjør kode, knapp opp: kjør kode, ikke mellom
    if (reading != LOW && button.lastDebounceTime > button.debounceDelay) {
      // Serial.print("Button held for: ");
      int buttonHold = millis() - button.lastDebounceTime;
      button.holdTime = buttonHold;
      // Serial.print(button.holdTime);
      // Serial.println("\n");

    }
    button.lastDebounceTime = millis();       // 100 ms
  }
  
  if ((millis() - button.lastDebounceTime) >= button.debounceDelay) {    // knappen må være holdt inne i 50 ms for at den skal gå her
    if (reading != button.state) {
      button.state = reading;
      if(button.state == LOW) {
        button.toggle = !button.toggle;
        button.count++;
      }
    }
  }
  button.lastState = reading;
}
