#include <Servo.h>

Servo servo1, servo2;  // crea los objetos Servo

  //Para servo de 270: k*ángulo real
  //Para servo de 180: ángulo real
  //Si en el de 180 decimos un ángulo mayor, se queda en su tope de 180 y listo
double k= (180.0/270.0); //para los servos de 270 grados, en el de 180 k=1 (osea que no hay xd)

  //Inicializa al valor medio del ángulo de rango
int pos1 = 90;    // posicion del servo1
int pos2 = k*135;    // posicion del servo1

int realiment1 = 0; //realimentacion del servo1
int realiment2 = 0; //realimentacion del servo1

void setup() {
  Serial.begin(9600);
  
  servo1.attach(9);  // servo de 180 al pin digital 9
  servo2.attach(7);  // servo de 270 al pin digital 7

  //Inicializa la posición de los servos
  servo1.write(pos1);
  servo2.write(pos2);
}

void loop() {

  realiment1 =analogRead(A0);
  realiment2 =analogRead(A1);


  Serial.println(realiment1);
  Serial.println(realiment2);
  delay(1000);
  
}
