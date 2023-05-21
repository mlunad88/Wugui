#include <Servo.h>
#include "realimentacion.h"
#include "CInversa.h"
/* MACROS */
// Macros para los pines de los servos
#define S1 11 // CODO
#define S2 12 // MUÑECA
#define S3 10 // HOMBRO(?) -->180 grados
#define S4 9 //  BASE 


// Macros para los pines analógicos de los potes
#define P1 A1 // CODO
#define P2 A0 // MUÑECA
#define P3 A2 // HOMBRO
#define P4 A3 // BASE

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
#define servo_range_S3 80.0
#define k_S3 1
#define OFFSET_S3 66.5
#define PENDIENTE_S3 3
void setup() {
  servo3.write(eslabon3.pos_env); //Establecer la posicion inicial del servo
  eslabon3.offset = analogRead(P3);
  eslabon3.flag = true;
  eslabon3.num_servo=3;
  eslabon3.num_servo = 3;
  eslabon3.set_tol(0.01);

  //ESLABON 4//
  servo4.attach(S4);
@ -92,7 +93,7 @@ void setup() {
  servo4.write(eslabon4.pos_env); //Establecer la posicion inicial del servo
  eslabon4.offset = analogRead(P4);
  eslabon4.flag = true;
  eslabon4.num_servo=4;
  eslabon4.num_servo = 4;
}

void loop() {
@ -110,20 +111,22 @@ void loop() {
    angle1 = convert_and_clip(input);
    angle2 = convert_and_clip(input);
    angle3 = convert_and_clip_S3(input);
    angle4 = convert_and_clip(input);
    eslabon1.pos_env = k*angle1;
    eslabon2.pos_env = k*angle2;
    eslabon3.pos_env = k_S3*angle3;
    //eslabon4.pos_env = k*angle1;
    eslabon4.pos_env = k*angle4;
    servo1.write(eslabon1.pos_env);
    servo2.write(eslabon2.pos_env);
    servo3.write(eslabon3.pos_env);
    servo4.write(eslabon4.pos_env);
  };
  Serial.println(P4);
  delay(1000);
  
  // Funciones de realimentacion
  //motor1();
  //motor2();
  //motor3();
  motor1();
  motor2();
  motor3();
  motor4();
}

int convert_and_clip(int angle) {
@ -182,24 +185,22 @@ void motor1(){
    
    if (resultado_cadena == 0){
      Serial.println("TOLERANCIA 1 OK");
      Serial.println(eslabon1.err_rel);
      eslabon1.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 1){
      Serial.println("CICLOS MAXIMOS ALCANZADOS, REVISAR TOLERANCIA");
      Serial.println("CICLOS MAXIMOS ALCANZADOS EN 1, REVISAR TOLERANCIA");
      eslabon1.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 2){
      Serial.println("ERROR DEMASIADO ELEVADO");
      Serial.println(eslabon1.err_rel);
      Serial.println("ERROR 1 DEMASIADO ELEVADO");
      eslabon1.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 3){
      servo1.write(k*convert_and_clip(eslabon1.pos_env));
      eslabon1.pos_env = eslabon1.pos_rec;
      servo1.write(k*convert_and_clip(eslabon1.pos_rec));
      Serial.println("SEGUIMOS CALCULANDO");
      eslabon1.flag = false;
      /*SEGUIMOS EN LA REALIMENTACION*/
@ -227,24 +228,22 @@ void motor2(){
    
    if (resultado_cadena == 0){
      Serial.println("TOLERANCIA 2 OK");
      Serial.println(eslabon2.err_rel);
      eslabon2.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 1){
      Serial.println("CICLOS MAXIMOS ALCANZADOS, REVISAR TOLERANCIA");
      Serial.println("CICLOS MAXIMOS ALCANZADOS EN 2, REVISAR TOLERANCIA");
      eslabon2.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 2){
      Serial.println("ERROR DEMASIADO ELEVADO");
      Serial.println(eslabon2.err_rel);
      Serial.println("ERROR 2 DEMASIADO ELEVADO");
      eslabon2.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 3){
      servo2.write(k*convert_and_clip(eslabon2.pos_env));
      eslabon2.pos_env = eslabon2.pos_rec;
      servo2.write(k*convert_and_clip(eslabon2.pos_rec));
      Serial.println("SEGUIMOS CALCULANDO");
      eslabon2.flag = false;
      /*SEGUIMOS EN LA REALIMENTACION*/
@ -272,24 +271,22 @@ void motor3(){
    
    if (resultado_cadena == 0){
      Serial.println("TOLERANCIA 3 OK");
      Serial.println(eslabon3.err_rel);
      eslabon3.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 1){
      Serial.println("CICLOS MAXIMOS ALCANZADOS, REVISAR TOLERANCIA");
      Serial.println("CICLOS MAXIMOS ALCANZADOS EN 3, REVISAR TOLERANCIA");
      eslabon3.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 2){
      Serial.println("ERROR DEMASIADO ELEVADO");
      Serial.println(eslabon3.err_rel);
      Serial.println("ERROR 3 DEMASIADO ELEVADO");
      eslabon3.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 3){
      eslabon3.pos_env = eslabon3.pos_rec;
      servo3.write(k_S3*convert_and_clip_S3(eslabon3.pos_rec));
      eslabon3.pos_env = eslabon3.pos_rec;
      Serial.println("SEGUIMOS CALCULANDO");
      eslabon3.flag = false;
      /*SEGUIMOS EN LA REALIMENTACION*/
void motor3(){
    Serial.println("--------------");
  };
}
void motor4(){
  /*LECTURAS */

  // CALCULO DE LA MEDIA DE LAS LECTURAS
  if (eslabon4.cuenta_vector < eslabon4.dim ){
    eslabon4.recoge(analogRead(P4));
    eslabon4.cuenta_vector++;
  }
  else{
    eslabon4.media(PENDIENTE);
    
    /* REALIMENTACION */
    eslabon4.lectura_pos = eslabon4.pos_real; //variable de posicion leida media
    if (eslabon4.flag) eslabon4.ini(); //inicializacion de la realimentacion
    
    int resultado_cadena = eslabon4.cadena();
    
    if (resultado_cadena == 0){
      Serial.println("TOLERANCIA 4 OK");
      eslabon4.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 1){
      Serial.println("CICLOS MAXIMOS ALCANZADOS EN 4, REVISAR TOLERANCIA");
      eslabon4.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 2){
      Serial.println("ERROR 4 DEMASIADO ELEVADO");
      eslabon4.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 3){
      servo4.write(k*convert_and_clip(eslabon4.pos_rec));
      eslabon4.pos_env = eslabon4.pos_rec;
      Serial.println("SEGUIMOS CALCULANDO");
      eslabon4.flag = false;
      /*SEGUIMOS EN LA REALIMENTACION*/
    };
    eslabon4.cuenta_vector = 0;
    Serial.println("--------------");
  };
}
