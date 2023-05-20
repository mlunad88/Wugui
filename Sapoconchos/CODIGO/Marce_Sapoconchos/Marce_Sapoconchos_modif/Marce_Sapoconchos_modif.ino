#include <Servo.h>
#include "realimentacion.h"

/* MACROS */
// Macros para los pines de los servos
#define S1 11 // CODO
#define S2 12 // MUÑECA
#define S3 10 // HOMBRO(?) -->180 grados
#define S4 9 //  BASE 


// Macros para los pines analógicos de los potes
#define P1 A1 // CODO
#define P2 A0 // MUÑECA
#define P3 A2 //HOMBRO
#define P4 A3 //BASE

// Otras macros
//(S1 & S2)
#define zero_position_S1S2 135.0 // Posición del servo correspondiente a 0 grados
#define servo_range_S1S2 125.0  // Rango del servo entre 2, (puede ser 250 o 180 grados)
#define k (180.0/270.0) //para los servos de 270 grados, en el de 180 k=1 (osea que no hay)
#define OFFSET 59.6
#define PENDIENTE 2.765

//(S3)
#define zero_position_S3 90.0
#define servo_range_S3 90.0
#define k_S3 1
#define OFFSET_S3 66.5
#define PENDIENTE_S3 3.4


// Creación de los objetos servo en el código
Servo servo1; //CODO
Servo servo2; //MUÑECA
Servo servo3; //HOMBRO
Servo servo4; //BASE

// Creación de los objetos realimentación
realimentacion eslabon1;
realimentacion eslabon2;
realimentacion eslabon3;
realimentacion eslabon4;

//float conv_ang1, conv_ang2, offset=59.6, pendiente=2.765; //supuestamente 3
//double offset2=66.5, pendiente2=3.4;
bool flag = true;
bool flag2 = true;
//HAY QUE AJUSTAR VALORES DE OFFSET Y PENDIENTE!!!!!

int convert_and_clip(int); //CONVERTIR ANGULOS
void motor1(void);
void motor2(void);

void setup() {
  pinMode(P1,INPUT);
  pinMode(P2,INPUT);
  pinMode(P3, INPUT);
  pinMode(P4,INPUT);
  
  // Para leer la posición de los servos y los potes en pantalla
  Serial.begin(9600);

  /* Unimos los servos al pin que les corresponde */
  
  // ESLABON 0 //
  /*servo1.attach(S1);
  
  eslabon1.pos_env = k*convert_and_clip(0);
  servo1.write(eslabon1.pos_env); // Establecer la posición inicial del servo
  eslabon1.num_servo = 1;*/

  // ESLABON 1//
 /* servo2.attach(S2);
  eslabon2.pos_env = k*convert_and_clip(0);
  servo2.write(eslabon2.pos_env); // Establecer la posición inicial del servo
  eslabon2.num_servo = 2;*/
  
  //ESLABON 2//
  servo3.attach(S3);
  eslabon3.pos_env=k_S3*convert_and_clip_S3(0);
  servo3.write(eslabon3.pos_env); //Establecer la posicion inicial del servo
  eslabon3.num_servo=3;

  //ESLABON 3//
  /*
  servo4.attach(S4);
  eslabon4.pos_env=k*convert_and_clip(0);
  servo4.write(eslabon4.pos_env); //Establecer la posicion inicial del servo
  eslabon4.num_servo=4;
  */
}

void loop() {
  int angle1, angle2, angle3, angle4;
  
  /*LEER ANGULO MANDADO*/
  if (Serial.available()>0)
  {
    String str = Serial.readStringUntil('\n');
    angle1 = str.toFloat();
    Serial.println(" ");
    Serial.print("Has enviado: ");
    Serial.print(angle1);
    Serial.println(" ");
    angle1 = convert_and_clip_S3(angle1);
    eslabon3.pos_env = k_S3*angle1;
  };
  //motor1();
  //motor2();
  servo3.write(eslabon3.pos_env);
  delay(1000);
}

int convert_and_clip(int angle) {
  /*
   * Función para convertir ángulos negativos en grados positivos
   * en relación a la posición de 0 grados del servo y
   * obtener ángulo recortado al rango del servo
   */
  angle += zero_position_S1S2;
  if (angle < -servo_range_S1S2 + zero_position_S1S2) {
    Serial.println("Te fuiste pa bajo bro");
    return -servo_range_S1S2 + zero_position_S1S2;
  } else if (angle > servo_range_S1S2 + zero_position_S1S2) {
    Serial.println("Tas pasao bro");
    return servo_range_S1S2 + zero_position_S1S2;
  } else {
    Serial.println("Todo chill");
    return angle;
  }
}

