int left_motor = 9;
int right_motor = 10;

void setup() {
  // put your setup code here, to run once:
  pinMode(left_motor, OUTPUT);
  pinMode(right_motor, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(left_motor, 50);
  analogWrite(right_motor, 50);
  delay(1000);
}
