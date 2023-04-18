#include <Servo.h>

// Macros para los pines de los servos
#define S1 5
#define S2 9
#define S3 10
#define S4 11

// Macros para los pines analógicos de los potes
#define P1 A1
#define P2 A2
#define P3 A3
#define P4 A4


// Creación de los objetos servo en el código
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// Declaración de las variables que leerán los valores de los potenciómetros.
int pot1;
int pot2;
int pot3;
int pot4;

void programa1(void);
void programa2(void);
void manual(void);
void mover(Servo);

void setup() {
  pinMode(P1,INPUT);
  pinMode(P2,INPUT);
  pinMode(P3,INPUT);
  pinMode(P4,INPUT);
  
  // Para leer la posición de los servos y los potes en pantalla
  Serial.begin(9600);

  // Unimos los servos al pin que les corresponde
  servo1.attach(S1);
  servo2.attach(S2);
  servo3.attach(S3);
  servo4.attach(S4);

}

void loop() {
  int modo = 2;
  switch(modo){
    case 1: manual();
    case 2: programa1();
    default: Serial.println("Waiting...");
  }
}
void programa1(){
  
  int min_pos = 0; //METER AQUÍ LA POSICIÓN MÍNIMA
  int max_pos = 0; //METER AQUÍ LA POSICIÓN MÁXIMA
  
  //Mover lentamente el servo de la posicion minima
  //a la posicion maxima
  
  for(int i = min_pos; i < max_pos; i++){
    servo2 io0.write(i);
    servo3.write(2250 - i);
    delay(1);
  }
  delay(1000);
  for(int i = max_pos; i > min_pos; i--){
    servo2.write(i);
    servo3.write(2250-i);
    delay(1);
  }
}
void manual(){
  // Declaración de las variables que darán la posición a los servos
  int ser1;
  int ser2;
  int ser3;
  int ser4;
  
  // Leemos el valor de los 4 potenciómetros
  pot1 = analogRead(P1);
  pot2 = analogRead(P2);
  pot3 = analogRead(P3);
  pot4 = analogRead(P4);

  // Convertimos el valor de los potenciómetros a un ángulo del servo
  ser1 = map(pot1, 0, 1024, 0, 180);
  ser2 = map(pot2, 0, 1024, 0, 180);
  ser3 = map(pot3, 0, 1024, 0, 180);
  ser4 = map(pot4, 0, 1024, 0, 180);

  // Llevamos el servo a la posición marcada
  servo1.write(ser1);
  servo2.write(ser2);
  servo3.write(ser3);
  servo4.write(ser4);

  //Le damos un pequeño tiempo para que el servo alcance la posición
  
}
void programa2(){
  
  // Declaración de las variables que darán la posición a los servos
  
  //PONER AQUÍ LA POSICIÓN FINAL DE LOS SERVOS
  int ser1 = 0;
  int ser2 = 0;
  int ser3 = 0;
  int ser4 = 0;
  
  // Llevamos el servo a la posición marcada
  servo1.write(ser1);
  servo2.write(ser2);
  servo3.write(ser3);
  servo4.write(ser4);

  // Convertimos el valor de los potenciómetros a un ángulo del servo
  ser1 = map(pot1, 0, 1024, 0, 180);
  ser2 = map(pot2, 0, 1024, 0, 180);
  ser3 = map(pot3, 0, 1024, 0, 180);
  ser4 = map(pot4, 0, 1024, 0, 180);

  //Poner aquí la minimización del error
  
}
