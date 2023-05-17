#include <Servo.h>
#include "realimentacion.h"

// Macros para los pines de los servos
#define S1 5

// Macros para los pines analógicos de los potes
#define P1 A0

// Creación de los objetos servo en el código
Servo servo1;
realimentacion realimentacion;

int zero_position = 135; // Posición del servo correspondiente a 0 grados
int servo_range = 125;  // Rango del servo entre 2, (puede ser 250 o 180 grados)
double k= (180.0/270.0); //para los servos de 270 grados, en el de 180 k=1 (osea que no hay xd)

int realiment1 = 0; //realimentacion del servo1

void manual(void);
void real(void);
// Función para convertir ángulos negativos en grados positivos en relación a la posición de 0 grados del servo
int convert_angle(int angle) {
  return angle + zero_position;
}
// Función para recortar los ángulos de giro al rango del servo
int clip(int angle) {
  if (angle < -servo_range + zero_position) {
    return -servo_range + zero_position;
  } else if (angle > servo_range + zero_position) {
    return servo_range + zero_position;
  } else {
    return angle;
  }
}

void setup() {
  pinMode(P1,INPUT);
  
  // Para leer la posición de los servos y los potes en pantalla
  Serial.begin(9600);

  // Unimos los servos al pin que les corresponde
  servo1.attach(S1);
  myservo.write(zero_position); // Establecer la posición inicial del servo
}

void loop() {
  int angle = 0;  // Ejemplo de ángulo
  angle = convert_angle(angle); // Convertir ángulo a grados positivos en relación a la posición de 0 grados
  angle = clip(angle); // Obtener ángulo recortado al rango del servo
  realimentacion.lectura_pos = analogRead(P1);
  realimentacion.pos_env = k*angle;
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
