#include <Servo.h>
#include "realimentacion.h"
#include "CInversa.h"


/* MACROS */
// Macros para los pines de los servos
#define S1 9 //  BASE 
#define S2 10 // HOMBRO(?) -->180 grados
#define S3 11 // CODO
#define S4 12 // MUÑECA
#define S5 13 //GRIPER

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
Servo servo5; //GRIPPER

// Creación de los objetos realimentación
realimentacion eslabon1;
realimentacion eslabon2;
realimentacion eslabon3;
realimentacion eslabon4;



//////VARIABLES//////

CInversa cin_inversa;

struct Posicion{
float x;
float y;
float z;  
};

struct Angulos{
  float ang1;
  float ang2;
  float ang3;
  float ang4;
};

float velocidad=0.01;
int n_servo=0;
float k1=k_S134;
float k2=k_S2;


// Angulos máximos y mínimos para cada servo
float S134_ang_min = zero_pos_S134-servo_range_S134;
float S134_ang_max = zero_pos_S134+servo_range_S134;
float S2_ang_min= zero_pos_S2-servo_range_S2;
float S2_ang_max= zero_pos_S2;


////DECLARANDO FUNCIONES/////

//void Init_pos_servos(float k, realimentacion eslabon, Servo servo, int n_servo);
void Mover_brazo(float a1, float a2, float a3, float a4);

//Limitar angulos
float convert_and_clip_S134(float angle);
float convert_and_clip_S2(float angle);

//Realimentaciones
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
  servo5.attach(S5);

  //INICIALIZAR
  /* void Init_pos_servos(float k1,realimentacion eslabon1, Servo servo1,int 1);
  void Init_pos_servos(float k2,realimentacion eslabon2, Servo servo2, int 2);
  void Init_pos_servos(float k1,realimentacion eslabon3, Servo servo3, int 3);
  void Init_pos_servos(float k1,realimentacion eslabon4, Servo servo4,int 4);*/

  
   // ESLABON 1 //
  servo1.attach(S1);
  eslabon1.pos_env = k_S134*convert_and_clip_S134(0);
  servo1.write(eslabon1.pos_env); // Establecer la posición inicial del servo
  eslabon1.offset = analogRead(P1);
  eslabon1.flag = true;
  eslabon1.num_servo = 1;

  // ESLABON 2//
  eslabon2.pos_env = k_S2*convert_and_clip_S134(0);
  servo2.write(eslabon2.pos_env); // Establecer la posición inicial del servo
  eslabon2.offset = analogRead(P2);
  eslabon2.flag = true;
  eslabon2.num_servo = 2;
  
  //ESLABON 3//
  eslabon3.pos_env=k_S134*convert_and_clip_S134(0);
  servo3.write(eslabon3.pos_env); //Establecer la posicion inicial del servo
  eslabon3.offset = analogRead(P3);
  eslabon3.flag = true;
  eslabon3.num_servo = 3;
  eslabon3.set_tol(0.01);

  //ESLABON 4//
  eslabon4.pos_env=k_S134*convert_and_clip_S134(0);
  servo4.write(eslabon4.pos_env); //Establecer la posicion inicial del servo
  eslabon4.offset = analogRead(P4);
  eslabon4.flag = true;
  eslabon4.num_servo = 4;

  //GRIPPER//
  servo5.write(0);
}



