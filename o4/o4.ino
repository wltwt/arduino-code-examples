// Økt 3

/*
  Undersøke registrering av knappetrykk.
*/

const int ledPin = 11;

struct Button {
  int pin;
  int lastState;
  int state;
  int count;
};

Button button = {7, HIGH};

void setup() {
  Serial.begin(9600);
  pinMode(button.pin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  stateChange();
  controlLED();
}

void stateChange() {
  button.state = digitalRead(button.pin);

  if (button.state != button.lastState) {
    if(button.state == LOW) {
        button.count++;

        Serial.println("ON");
        Serial.print("Button pressed: ");
        Serial.print(button.count);
        Serial.println(" times.");
    } else {
        Serial.println("off");
    }
    delay(130); // måtte høy verdig til for å få det til å funke ok
  }
}

void controlLED() {
  if (button.count % 4 == 0) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}