/*

  Økt 8: glatting av sensorverdier

*/
const int size = 10;

int readings[size];
int rIndex = 0;
int total = 0;
int average = 0;

int current = 0;
const int timer = 100;

struct Pin {
  const int photocell = A0;
};

Pin pin;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < size; i++) {
    readings[i] = 0;
  }
}

void loop() {
  total -= readings[rIndex];
  readings[rIndex] = analogRead(pin.photocell);
  total += readings[rIndex];
  rIndex++;

  if (rIndex >= size) {
    rIndex = 0;
  }

  average = total / size;

  if (millis() - current >= timer) {
    current = millis();

    Serial.print("Med glatting: ");

    Serial.print(average);

    Serial.print(", Uten glatting: ");

    Serial.print(analogRead(pin.photocell));

    Serial.println("");
  }

  delay(1);
}