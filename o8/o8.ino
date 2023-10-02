/*

  Økt 8: glatting av sensorverdier

*/
int* ptr;
int i;
int a_size = 100;

int* initArray() {
  static int a[50];
  for (int i = 0; i < 50; i++)
  {
    a[i] = i;
  }
  return a;
}


void setup() {
  ptr = initArray();
  Serial.begin(9600);
}

void loop() {
  if (i == 50) {
    i = 0;
  }
  Serial.println(ptr[i]);
  i++;
  delay(200);
}