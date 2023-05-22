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
#define servo_range_S3 80.0
#define k_S3 1
#define OFFSET_S3 66.5
#define PENDIENTE_S3 3


// Creación de los objetos servo en el código
Servo servo1; //CODO
Servo servo2; //MUÑECA
Servo servo3; //HOMBRO
Servo servo4; //BASE

// CINEMATICA INVERSA
//CInversa cin_inversa;

// Creación de los objetos realimentación
realimentacion eslabon1;
realimentacion eslabon2;
realimentacion eslabon3;
realimentacion eslabon4;

int convert_and_clip(int); //CONVERTIR ANGULOS
int convert_and_clip_S3(int); //CONVERTIR ANGULOS
void viajar_punto(int);
void esencial(int);

void motor1(void);
void motor2(void);
void motor3(void);
void motor4(void);

void setup() {
  pinMode(P1,INPUT);
  pinMode(P2,INPUT);
  pinMode(P3,INPUT);
  pinMode(P4,INPUT);
  
  // Para leer la posición de los servos y los potes en pantalla
  Serial.begin(9600);

  /* Unimos los servos al pin que les corresponde */
  
  // ESLABON 1 //
  servo1.attach(S1);
  eslabon1.pos_env = k*convert_and_clip(0);
  servo1.write(eslabon1.pos_env); // Establecer la posición inicial del servo
  eslabon1.offset = analogRead(P1);
  eslabon1.flag = true;
  eslabon1.num_servo = 1;

  // ESLABON 2//
  servo2.attach(S2);
  eslabon2.pos_env = k*convert_and_clip(0);
  servo2.write(eslabon2.pos_env); // Establecer la posición inicial del servo
  eslabon2.offset = analogRead(P2);
  eslabon2.flag = true;
  eslabon2.num_servo = 2;
  
  //ESLABON 3//
  servo3.attach(S3);
  eslabon3.pos_env=k_S3*convert_and_clip_S3(0);
  servo3.write(eslabon3.pos_env); //Establecer la posicion inicial del servo
  eslabon3.offset = analogRead(P3);
  eslabon3.flag = true;
  eslabon3.num_servo = 3;
  eslabon3.set_tol(0.01);

  //ESLABON 4//
  servo4.attach(S4);
  eslabon4.pos_env=k*convert_and_clip(0);
  servo4.write(eslabon4.pos_env); //Establecer la posicion inicial del servo
  eslabon4.offset = analogRead(P4);
  eslabon4.flag = true;
  eslabon4.num_servo = 4;

  delay(2000);
}

