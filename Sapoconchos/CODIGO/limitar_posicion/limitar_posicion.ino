//Pone la situacion inicial del servo en el centro del su posible recorrido
//Imprime por pantalla su realimentacion
//reduce el angulo de giro de 270 a 250


#include <Servo.h>

Servo myservo;  // Crear objeto servo
int zero_position = 135; // Posición del servo correspondiente a 0 grados
int servo_range = 125;  // Rango del servo entre 2, (puede ser 250 o 180 grados)
double k= (180.0/270.0); //para los servos de 270 grados, en el de 180 k=1 (osea que no hay xd)

int realiment1 = 0; //realimentacion del servo1


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
  Serial.begin(9600);

  myservo.attach(9);  // Configurar pin de salida
  myservo.write(zero_position); // Establecer la posición inicial del servo
}

void loop() {
  int angle = 0;  // Ejemplo de ángulo negativo
  angle = convert_angle(angle); // Convertir ángulo a grados positivos en relación a la posición de 0 grados
  angle = clip(angle); // Obtener ángulo recortado al rango del servo
  myservo.write(k*angle);  // Mover a posición recortada
  delay(1000);           // Esperar 1 segundo

  realiment1 =analogRead(A0);
  Serial.println(realiment1);
  delay(1000);
}
