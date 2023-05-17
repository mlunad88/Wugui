#include <Servo.h>
#include "realimentacion.h"

// Macros para los pines de los servos
#define S1 5

// Macros para los pines analógicos de los potes
#define P1 A0

//Dimensión del vector de la media de las lecturas
#define DIM 10

// Creación de los objetos servo en el código
Servo servo1;
realimentacion eslabon;

int zero_position = 135; // Posición del servo correspondiente a 0 grados
int servo_range = 125;  // Rango del servo entre 2, (puede ser 250 o 180 grados)
double k= (180.0/270.0); //para los servos de 270 grados, en el de 180 k=1 (osea que no hay xd)

//Inicializa al valor medio del ángulo de rango
int pos1 = 180;    // posicion del servo1
int pos2 = k*180;    // posicion del servo1?????????????????????????

float conv_ang1, conv_ang2, offset=59.6, pendiente=2.765; //supuestamente 3
double offset2=66.5, pendiente2=3.4;
//HAY QUE AJUSTAR VALORES DE OFFSET Y PENDIENTE!!!!!

void manual(void);
void real(void);
int convert_and_clip(int);
float media(int, int); // media(LECTURA, NUMERO SERVO)

void setup() {
  pinMode(P1,INPUT);
  
  // Para leer la posición de los servos y los potes en pantalla
  Serial.begin(9600);

  // Unimos los servos al pin que les corresponde
  servo1.attach(S1);
  servo1.write(zero_position); // Establecer la posición inicial del servo
}

void loop() {
  int angle = 90;  // Ejemplo de ángulo
  
  angle = convert_and_clip(angle);
  
  eslabon.pos_env = k*angle; //Enviamos al servo
  /*LECTURAS*/
  Serial.println("CALCULOS");
  Serial.println("--------------");
  
  eslabon.lectura_pos = media(analogRead(P1), 1);

  real();
}

void manual(){
  servo1.write(eslabon.pos_env);
  Serial.println(analogRead(P1));
  delay(1000);
}
void real(){
  eslabon.err = eslabon.lectura_pos - eslabon.pos_env;
  eslabon.k = 0;
  eslabon.err_rel = eslabon.calcula_error();
  while (eslabon.err_rel < 0.01 || eslabon.k <10){
    eslabon.pos_env = eslabon.calculapos();
    manual();
    eslabon.err_rel = eslabon.calcula_error();
    delay(50);
    eslabon.k++;
  }
}
float media(int lectura, int num_servo){
  int v[DIM];
  int suma=0;
  float media = 0;
    for (int i = 0; i<DIM; i++){
      if (i<DIM){
        v[i]=lectura;
        suma+=lectura;
      }
      if (i==DIM){
        /*for(j=0;j<5;j++){
       Serial.println(v[j]);
       Serial.println("///");*/
       media=(float)(suma/DIM);
       float pos_real=(media-offset)/pendiente;
       Serial.println("MEDIA SERVO ");
       Serial.print(num_servo);
       Serial.print(": ");
       Serial.print(media);
       Serial.println("POSICION REAL SERVO ");
       Serial.print(num_servo);
       Serial.print(": ");
       Serial.print(pos_real);
      }
  }
  return media;
}
int convert_and_clip(int angle) {
  // Función para convertir ángulos negativos en grados positivos
  // en relación a la posición de 0 grados del servo y
  // obtener ángulo recortado al rango del servo
  angle += zero_position;
  if (angle < -servo_range + zero_position) {
    return -servo_range + zero_position;
  } else if (angle > servo_range + zero_position) {
    return servo_range + zero_position;
  } else {
    return angle;
  }
}
