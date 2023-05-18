/*DEFINICION DISTINTAS CINEMATICAS:
 * La cinemática directa, la cinemática inversa y la cinemática diferencial son conceptos relacionados con el análisis del 
 * movimiento en robótica. Aquí te explico brevemente la diferencia entre ellos:

1. Cinemática directa: La cinemática directa se refiere al cálculo de la posición y orientación 
final de un robot en función de las posiciones y velocidades de sus articulaciones. En otras palabras,
dado un conjunto de ángulos o posiciones de las articulaciones, la cinemática directa nos permite determinar
la posición y orientación del extremo del robot, como su posición (coordenadas x, y, z) y su 
orientación (ángulos de Euler o cuaterniones).

2. Cinemática inversa: La cinemática inversa es el proceso opuesto a la cinemática directa. En lugar
de determinar la posición final del robot a partir de las articulaciones, la cinemática inversa calcula
los ángulos o posiciones de las articulaciones necesarios para alcanzar una posición y orientación deseada 
del extremo del robot. En resumen, dado un objetivo de posición y orientación, la cinemática inversa nos permite 
determinar los ángulos o posiciones de las articulaciones necesarios para alcanzar ese objetivo.

3. Cinemática diferencial: La cinemática diferencial se utiliza en robots móviles y se refiere al cálculo de las
velocidades de las ruedas o actuadores necesarias para lograr una velocidad y dirección deseada del robot. En lugar 
de trabajar con ángulos de articulaciones, la cinemática diferencial se centra en las velocidades lineales y angulares 
del robot. Esto es particularmente útil en robots móviles con ruedas diferenciales, donde se deben controlar las velocidades 
de las ruedas izquierda y derecha para lograr una trayectoria deseada.

En resumen, la cinemática directa se ocupa de calcular la posición final del robot a partir de las articulaciones,
la cinemática inversa determina las posiciones de las articulaciones necesarias para alcanzar una posición deseada, 
y la cinemática diferencial calcula las velocidades de los actuadores para lograr una velocidad y dirección deseadas
del robot. Cada uno de estos conceptos es útil en diferentes contextos y aplicaciones de la robótica.*/


#include <Servo.h>

// Definir los pines de los servomotores
const int servoPin1 = 2;
const int servoPin2 = 3;
const int servoPin3 = 4;
const int servoPin4 = 5;

// Crear objetos Servo para controlar los servomotores
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// Clase para la realimentación y corrección del error de posición
class FeedbackControl {
private:
  int servoPos;
  int desiredPos;
  int error;
  int kP;

public:
  FeedbackControl(int kPValue) {
    servoPos = 90; // Posición inicial del servo
    desiredPos = 90; // Posición deseada del servo
    error = 0; // Inicializar el error a cero
    kP = kPValue; // Constante proporcional del controlador
  }

  void setDesiredPosition(int pos) {
    desiredPos = pos;
  }

  void update() {
    // Calcular el error de posición
    error = desiredPos - servoPos;

    // Aplicar la corrección proporcional
    int correction = kP * error;

    // Actualizar la posición del servo
    servoPos += correction;
  }

  int getServoPosition() {
    return servoPos;
  }
};

// Clase para el cálculo de la cinemática inversa
class InverseKinematics {
public:
  static void calculateAngles(float x, float y, float z, int &angle1, int &angle2, int &angle3, int &angle4) {
    // Realizar cálculos de la cinemática inversa para obtener los ángulos de los servos

    // Implementar el cálculo de los ángulos aquí
    // ...

    // Asignar los valores calculados a las variables de ángulo
    angle1 = 90;
    angle2 = 90;
    angle3 = 90;
    angle4 = 90;
  }
};

// Función para mover el brazo a una posición específica
void moveArm(int servo1Pos, int servo2Pos, int servo3Pos, int servo4Pos) {
  servo1.write(servo1Pos);
  servo2.write(servo2Pos);
  servo3.write(servo3Pos);
  servo4.write(servo4Pos);
  delay(500); // Pausa para dar tiempo a que se mueva el servo
}

void setup() {
  // Inicializar los servomotores
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);
  servo4.attach(servoPin4);
}

void loop() {
  // Definir una matriz de posiciones (x, y, z) a las que se quiere ir
  float positions[][3] = {
    {10.0, 0.0, 10.0},   // Punto 1
    {0.0, 10.0, 10.0},   // Punto 2
    {0.0, 0.0, 10.0},    // Punto 3
    {10.0, 10.0, 10.0}   // Punto 4
  };
  
  int numPositions = sizeof(positions) / sizeof(positions[0]);

  for (int i = 0; i < numPositions; i++) {
    float x = positions[i][0];
    float y = positions[i][1];
    float z = positions[i][2];
    int angle1, angle2, angle3, angle4;
    InverseKinematics::calculateAngles(x, y, z, angle1, angle2, angle3, angle4);
    moveArm(angle1, angle2, angle3, angle4);
    delay(2000);
  }
}
