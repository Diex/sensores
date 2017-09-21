int sensor = 6;

boolean estado = false;
boolean cambio = false;
boolean ledon = false;

void setup() {
  Serial.begin(9600);
  pinMode(sensor, INPUT);
}

void loop() {
  
  estado = digitalRead(sensor);

  if(estado == LOW){
    cambio = true;
  }

  if(cambio == true){
    ledon = !ledon;
    cambio = false;
  }

  if(ledon == true){
    digitalWrite(13, HIGH);
  }else{
    digitalWrite(13, LOW);
  }
  
}
