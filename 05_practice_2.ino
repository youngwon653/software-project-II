#define PIN_LED 7 // Active Low

void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, 1); // If this line of code is absent, the default state is 0
}

void loop() {
  digitalWrite(PIN_LED, 0); // ON
  delay(1000);

  for(int i = 0; i < 5; i++)
  {
    digitalWrite(PIN_LED, 1);
    delay(100);
    digitalWrite(PIN_LED, 0);
    delay(100);
  }
  
  digitalWrite(PIN_LED, 1);
  while(1) {} // infinite loop
}
