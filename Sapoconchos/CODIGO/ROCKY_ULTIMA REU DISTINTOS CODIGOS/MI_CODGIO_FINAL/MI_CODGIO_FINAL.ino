#include <Servo.h>
#include "realimentacion.h"
#include "CInversa.h"
/* MACROS */
// Macros para los pines de los servos
#define S1 9 //  BASE 
#define S2 10 // HOMBRO(?) -->180 grados
#define S3 11 // CODO
#define S4 12 // MUÑECA


// Macros para los pines analógicos de los potes
#define P1 A3 //BASE
#define P2 A2 //HOMBRO
#define P3 A1 // CODO
#define P4 A0 // MUÑECA

// Otras macros
//(S1,S3,S4)
#define zero_pos_S134 135.0 // Posición del servo correspondiente a 0 grados
#define servo_range_S134 125.0  // Rango del servo entre 2, (puede ser 250 o 180 grados)
#define k_S134 (180.0/270.0) //para los servos de 270 grados, en el de 180 k=1 (osea que no hay)
#define OFFSET_S134 59.6
#define PENDIENTE_S134 2.765

//(S2)
#define zero_pos_S2 90.0
#define servo_range_S2 80.0
#define k_S2 1
#define OFFSET_S2 66.5
#define PENDIENTE_S2 3.4


// Creación de los objetos servo en el código
Servo servo1; //BASE
Servo servo2; //HOMBRO
Servo servo3; //CODO
Servo servo4; //MUÑECA 

// Creación de los objetos realimentación
realimentacion eslabon1;
realimentacion eslabon2;
realimentacion eslabon3;
realimentacion eslabon4;


CInversa cin_inversa;

struct Posicion{
float x;
float y;
float z;  
};

int velocidad=10;
int n_servo=0;

float k1=k_S134;
float k2=k_S2;


// Angulos máximos y mínimos para cada servo
int S134_ang_min = zero_pos_S134-servo_range_S134;
int S134_ang_max = zero_pos_S134+servo_range_S134;
int S2_ang_min= zero_pos_S2-servo_range_S2;
int S2_ang_max= zero_pos_S2;


void Init_pos_servos(float k, realimantacion eslabon, Servo servo, int n_servo);
int convert_and_clip_S134(int angle);
int convert_and_clip_S2(int angle);
void Mover_brazo(int a1, int a2, int a3, int a4);
void motor1();
void motor2();
void motor3();
void motor4();


void setup() {
  // put your setup code here, to run once:
  pinMode(P1,INPUT);
  pinMode(P2,INPUT);
  pinMode(P3, INPUT);
  pinMode(P4,INPUT);

    // Para leer la posición de los servos y los potes en pantalla
  Serial.begin(9600);

  servo1.attach(S1);
  servo2.attach(S2);
  servo3.attach(S3);
  servo4.attach(S4);

Servo servo, int n_servo,
  //INICIALIZAR
  void Init_pos_servos(float k1,realimentacion eslabon1, Servo servo1, 1);
  void Init_pos_servos(float k2,realimentacion eslabon2, Servo servo2, 2);
  void Init_pos_servos(float k1,realimentacion eslabon3, Servo servo3, 3);
  void Init_pos_servos(float k1,realimentacion eslabon4, Servo servo4, 4);
  cin_inversa();

}



void loop() {
  //METODO1: METER UNA POS POR MONITOR
  // Leer la posición deseada desde el Monitor Serie
 /* float x, y, z;
  Serial.println("Ingrese las coordenadas (x, y, z) deseadas:");
  Serial.print("x: ");
  while (!Serial.available());
  x = Serial.parseFloat();
  Serial.print("y: ");
  while (!Serial.available());
  y = Serial.parseFloat();
  Serial.print("z: ");
  while (!Serial.available());
  z = Serial.parseFloat();*/


  //METODO 2: HACER UNA MATRIZ DE 4 POSICIONES
   Posicion posiciones[4] = {
    {10.0, 0.0, 10.0},   // Punto 1 (X1,Y1,Z1)
    {0.0, 10.0, 10.0},   // Punto 2 (X2,Y2,Z2)
    {0.0, 0.0, 10.0},    // Punto 3 (X3,Y3,Z3)
    {10.0, 10.0, 10.0}   // Punto 4 (X4,Y4,Z4)
  };

  int n_posiciones = sizeof(posiciones) / sizeof(posiciones[0]);

    for (int i = 0; i < n_posiciones; i++) {
    float x = posiciones[i].x;
    float y = posiciones[i].y;
    float z = posiciones[i].z;


    cin_inversa.calcular_angulos(x,y,z);
 
    Mover_brazo(cin_inversa.ang1, cin_inversa.ang2, cin_inversa.ang3, cin_inversa.ang4);
    delay(2000);
    }
}


