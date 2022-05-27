int led_light = 4;

void setup() {
  // put your setup code here, to run once:
  pinMode(led_light, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(led_light, HIGH);
  delay(1000);
  digitalWrite(led_light, LOW);
  delay(1000);
}