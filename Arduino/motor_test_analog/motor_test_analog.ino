#define MOTOR1F 5
#define MOTOR1R 6
#define MOTOR2F 10
#define MOTOR2R 11

void setup() {
  // put your setup code here, to run once:
  DDRB |= 0b00001100;
  DDRD |= 0b01100000;

  //set interrupts for MOTOR encoder
  cli();
  EICRA |= 0b;

  sei();
}

void forward() {
  digitalWrite(5, HIGH);
  digitalWrite(10, HIGH);
}

void backward() {
  digitalWrite(6, HIGH);
  digitalWrite(11, HIGH);
}

void loop() {
  backward();
}
