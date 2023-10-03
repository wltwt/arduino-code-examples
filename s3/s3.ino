/*
  Selvstudiumsoppgave 3
  1 og 2

  Kontinuerlig overvåking av sensorverdi med glatting,
  der verdiene blir brukt til å utføre operasjoner på komponenter.
*/

const int size = 10;
int read[size];

struct Pin {
  const int photocell = A0;
  const int rLED = 11;
};

struct State {
  int state;
  int prevState = HIGH;
};

Pin pin;
State redLED;

int total = 0;
int average = 0;

unsigned long prevTime = 0;
unsigned long printerTimer = 0;
unsigned long readPrev = 0;

static int i = 0;

void readAverageSensor(int *read) {
  if (millis() - readPrev >= 2){
    readPrev = millis();
    total -= read[i];
    read[i] = analogRead(pin.photocell);
    total += read[i];
    increment();
    average = total / size;
  }
}

void increment() {
  if (i >= size - 1) {
    i = 0;
  } else {
    i++;
  }
}

void blinkFrequency() {
  if (millis() - prevTime >= average) {
    prevTime = millis();
    redLED.state = !redLED.state; // toggle state
  }
  digitalWrite(pin.rLED, redLED.state);
}

void print(int timer) {
  if (millis() - printerTimer >= timer) {
    printerTimer = millis();
    Serial.println(average);
  }
}

void setup() {
  pinMode(pin.rLED, OUTPUT);
  Serial.begin(9600);

  for (int i = 0; i < size; i++) {
    read[i] = 0;
  }
}

void loop() {
  readAverageSensor(read);
  blinkFrequency();
  print(100);
}