void loop() {
  /*CON UN SWITCH SE VA A PODER ESCOGER QUE QUEREMOS HACER:
      1. QUE EL ROBOT SIGA UN CICLO DE TRABAJO MOVIENDOSE A LOS 4 ANGULOS ASIGNADAS
      2. QUE POR PANTALLA TE PIDA A CUAL DE LOS CUATRO ANGULOS TE QUIERES MOVER
      3. QUE INTRODUZCAS POR PANTALLA 4 ANGULOS DE GIRO 
      4. QUE EL ROBOT SIGA UN CICLO DE TRABAJO MOVIENDOSE A LOS 4 POSICIONES CALCULADAS
      5. QUE POR PANTALLA TE PIDA A CUAL DE LAS CUATRO POSICIONES TE QUIERES MOVER
      6. QUE INTRODUZCAS POR PANTALLA UNA POSICION POR COORDENADAS 
   
   */

   Posicion posiciones[4] = {{0.0,0.0,0.0},{0.0,0.0,0.0},{0.0,0.0, 0.0},{0.0,0.0,0.0} };

   Angulos angulos[4]={
     {0.0, 0.0, 0.0, 0.0 },// Punto 1 (III)
     {90.0, -70.0, 0.0, -90.0 },  // Punto 2    
     {-10.0, -90.0, -80.0, 5.0},   // Punto 3
     {-110.0, -90.0, -50.0, -20.0}   //Punto 4
  };

  for(int i=0;i<4;i++){
    cin_inversa.set_angulos(angulos[i].ang1,angulos[i].ang2,angulos[i].ang3,angulos[i].ang4);
    cin_inversa.calcular_cdirecta();
    posiciones[i]={cin_inversa.x,cin_inversa.y,cin_inversa.z};
  }

        //IMPRIMIR POSICIONES
        Serial.println("POS0.X:");
        Serial.println(posiciones[0].x);
        Serial.println("POS0.Y:");
        Serial.println(posiciones[0].y);
        Serial.println("POS0.Z:");
        Serial.println(posiciones[0].z);
        Serial.println("POS1.X:");
        Serial.println(posiciones[1].x);
        Serial.println("POS1.Y:");
        Serial.println(posiciones[1].y);
        Serial.println("POS1.Z:");
        Serial.println(posiciones[1].z);
        Serial.println("POS2.X:");
        Serial.println(posiciones[2].x);
        Serial.println("POS2.Y:");
        Serial.println(posiciones[2].y);
        Serial.println("POS2.Z:");
        Serial.println(posiciones[2].z);
        Serial.println("POS3.X:");
        Serial.println(posiciones[3].x);
        Serial.println("POS3.Y:");
        Serial.println(posiciones[3].y);
        Serial.println("POS3.Z:");
        Serial.println(posiciones[3].z);


  int n=0;
  Serial.println("Ingrese la opcion deseada:");
  while (!Serial.available());
  String str_n=Serial.readStringUntil('\n');
  n = str_n.toInt();

  switch(n){
    case 1:
    {
        //CICLO TRABAJO con ANGULOS -->faltan pasos intermedios
        Mover_brazo(angulos[0].ang1,angulos[0].ang2,angulos[0].ang3,angulos[0].ang4);
        servo5.write(0);
        delay(5000);

        Mover_brazo(angulos[1].ang1,angulos[1].ang2,angulos[1].ang3,angulos[1].ang4);
        delay(2000);
        servo5.write(160);
        delay(5000);

        Mover_brazo(angulos[2].ang1,angulos[2].ang2,angulos[2].ang3,angulos[2].ang4);
        delay(2000);
        for(int i=0;i<5;i++)
        {
          servo5.write(120);
          delay(500);
          servo5.write(160);
          delay(500);
        }

        Mover_brazo(angulos[3].ang1,angulos[3].ang2,angulos[3].ang3,angulos[3].ang4);
        delay(2000);
        servo5.write(0);
        delay(5000);

         Mover_brazo(angulos[0].ang1,angulos[0].ang2,angulos[0].ang3,angulos[0].ang4);
        
        delay(5000);
    }
    break;
    case 2: //METER POR PANTALLA A CUAL DE LOS CUATRO ANGULOS QUIERES MOVERTE
    {
      Serial.println("Ingrese el punto al que quiere moverse:");

      Serial.print("Punto: ");
      while (!Serial.available());
      String str_p=Serial.readStringUntil('\n');
      int p = str_p.toInt();

      Mover_brazo(angulos[p].ang1,angulos[p].ang2,angulos[p].ang3,angulos[p].ang4);
      
    }
    break;
    case 3: //INTRODUCIR POR PANTALLA ANGULOS DE GIRO
    {
      Serial.println("Ingrese los 4 ANGULOS deseados:");
      
      Serial.print("ANGULO1: ");
      while (!Serial.available());
      String str_A1=Serial.readStringUntil('\n');
      float angulo1 = str_A1.toFloat();
      
      Serial.print("ANGULO2: ");
      while (!Serial.available());
      String str_A2=Serial.readStringUntil('\n');
      float angulo2 = str_A2.toFloat();

      Serial.print("ANGULO3: ");
      while (!Serial.available());
      String str_A3=Serial.readStringUntil('\n');
      float angulo3 = str_A3.toFloat();

      Serial.print("ANGULO4: ");
      while (!Serial.available());
      String str_A4=Serial.readStringUntil('\n');
      float angulo4 = str_A4.toFloat();

      Mover_brazo(angulo1,angulo2,angulo3,angulo4);
    }
    break;
    case 4:
    {
        //CICLO TRABAJO con POSICIONES -->faltan pasos intermedios
        motor1();
        motor2();
        motor3();
        motor4(); 
       
        cin_inversa.calcular_angulos(posiciones[0].x,posiciones[0].y,posiciones[0].z);
        Mover_brazo(cin_inversa.ang1, cin_inversa.ang2, cin_inversa.ang3, cin_inversa.ang4);
        servo5.write(0);
        delay(5000);
        
        cin_inversa.calcular_angulos(posiciones[1].x,posiciones[1].y,posiciones[1].z);
        Mover_brazo(cin_inversa.ang1, cin_inversa.ang2, cin_inversa.ang3, cin_inversa.ang4);
        delay(2000);
        servo5.write(170);
        delay(5000);

        cin_inversa.calcular_angulos(posiciones[2].x,posiciones[2].y,posiciones[2].z);
        Mover_brazo(cin_inversa.ang1, cin_inversa.ang2, cin_inversa.ang3, cin_inversa.ang4);
        delay(2000);
        for(int i=0;i<5;i++)
        {
          servo5.write(150);
          delay(500);
          servo5.write(170);
          delay(500);
        }
       
        cin_inversa.calcular_angulos(posiciones[3].x,posiciones[3].y,posiciones[3].z);
        Mover_brazo(cin_inversa.ang1, cin_inversa.ang2, cin_inversa.ang3, cin_inversa.ang4);
        delay(2000);
        servo5.write(0);
        delay(5000);

        cin_inversa.calcular_angulos(posiciones[0].x,posiciones[0].y,posiciones[0].z);
        Mover_brazo(cin_inversa.ang1, cin_inversa.ang2, cin_inversa.ang3, cin_inversa.ang4);
        
        delay(5000);
      
    }
    break;
    case5:
    {

      
    }
    break;
    case 6:
    {
      // Leer la posición deseada desde el Monitor Serie
       
        float x, y, z;
        Serial.println("Ingrese las coordenadas (x, y, z) deseadas:");
        Serial.print("x: ");
        while (!Serial.available());
        String strx=Serial.readStringUntil('\n');
        x = strx.toFloat();  

        Serial.print("y: ");
        while (!Serial.available());
        String stry=Serial.readStringUntil('\n');
        y = stry.toFloat();
        
        Serial.print("z: ");
        while (!Serial.available());
        String strz=Serial.readStringUntil('\n');
        z = strz.toFloat();

        cin_inversa.calcular_angulos(x,y,z);
         motor1();
         motor2();
         motor3();
         motor4(); 
         Mover_brazo(cin_inversa.ang1, cin_inversa.ang2, cin_inversa.ang3, cin_inversa.ang4);

    }
    break;
    default:
    break;
  }
   

  
 
    
}