int convert_and_clip_S3(int angle) { //SOLO PUEDE MOVER ANGULOS NEGATIVOS HASTA -90
  /*
   * Función para convertir ángulos negativos en grados positivos
   * en relación a la posición de 0 grados del servo y
   * obtener ángulo recortado al rango del servo
   */
  angle += zero_position_S3;
  if (angle < -servo_range_S3 + zero_position_S3) {
    Serial.println("Te fuiste pa bajo bro");
    return -servo_range_S3 + zero_position_S3;
  } else if (angle > zero_position_S3) {
    Serial.println("Tas pasao bro");
    return zero_position_S3;
  } else {
    Serial.println("Todo chill");
    return angle;
  }
}
void motor1(){
  /*LECTURAS */

  // CALCULO DE LA MEDIA
  if (eslabon1.cuenta_vector < eslabon1.dim ){
    eslabon1.recoge(analogRead(P1));
    eslabon1.cuenta_vector++;
  }
  else{
    Serial.println(" ");
    Serial.println("--------------");
    Serial.println("CALCULOS");
    Serial.print("MEDIA SERVO NUM. ");
    Serial.print(eslabon1.num_servo);
    Serial.print(": ");
    Serial.print(eslabon1.media(OFFSET, PENDIENTE));
    Serial.println("");
    Serial.print("POSICION REAL SERVO ");
    Serial.print(eslabon1.num_servo);
    Serial.print(": ");
    Serial.print(eslabon1.pos_real);
    Serial.println(" ");
    delay(500);
    /* REALIMENTACION */
    
    if (flag) {eslabon1.ini();}; //inicializacion de la realimentacion
    
    eslabon1.lectura_pos = eslabon1.pos_real; //variable de posicion leida media
    
    int resultado_cadena = eslabon1.cadena();
    
    if (resultado_cadena == 0){
      Serial.println("TOLERANCIA 1 OK");
      eslabon1.cuenta_vector = 0;
      flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 1){
      Serial.println("CICLOS MAXIMOS ALCANZADOS, REVISAR TOLERANCIA");
      eslabon1.cuenta_vector = 0;
      flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 2){
      eslabon1.pos_env = eslabon1.pos_rec;
      servo1.write(eslabon1.pos_rec);
      Serial.println("SEGUIMOS CALCULANDO");
      flag = false;
      /*SEGUIMOS EN LA REALIMENTACION*/
    };
    Serial.println("--------------");
  };
}
void motor2(){
  /*LECTURAS */

  // CALCULO DE LA MEDIA
  if (eslabon2.cuenta_vector < eslabon2.dim){
      eslabon2.recoge(analogRead(P2));
      eslabon2.cuenta_vector++;
  }
  else{
    Serial.println(" ");
    Serial.println("--------------");
    Serial.println("CALCULOS");
    Serial.print("MEDIA SERVO NUM. ");
    Serial.print(eslabon2.num_servo);
    Serial.print(": ");
    Serial.print(eslabon2.media(OFFSET, PENDIENTE));
    Serial.println("");
    Serial.print("POSICION REAL SERVO ");
    Serial.print(eslabon2.num_servo);
    Serial.print(": ");
    Serial.print(eslabon2.pos_real);
    Serial.println(" ");
    delay(500);
    /* REALIMENTACION */
    
    if (flag2) {eslabon2.ini();}; //inicializacion de la realimentacion
    
    eslabon2.lectura_pos = eslabon2.pos_real; //variable de posicion leida media
    
    int resultado_cadena2 = eslabon2.cadena();
    if (resultado_cadena2 == 0){
      Serial.println("TOLERANCIA OK");
      eslabon2.cuenta_vector = 0;
      flag2 = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena2 == 1){
      Serial.println("CICLOS MAXIMOS ALCANZADOS, REVISAR TOLERANCIA");
      eslabon2.cuenta_vector = 0;
      flag2 = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena2 == 2){
      eslabon2.pos_env = eslabon2.pos_rec;
      //servo2.write(eslabon2.pos_rec);
      Serial.println("SEGUIMOS CALCULANDO");
      flag2 = false;
      /*SEGUIMOS EN LA REALIMENTACION*/
    };
    
    Serial.println("--------------");
  };
}
