#include <Servo.h>

// Macros para los pines de los servos
#define S1 5

// Macros para los pines analógicos de los potes
#define P1 A0

// Creación de los objetos servo en el código
Servo servo1;

void manual(void);


void setup() {
  pinMode(P1,INPUT);
  
  // Para leer la posición de los servos y los potes en pantalla
  Serial.begin(9600);

  // Unimos los servos al pin que les corresponde
  servo1.attach(S1);

}

void loop() {
  manual();
}

void manual(void){
  servo1.write(60);
  Serial.println(analogRead(P1));
  delay(1000);
  servo1.write(75);
  Serial.println(analogRead(P1));
  delay(1000);
  servo1.write(90);
  Serial.println(analogRead(P1));
  delay(1000);
  //servo1.write(0);
  //Serial.println(analogRead(P1));
  //delay(1000);
}
