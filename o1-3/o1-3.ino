// økt 1.3

const int l_pin = 11;

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

Usertime t, photocell;
State state;

void updatePhotocell(int pin) {
  state.A0_Value = map(analogRead(pin), 0, 1023, 0, 255);
}

/*
  Map funksjonen tar inn fire verdier og gjør ett sett av verdier om til et annet sett av verdier.
*/

void updatePhotocellExperiment_1(int pin) {
  state.A0_Value = map(analogRead(pin), 0, state.count % 1023, 0, state.count % 255);
}

/*
  Hvis man mater inn en verdi rundt 200 og bruker map(..., 0, 1023, 0, 400)
  vil man ca. få ut 80 så det funksjonen gjør er at den tar inn en verdi,
  så definerer man området til verdien og deretter gir den tilsvarende verdi
  i et annet intervall man definerer.

  Eksempel:

  Den reelle verdien man kan få er ~0-1000, så man setter input argumentet til det,
  hvis man vil ha ut halveis-verdien til å bli 5 setter man de to siste argumentene til
  0, 10.

  map(analogRead(pin), 0, 10, 0, 10);

  er ekvivalent med:
  map(analogRead(pin), 0, 1023, 0, 1023);
  og
  map(analogRead(pin), 1023, 0, 1023, 0);
  og
  map(analogRead(pin), -1023, 0, -1023, 0);
  
  ---
  
  map(analogRead(pin), -1023, 0, 1023, 0);
  ->negativ ekvivalent verdi
  
  ---

  map(analogRead(pin), -1023, 0, 0, 1023);
  ->inngansverdi + 1023 + utgangsverdi

  map(analogRead(pin), 0, -1023, 1023, 0);
  ->inngangsverdi + 1023

  map(analogRead(pin), 1023, 0, 0, 1023);
  ->1023 - inngansverdi

  ---

  fant ut en kalibrering for lyset i rommet slik at jo mindre lys som treffer photocellen jo sterkere lyser LED'en:
  map(analogRead(pin), 200, 0, 0, 255);
  her er altså sterkeste lyset photocellen leste i rommet 200, og dermed ble resten av verdiene skalert til LED'en
  sin input-verdi. Dette ble gjort ved hjelp av arduinoen sin evne til å håndtere PWM.  
*/

void updatePhotocellExperiment_2(int pin) {
  state.A0_Value = map(analogRead(pin), 200, 0, 0, 255);
}

void ledChange() {
  digitalWrite(l_pin, state.led);
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
    state.count++;
    // updatePhotocell(state.photocellPin);
    // updatePhotocellExperiment_1(state.photocellPin);
    updatePhotocellExperiment_2(state.photocellPin);
    Serial.println(state.A0_Value);
  }
}

void outputLED() {
  analogWrite(l_pin, state.A0_Value);
}

void setup() {
  pinMode(l_pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  readPhotocell(2);
  outputLED();
}
