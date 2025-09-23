#define PIN_LED 7 // LED 핀 지정
#define ON 0 // Active Low -> 0 == ON
#define OFF 1

long period_; // unit: microsecond

void setup() {
  pinMode(PIN_LED, OUTPUT);
  set_period(1000); // 깜빡이는 주기 설정
  set_duty(0); // unit: %
}

void loop() {
  for(long t = 1; t <= (long)500000; t += period_) // 최소밝기 -> 최대밝기
    set_duty(t / 5000);
  for(long t = (long)500000; t > 0; t -= period_) // 최대밝기 -> 최소밝기
    set_duty(t / 5000);
}

void set_period(int period) { // unit: microsecond
  if(period < 100) // 범위 제한
    period = 100;
  else if(period > 10000)
    period = 10000;
  period_ = period;
}
void set_duty(int duty) { // unit: %
  if(duty < 0) // 범위 제한
    duty = 0;
  else if(duty > 100)
    duty = 100;
    
  digitalWrite(PIN_LED, ON);
  delayMicroseconds(period_ * duty / 100);
  digitalWrite(PIN_LED, OFF); // 주기 동안 LED가 켜져있는 시간
  delayMicroseconds(period_ * (100 - duty) / 100); // 주기 동안 LED가 꺼져있는 시간
}
