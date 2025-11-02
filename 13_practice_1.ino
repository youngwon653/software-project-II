#include <Servo.h>

// Arduino pin assignment
#define PIN_SERVO 10

// configurable parameters for Servo
#define _DUTY_MIN 550 // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1475 // servo neutral position (90 degree)
#define _DUTY_MAX 2400 // servo full counter-clockwise position (180 degree)

#define _SERVO_SPEED 0.3 // servo angular speed (unit: degree/sec)

// Loop Interval
#define _INTERVAL_SERVO   20 // servo interval (unit: msec)
#define _INTERVAL_SERIAL  100 // serial interval (unit: msec)

// global variables
unsigned long last_sampling_time_servo;  // unit: msec
unsigned long last_sampling_time_serial; // unit: msec
bool event_servo, event_serial;

Servo myservo;

float duty_change_per_interval; // maximum duty difference per interval
int duty_target;    // Target duty time
float duty_curr;      // Current duty time

int toggle_interval, toggle_interval_cnt;

void setup() {
  // initialize GPIO pins
  myservo.attach(PIN_SERVO); 
  
  duty_target = duty_curr = _DUTY_MIN;
  myservo.writeMicroseconds(duty_curr);
  
  // initialize serial port
  Serial.begin(57600);  // <----- baud rate

  // convert angular velocity into duty change per interval.
  duty_change_per_interval = 
    (float)(_DUTY_MAX - _DUTY_MIN) * (_SERVO_SPEED / 180.0) * (_INTERVAL_SERVO / 1000.0);
    
  // initialize variables for servo update.
  toggle_interval = (180.0 / _SERVO_SPEED) * 1000 / _INTERVAL_SERVO;
  toggle_interval_cnt = toggle_interval;
  
  // initialize last sampling time
  last_sampling_time_servo = 0;
  last_sampling_time_serial = 0;
}

void loop() {
  // wait until next event time. 
  unsigned long time_curr = millis();
  if (time_curr >= (last_sampling_time_servo + _INTERVAL_SERVO)) {
    last_sampling_time_servo += _INTERVAL_SERVO;
    event_servo = true;
  }
  if (time_curr >= (last_sampling_time_serial + _INTERVAL_SERIAL)) {
    last_sampling_time_serial += _INTERVAL_SERIAL;
    event_serial = true;
  }

  if (event_servo) {
    event_servo = false;
    // adjust duty_curr toward duty_target by duty_change_per_interval
    if (duty_target > duty_curr) {
      duty_curr += duty_change_per_interval;
      if (duty_curr > duty_target)
          duty_curr = duty_target;
    } else {
      duty_curr -= duty_change_per_interval;
      if (duty_curr < duty_target)
        duty_curr = duty_target;
    }
    // update servo position
    myservo.writeMicroseconds((int)duty_curr);
    // toggle duty_target between _DUTY_MIN and _DUTY_MAX.
    if (toggle_interval_cnt >= toggle_interval) {
      toggle_interval_cnt = 0;
      if (duty_target == _DUTY_MIN)
        duty_target = _DUTY_MAX;
      else
        duty_target = _DUTY_MIN;
    } else {
      toggle_interval_cnt++;
    }
  }
  if (event_serial) {
    event_serial = false;
    Serial.print("Min:550");
    Serial.print(",duty_target:"); Serial.print(duty_target);
    Serial.print(",duty_curr:");   Serial.print(duty_curr);
    //Serial.print(",duty_change_per_interval:");   Serial.print(duty_change_per_interval);
    Serial.println(",Max:2400");
  }
  delay(5);
}
