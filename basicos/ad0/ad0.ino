void setup() {
  // put your setup code here, to run once:
pinMode(A1, INPUT);
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int valor = analogRead(A1);
  Serial.println(valor);
}
