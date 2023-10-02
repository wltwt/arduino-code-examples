// økt 1.2

const int l_pin = LED_BUILTIN;
struct Usertime {
  unsigned long now;
  unsigned long earlier;
};

struct State {
  int led = LOW; 
};

Usertime t;
State state;

void setup() {
  t.now = millis();
  t.earlier = 100;
  pinMode(l_pin, OUTPUT);
}

void switchState(int timer) {
  t.now = millis();
  if (t.now - t.earlier >= timer) {
    t.earlier=t.now;
    if (state.led) {
      state.led = LOW;
    } else {
      state.led = HIGH;
    }
    ledChange();
  } 
}

void ledChange() {
  digitalWrite(l_pin, state.led);
}

void loop() {
  switchState(500);
}