void Mover_brazo(int a1, int a2, int a3, int a4) 
{
  a1=convert_and_clip_S134(a1);
  a2=convert_and_clip_S2(a2);
  a3=convert_and_clip_S134(a3);
  a4=convert_and_clip_S134(a4);

//CREO QUE ESTO NO SE PUEDE HACER-->USAR REALIMENTACION

  int pos1 = convert_and_clip_S134(eslabon1.lectura_pos);
  int pos2 = convert_and_clip_S2(eslabon2.lectura_pos);
  int pos3 = convert_and_clip_S134(eslabon3.lectura_pos);
  int pos4 = convert_and_clip_S134(eslabon4.lectura_pos);

  while (pos1 != a1 || pos2 != a2 || pos3 != a3 || pos4 != a4) {
    if (pos1 != a1) {
      pos1 += (pos1 < a1) ? velocidad : -velocidad;
      servo1.write(pos1);
    }
    if (pos2 != a2) {
      pos2 += (pos2 < a2) ? velocidad : -velocidad;
      servo2.write(pos2);
    }
    if (pos3 != a3) {
      pos3 += (pos3 < a3) ? velocidad : -velocidad;
      servo3.write(pos3);
    }
    if (pos4 != a4) {
      pos4 += (pos4 < a4) ? velocidad : -velocidad;
      servo4.write(pos4);
    }

    delay(15); // Ajusta este valor para controlar la velocidad de giro
  }

  delay(500); // Pausa para dar tiempo a que se muevan los servos
}



void Init_pos_servos(float k, realimantacion eslabon, Servo servo, int n_servo, int p ){
  if(eslabon==eslabon2){
    eslabon.pos_env=k_S2*convert_and_clip_S2(0);
    servo.write(eslabon.pos_env);
    eslabon.offset = analogRead(P2);
    eslabon.flag = true;
    eslabon.num_servo = n_servo;}
  else
  {
    eslabon.pos_env=k_S134*convert_and_clip_S134(0);
    servo.write(eslabon.pos_env);
        switch(n_servo){
      case '1':
      eslabon.offset = analogRead(P1);
      break;
      case '3':
      eslabon.offset = analogRead(P3);
      ;
      case'4':
      eslabon.offset = analogRead(P4);
    }
    eslabon.flag = true;
    eslabon.num_servo = n_servo;}
}


int convert_and_clip_S134(int angle) {
  /*
   * Función para convertir ángulos negativos en grados positivos
   * en relación a la posición de 0 grados del servo y
   * obtener ángulo recortado al rango del servo
   */
  angle += zero_pos_S134;
  if (angle < S134_ang_min) {
    Serial.println("Te fuiste pa bajo bro");
    return S134_ang_min;
  } else if (angle > S134_ang_max) {
    Serial.println("Tas pasao bro");
    return S134_ang_max;
  } else {
    Serial.println("Todo chill");
    return angle;
  }
}

int convert_and_clip_S2(int angle) { //SOLO PUEDE MOVER ANGULOS NEGATIVOS HASTA -90
  /*
   * Función para convertir ángulos negativos en grados positivos
   * en relación a la posición de 0 grados del servo y
   * obtener ángulo recortado al rango del servo
   */
  angle += zero_pos_S2;
  if (angle < S2_ang_min) {
    Serial.println("Te fuiste pa bajo bro");
    return S2_ang_min;
  } else if (angle > S2_ang_max) {
    Serial.println("Tas pasao bro");
    return S2_ang_max;
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
    
    /* REALIMENTACION */
    eslabon1.lectura_pos = eslabon1.pos_real; //variable de posicion leida media
    if (eslabon1.flag) eslabon1.ini(); //inicializacion de la realimentacion
    
    int resultado_cadena = eslabon1.cadena();
    
    if (resultado_cadena == 0){
      Serial.println("TOLERANCIA 1 OK");
      Serial.println(eslabon1.err_rel);
      eslabon1.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 1){
      Serial.println("CICLOS MAXIMOS ALCANZADOS, REVISAR TOLERANCIA");
      eslabon1.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 2){
      Serial.println("ERROR DEMASIADO ELEVADO");
      Serial.println(eslabon1.err_rel);
      eslabon1.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 3){
      eslabon1.pos_env = eslabon1.pos_rec;
      servo1.write(k_S134*convert_and_clip(eslabon1.pos_rec));
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
      Serial.println(eslabon2.err_rel);
      eslabon2.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 1){
      Serial.println("CICLOS MAXIMOS ALCANZADOS, REVISAR TOLERANCIA");
      eslabon2.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 2){
      Serial.println("ERROR DEMASIADO ELEVADO");
      Serial.println(eslabon2.err_rel);
      eslabon2.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 3){
      eslabon2.pos_env = eslabon2.pos_rec;
      servo2.write(k_S2*convert_and_clip(eslabon2.pos_rec));
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
      Serial.println(eslabon3.err_rel);
      eslabon3.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 1){
      Serial.println("CICLOS MAXIMOS ALCANZADOS, REVISAR TOLERANCIA");
      eslabon3.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 2){
      Serial.println("ERROR DEMASIADO ELEVADO");
      Serial.println(eslabon3.err_rel);
      eslabon3.flag = true;
      /*SE SALE DE LA REALIMENTACION*/
    }
    else if(resultado_cadena == 3){
      eslabon3.pos_env = eslabon3.pos_rec;
      servo3.write(k_S2*convert_and_clip_S3(eslabon3.pos_rec));
      Serial.println("SEGUIMOS CALCULANDO");
      eslabon3.flag = false;
      /*SEGUIMOS EN LA REALIMENTACION*/
    };
    eslabon3.cuenta_vector = 0;
    Serial.println("--------------");
  };
}
