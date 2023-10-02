// Økt 3

const int ledPin = 11;

struct Button {
  int pin;
  int lastState;
  unsigned long lastDebounceTime;
  unsigned long debounceDelay;
  int toggle;
  int state;
};

struct Time {
  unsigned long current;
  unsigned long previous;
};

struct Printer {
  int readingValueFirst;
  int readingValueLast;
  int buttonToggled;
};

Button button = {7, HIGH, 0, 1000, HIGH};
Time time;
Printer p;

void setup() {
  Serial.begin(9600);
  pinMode(button.pin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  /*int sensorValue = digitalRead(button.pin);
  Serial.println(sensorValue);
  if (sensorValue == HIGH) {
    digitalWrite(ledPin, LOW);
  } else {
    digitalWrite(ledPin, HIGH);
  }
  */
  debounceButton();
  printer(1000);
  digitalWrite(ledPin, button.toggle);
}

void debounceButton() {
  int reading = digitalRead(button.pin);
  p.readingValueFirst = reading;

  if (reading != button.lastState) {          // knapp trykket inn: kjør kode, knapp opp: kjør kode, ikke mellom
    if (reading != LOW && button.lastDebounceTime > button.debounceDelay) {
      Serial.print("Button held for: ");
      int holdTime = millis() - button.lastDebounceTime;
      Serial.print(holdTime);
      Serial.println("\n");
    }
    button.lastDebounceTime = millis();       // 100 ms
  }
  
  if ((millis() - button.lastDebounceTime) >= button.debounceDelay) {    // knappen må være holdt inne i 50 ms for at den skal gå her
    if (reading != button.state) {
      button.state = reading;
      if(button.state == LOW) {
        button.toggle = !button.toggle;
      }
    }
  }
  button.lastState = reading;
  p.readingValueLast = button.lastState;
}

void printer(int timer) {
  time.current = millis();
  if (time.current - time.previous >= timer) {
    time.previous = millis();
    Serial.print("readingFirst: ");
    Serial.print(p.readingValueFirst);
    Serial.print(", readingLast: ");
    Serial.print(p.readingValueLast);
    Serial.print(", lastDebounce: ");
    Serial.print(button.lastDebounceTime);
    Serial.print(", Toggle: ");
    Serial.print(button.toggle);
    Serial.print(", buttonState: ");
    Serial.print(button.state);
    Serial.print(", buttonLastState: ");
    Serial.print(button.lastState);
    Serial.println("\n");
  }
}