#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// Longitudes de los eslabones del brazo robótico
float l1 = 10.0;
float l2 = 10.0;
float l3 = 10.0;
float l4 = 10.0;

// Angulos máximos y mínimos para cada servo
int servo1MinAngle = 0;
int servo1MaxAngle = 180;
int servo2MinAngle = 0;
int servo2MaxAngle = 180;
int servo3MinAngle = 0;
int servo3MaxAngle = 180;
int servo4MinAngle = 0;
int servo4MaxAngle = 180;

void setup() {
  // put your setup code here, to run once:
 servo1.attach(pin_servo1);
  servo2.attach(pin_servo2);
  servo3.attach(pin_servo3);
  servo4.attach(pin_servo4);
}

void loop() {
   // Leer la posición deseada desde el Monitor Serie
  float x, y, z;
  Serial.println("Ingrese las coordenadas (x, y, z) deseadas:");
  Serial.print("x: ");
  while (!Serial.available());
  x = Serial.parseFloat();
  Serial.print("y: ");
  while (!Serial.available());
  y = Serial.parseFloat();
  Serial.print("z: ");
  while (!Serial.available());
  z = Serial.parseFloat();

  // Calcular los ángulos de las articulaciones utilizando la cinemática directa
  float r = sqrt(x*x + y*y);  // Distancia proyectada en el plano xy
  float d = sqrt(r*r + z*z);  // Distancia real al punto
  float theta1 = atan2(y, x) * 180 / PI;  // Ángulo de la articulación 1
  float theta2 = acos((l2*l2 - l3*l3 + d*d) / (2 * l2 * d)) * 180 / PI;  // Ángulo de la articulación 2
  float theta3 = acos((l2*l2 + l3*l3 - d*d) / (2 * l2 * l3)) * 180 / PI;  // Ángulo de la articulación 3
  float theta4 = atan2(z, r) * 180 / PI;  // Ángulo de la articulación 4

  // Mapear los ángulos a los rangos de los servos
  int servo1Angle = map(theta1, -180, 180, servo1MinAngle, servo1MaxAngle);
  int servo2Angle = map(theta2, 0, 180, servo2MinAngle, servo2MaxAngle);
  int servo3Angle = map(theta3, 0, 180, servo3MinAngle, servo3MaxAngle);
  int servo4Angle = map(theta4, -180, 180, servo4MinAngle, servo4MaxAngle);

  // Mover los servos a las posiciones correspondientes
  servo1.write(servo1Angle);
  servo2.write(servo2Angle);
  servo3.write(servo3Angle);
  servo4.write(servo4Angle);

}
