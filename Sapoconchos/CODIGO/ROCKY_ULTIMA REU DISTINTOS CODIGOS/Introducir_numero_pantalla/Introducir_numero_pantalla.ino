
void setup() {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()>0)
  {
    String str=Serial.readStringUntil('\n');
    float pos_deseada=str.toFloat();
    Serial.println(pos_deseada);
  }
}
