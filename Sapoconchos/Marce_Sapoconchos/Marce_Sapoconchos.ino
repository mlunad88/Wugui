#include <Servo.h>
#include "realimentacion.h"

// Macros para los pines de los servos
#define S1 5

// Macros para los pines analógicos de los potes
#define P1 A0

// Creación de los objetos servo en el código
Servo servo1;
realimentacion realimentacion;

void manual(void);
void real(void);

void setup() {
  pinMode(P1,INPUT);
  
  // Para leer la posición de los servos y los potes en pantalla
  Serial.begin(9600);

  // Unimos los servos al pin que les corresponde
  servo1.attach(S1);
}

void loop() {
  realimentacion.lectura_pos = analogRead(P1);
  realimentacion.pos_env = 45;
  real();
}

void manual(){
  servo1.write(realimentacion.pos_env);
  Serial.println(analogRead(P1));
  delay(1000);
}
void real(){
  realimentacion.err = realimentacion.lectura_pos - realimentacion.pos_env;
  realimentacion.k = 0;
  realimentacion.err_rel = realimentacion.calcula_error();
  while (realimentacion.err_rel < 0.01 || realimentacion.k <10){
    realimentacion.pos_env = realimentacion.calculapos();
    manual();
    realimentacion.err_rel = realimentacion.calcula_error();
    delay(50);
    realimentacion.k++;
  }
}
