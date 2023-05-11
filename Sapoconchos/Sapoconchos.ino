#include <Servo.h>

// Macros para los pines de los servos
#define S1 5

// Macros para los pines analógicos de los potes
#define P1 A0

// Creación de los objetos servo en el código
Servo servo1;

void manual(void);
float calcula_pos(float err, float lectura_pos, float pos_env);
void realimentacion(float lectura_pos, float pos_env);
float calcula_error(float err, float pos_env);

void setup() {
  pinMode(P1,INPUT);
  
  // Para leer la posición de los servos y los potes en pantalla
  Serial.begin(9600);

  // Unimos los servos al pin que les corresponde
  servo1.attach(S1);

}

void loop() {
  realimentacion(analogRead(P1), 45);
}

void manual(float pos_env){
  servo1.write(pos_env);
  Serial.println(analogRead(P1));
  delay(1000);
}
void realimentacion(float lectura_pos, float pos_env){
  float err = lectura_pos - pos_env;
  int k = 0;
  float err_rel = calcula_error(err, pos_env);
  while (err_rel < 0.01 || k <10){
    pos_env = calcula_pos(err,lectura_pos, pos_env);
    manual(pos_env);
    err_rel = calcula_error(err, pos_env);
    delay(50);
    k++;
  }
}
float calcula_pos(float err, float lectura_pos, float pos_env){
  if (lectura_pos != pos_env){
    pos_env = lectura_pos - err;
  };
  return pos_env;
}
float calcula_error(float err, float pos_env){
  float err_rel = abs(err)/pos_env;
  return err_rel;
}