void Mover_brazo(float a1, float a2, float a3, float a4) 
{
  a1=k_S134*convert_and_clip_S134(a1);
  a2=k_S2*convert_and_clip_S2(a2);
  a3=k_S134*convert_and_clip_S134(a3);
  a4=k_S134*convert_and_clip_S134(a4);


  servo1.write(a1);
  servo2.write(a2);
  servo3.write(a3);
  servo4.write(a4);


  //CONTROL VELOCIDAD
  /*
  int pos1 = k_S134*convert_and_clip_S134(eslabon1.lectura_pos);
  int pos2 = k_S2*convert_and_clip_S2(eslabon2.lectura_pos);
  int pos3 = k_S134*convert_and_clip_S134(eslabon3.lectura_pos);
  int pos4 = k_S134*convert_and_clip_S134(eslabon4.lectura_pos); 
  
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

    delay(50); // Ajusta este valor para controlar la velocidad de giro
  }
*/
  delay(500); // Pausa para dar tiempo a que se muevan los servos
}


float convert_and_clip_S134(float angle) {

  //SERVO 270
  /*
   * Función para convertir ángulos negativos en grados positivos
   * en relación a la posición de 0 grados del servo y
   * obtener ángulo recortado al rango del servo
   */
  angle += zero_pos_S134;
  if (angle < S134_ang_min) {
    //Serial.println("Te fuiste pa bajo bro");
    return S134_ang_min;
  } else if (angle > S134_ang_max) {
   // Serial.println("Tas pasao bro");
    return S134_ang_max;
  } else {
   // Serial.println("Todo chill");
    return angle;
  }
}

float convert_and_clip_S2(float angle) { //SOLO PUEDE MOVER ANGULOS NEGATIVOS HASTA -90
  
  //SERVO 180
  
  /*
   * Función para convertir ángulos negativos en grados positivos
   * en relación a la posición de 0 grados del servo y
   * obtener ángulo recortado al rango del servo
   */
  angle += zero_pos_S2;
  if (angle < S2_ang_min) {
    //Serial.println("Te fuiste pa bajo bro");
    return S2_ang_min;
  } else if (angle > S2_ang_max) {
   // Serial.println("Tas pasao bro");
    return S2_ang_max;
  } else {
   // Serial.println("Todo chill");
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
    eslabon1.media(PENDIENTE_S134);
    
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
      servo1.write(k_S134*convert_and_clip_S134(eslabon1.pos_rec));
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
    eslabon2.media(PENDIENTE_S2);
    
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
      servo2.write(k_S2*convert_and_clip_S2(eslabon2.pos_rec));
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
    eslabon3.media(PENDIENTE_S134);
    
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
      servo3.write(k_S2*convert_and_clip_S134(eslabon3.pos_rec));
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
    eslabon4.media(PENDIENTE_S134);
    
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
      servo4.write(k_S134*convert_and_clip_S134(eslabon4.pos_rec));
      eslabon4.pos_env = eslabon4.pos_rec;
      Serial.println("SEGUIMOS CALCULANDO");
      eslabon4.flag = false;
      /*SEGUIMOS EN LA REALIMENTACION*/
    };
    eslabon4.cuenta_vector = 0;
    Serial.println("--------------");
  };
}
