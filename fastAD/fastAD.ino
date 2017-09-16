void setup() {
  Serial.begin(115200); // 14400 B/s = max ~7Khz !!!
}

void loop() {
  delay(1);
  Serial.print(analogRead(A0));
  Serial.print('\n');
}
