
int entrada = A0;
int led = 13;

void setup() {
  pinMode(led, OUTPUT);
  
  Serial.begin(9600); 
  
}

void loop() {
  int valor = analogRead(entrada);
  int mili = map(valor, 550, 171, 100, 500); 
  Serial.print(mili);
  Serial.println(" mm");
  
  delay(10);
}