void loop() {
  if (Serial.available()>0){
    String str = Serial.readStringUntil('\n');
    int input = str.toFloat();
    esencial(input);
    Serial.println(input);
    servo1.write(eslabon1.pos_env);
    servo2.write(eslabon2.pos_env);
    servo3.write(eslabon3.pos_env);
    servo4.write(eslabon4.pos_env);
  };
  
  // Funciones de realimentacion
  motor1();
  motor2();
  motor3();
  motor4();
}
void esencial(int n){
  int angle1, angle2, angle3, angle4;
  angle1 = convert_and_clip(n);
  angle2 = convert_and_clip(n);
  angle3 = convert_and_clip_S3(-n);
  angle4 = convert_and_clip(n);
  eslabon1.pos_env = k*angle1;
  eslabon2.pos_env = k*angle2;
  eslabon3.pos_env = k_S3*angle3;
  eslabon4.pos_env = k*angle4;
}
void viajar_punto(int n){
  int angle1, angle2, angle3, angle4;
  
  switch(n){
    case 1:{
      angle1 = convert_and_clip(20);
      angle2 = convert_and_clip(20);
      angle3 = convert_and_clip_S3(-20);
      angle4 = convert_and_clip(20);
      eslabon1.pos_env = k*angle1;
      eslabon2.pos_env = k*angle2;
      eslabon3.pos_env = k_S3*angle3;
      eslabon4.pos_env = k*angle4;
    }
    case 2:{
      angle1 = convert_and_clip(30);
      angle2 = convert_and_clip(30);
      angle3 = convert_and_clip_S3(-40);
      angle4 = convert_and_clip(20);
      eslabon1.pos_env = k*angle1;
      eslabon2.pos_env = k*angle2;
      eslabon3.pos_env = k_S3*angle3;
      eslabon4.pos_env = k*angle4;
    }
    case 3:{
      angle1 = convert_and_clip(0);
      angle2 = convert_and_clip(30);
      angle3 = convert_and_clip_S3(-60);
      angle4 = convert_and_clip(30);
      eslabon1.pos_env = k*angle1;
      eslabon2.pos_env = k*angle2;
      eslabon3.pos_env = k_S3*angle3;
      eslabon4.pos_env = k*angle4;
    }
    break;
  };
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

  // CALCULO DE LA MEDIA DE LAS LECTURAS
  if (eslabon1.cuenta_vector < eslabon1.dim ){
    eslabon1.recoge(analogRead(P1));
    eslabon1.cuenta_vector++;
  }
  else{
    eslabon1.media(PENDIENTE);
    eslabon1.lectura_pos = eslabon1.pos_real; //variable de posicion leida media
    /* REALIMENTACION */
    if (eslabon1.flag) eslabon1.ini(); //inicializacion de la realimentacion
    
    int resultado_cadena = eslabon1.cadena();
    
    if (resultado_cadena == 0){
      Serial.println("TOLERANCIA 1 OK");
      eslabon1.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 1){
      Serial.println("CICLOS MAXIMOS ALCANZADOS EN 1, REVISAR TOLERANCIA");
      eslabon1.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 2){
      Serial.println("ERROR 1 DEMASIADO ELEVADO");
      eslabon1.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 3){
      servo1.write(k*convert_and_clip(eslabon1.pos_env));
      eslabon1.pos_env = eslabon1.pos_rec;
      Serial.println("SEGUIMOS CALCULANDO");
      eslabon1.flag = false;
      /*SEGUIMOS EN LA REALIMENTACION*/
    };
    eslabon1.cuenta_vector = 0;
    Serial.println("--------------");
  };
}
void motor2(){
  /*LECTURAS */

  // CALCULO DE LA MEDIA DE LAS LECTURAS
  if (eslabon2.cuenta_vector < eslabon2.dim ){
    eslabon2.recoge(analogRead(P2));
    eslabon2.cuenta_vector++;
  }
  else{
    eslabon2.media(PENDIENTE);
    
    /* REALIMENTACION */
    eslabon2.lectura_pos = eslabon2.pos_real; //variable de posicion leida media
    if (eslabon2.flag) eslabon2.ini(); //inicializacion de la realimentacion
    
    int resultado_cadena = eslabon2.cadena();
    
    if (resultado_cadena == 0){
      Serial.println("TOLERANCIA 2 OK");
      eslabon2.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 1){
      Serial.println("CICLOS MAXIMOS ALCANZADOS EN 2, REVISAR TOLERANCIA");
      eslabon2.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 2){
      Serial.println("ERROR 2 DEMASIADO ELEVADO");
      eslabon2.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 3){
      servo2.write(k*convert_and_clip(eslabon2.pos_env));
      eslabon2.pos_env = eslabon2.pos_rec;
      Serial.println("SEGUIMOS CALCULANDO");
      eslabon2.flag = false;
      /*SEGUIMOS EN LA REALIMENTACION*/
    };
    eslabon2.cuenta_vector = 0;
    Serial.println("--------------");
  };
}
void motor3(){
  /*LECTURAS */

  // CALCULO DE LA MEDIA DE LAS LECTURAS
  if (eslabon3.cuenta_vector < eslabon3.dim ){
    eslabon3.recoge(analogRead(P3));
    eslabon3.cuenta_vector++;
  }
  else{
    eslabon3.media(PENDIENTE);
    
    /* REALIMENTACION */
    eslabon3.lectura_pos = eslabon3.pos_real; //variable de posicion leida media
    if (eslabon3.flag) eslabon3.ini(); //inicializacion de la realimentacion
    
    int resultado_cadena = eslabon3.cadena();
    
    if (resultado_cadena == 0){
      Serial.println("TOLERANCIA 3 OK");
      eslabon3.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 1){
      Serial.println("CICLOS MAXIMOS ALCANZADOS EN 3, REVISAR TOLERANCIA");
      eslabon3.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 2){
      Serial.println("ERROR 3 DEMASIADO ELEVADO");
      eslabon3.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 3){
      servo3.write(k_S3*convert_and_clip_S3(eslabon3.pos_rec));
      eslabon3.pos_env = eslabon3.pos_rec;
      Serial.println("SEGUIMOS CALCULANDO");
      eslabon3.flag = false;
      /*SEGUIMOS EN LA REALIMENTACION*/
    };
    eslabon3.cuenta_vector = 0;
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
