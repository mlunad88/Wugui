#include <Servo.h>

Servo myservo;  // crea el objeto servo

int pos = 0;    // posicion del servo
int realiment=0;
int r_mayor=0;
int r_menor=300;
double k= (180.0/270.0);

void setup() {
  Serial.begin(9600);
  myservo.attach(7);  // vincula el servo al pin digital 9
  myservo.write(190);//VALOR ENTRE 500 Y 2000
}

void loop() {
  // put your main code here, to run repeatedly:
  int realiment=analogRead(A0);
  if(realiment>=r_mayor){
      r_mayor=realiment;
  }
  if (realiment<=r_menor){
    r_menor=realiment;
  }

  Serial.println(k);
  Serial.println("mayor: ");
  Serial.println(r_mayor);
  Serial.println("menor: ");
  Serial.println(r_menor);
  Serial.println("Actual: ");
  Serial.println(realiment);
  delay(1000);
  
}
