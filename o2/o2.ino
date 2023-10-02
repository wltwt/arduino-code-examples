// Økt 2
// NB! viktig å bruke en utgangspin som har ~ forran seg

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

PWM pwm = {11, 0, 11}; // kanskje ikke beste variabelnavn
RGB rgb;
Usertime t, photocell;
State state;

void setup() {
    pinMode(pwm.pin, OUTPUT); // definert utgang på PWM pin 11
    for (int i = 0; i<sizeof(rgb.pins);i++){
        pinMode(rgb.pins[i], OUTPUT); 
    }
}

void loop() {
    // bruker map her slik at man kan bruke hvilket interval man vil til endring av PWM verdi 
    analogWrite(pwm.pin, map(pwm.value, 0, 255+abs(pwm.change), 0, 255));

    pwm.value = pwm.value + pwm.change;

    // vil blinke om "change" variabelen ikke er delelig på 255 uten map-funksjonen over
    if (pwm.value <= 0 || pwm.value >= 255 ) {
        pwm.change = -pwm.change;
        state.count = state.count + 10;
    }
    readPhotocell(2);
    setColor(state.A0_Value,(state.A0_Value + state.count) % 255,(state.A0_Value + (state.count+10)) % 255);
    delay(30);
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

