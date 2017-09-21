
int sharp = A0;
int lm35 = A2;

int led = 13;

void setup() {
  pinMode(led, OUTPUT);
  
  Serial.begin(9600); 
  
}

void loop() {
  int dist = analogRead(sharp);
  int mili = map(dist, 550, 171, 100, 500); 
  delay(1);
  int temp = analogRead(lm35);
  int mg = map(temp, 0, 1024, -55000, 150000);
  float temp2 = mg / 1000;

  Serial.print(temp2);
  Serial.print(" C : ");
  Serial.print(mili);
  Serial.println(" mm");
  
  delay(20);
}








