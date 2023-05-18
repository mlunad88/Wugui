#include <Servo.h>
#include "realimentacion.h"

/* MACROS */
// Macros para los pines de los servos
#define S1 5
#define S2 6
// Macros para los pines analógicos de los potes
#define P1 A0
#define P2 A1
// Otras macros
#define zero_position 135 // Posición del servo correspondiente a 0 grados
#define servo_range 125  // Rango del servo entre 2, (puede ser 250 o 180 grados)
#define k (180.0/270.0) //para los servos de 270 grados, en el de 180 k=1 (osea que no hay)
#define OFFSET 59.6
#define PENDIENTE 2.765

// Creación de los objetos servo en el código
Servo servo1;
Servo servo2;

// Creación de los objetos realimentación
realimentacion eslabon1;
realimentacion eslabon2;

//float conv_ang1, conv_ang2, offset=59.6, pendiente=2.765; //supuestamente 3
//double offset2=66.5, pendiente2=3.4;
bool flag = true;
//HAY QUE AJUSTAR VALORES DE OFFSET Y PENDIENTE!!!!!

int convert_and_clip(int); //CONVERTIR ANGULOS

void setup() {
  pinMode(P1,INPUT);
  pinMode(P2,INPUT);
  
  // Para leer la posición de los servos y los potes en pantalla
  Serial.begin(9600);

  /* Unimos los servos al pin que les corresponde */
  
  // ESLABON 0 //
  servo1.attach(S1);
  eslabon1.pos_env = zero_position;
  servo1.write(eslabon1.pos_env); // Establecer la posición inicial del servo
  eslabon1.num_servo = 1;

  // ESLABON 1//
  servo2.attach(S2);
  eslabon2.pos_env = zero_position;
  servo2.write(eslabon2.pos_env); // Establecer la posición inicial del servo
  eslabon2.num_servo = 2;
}

void loop() {
  int angle1;
  
  /*LEER ANGULO MANDADO*/
  if (Serial.available()>0)
  {
    String str = Serial.readStringUntil('\n');
    angle1 = str.toFloat();
    angle1 = convert_and_clip(angle1);
    eslabon1.pos_env = k*angle1;
    //Serial.println(pos_deseada);
  };
  
  /*LECTURAS */

  // CALCULO DE LA MEDIA
  if (eslabon1.cuenta_vector < eslabon1.dim ){
    eslabon1.recoge(analogRead(P1));
    eslabon1.cuenta_vector++;
  }
  else{
    Serial.println("CALCULOS");
    Serial.println("--------------");
    Serial.print("MEDIA SERVO NUM. ");
    Serial.print(eslabon1.num_servo);
    Serial.print(": ");
    Serial.print(eslabon1.media(OFFSET, PENDIENTE));
    Serial.println("");
    Serial.print("POSICION REAL SERVO ");
    Serial.print(eslabon1.num_servo);
    Serial.print(": ");
    Serial.print(eslabon1.pos_real);
    Serial.println("");
    delay(1000);
    /* REALIMENTACION */
    
    if (flag) {eslabon1.ini()}; //inicializacion de la realimentacion
    
    eslabon1.lectura_pos = eslabon1.pos_real; //variable de posicion leida media
    
    if (eslabon1.cadena()== 0){
      Serial.println("TOLERANCIA OK");
      eslabon1.cuenta_vector = 0;
      flag = true;
      delay(1000);
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(eslabon1.cadena()==1){
      Serial.println("CICLOS MAXIMOS ALCANZADOS, REVISAR TOLERANCIA");
      eslabon1.cuenta_vector = 0;
      flag = true;
      delay(1000);
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(eslabon1.cadena()==2){
      eslabon1.pos_env = eslabon1.pos_rec;
      servo1.write(eslabon1.pos_rec);
      delay(1000);
      Serial.println("SEGUIMOS CALCULANDO");
      flag = false;
      /*SEGUIMOS EN LA REALIMENTACION*/
    };
  };  
}

int convert_and_clip(int angle) {
  /*
   * Función para convertir ángulos negativos en grados positivos
   * en relación a la posición de 0 grados del servo y
   * obtener ángulo recortado al rango del servo
   */
  angle += zero_position;
  if (angle < -servo_range + zero_position) {
    return -servo_range + zero_position;
  } else if (angle > servo_range + zero_position) {
    return servo_range + zero_position;
  } else {
    return angle;
  }
}
