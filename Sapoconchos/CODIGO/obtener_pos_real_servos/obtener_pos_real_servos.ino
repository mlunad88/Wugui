#include <Servo.h>

Servo servo1, servo2;  // crea los objetos Servo

  //Para servo de 270: k*ángulo real
  //Para servo de 180: ángulo real
  //Si en el de 180 decimos un ángulo mayor, se queda en su tope de 180 y listo
double k= (180.0/270.0); //para los servos de 270 grados, en el de 180 k=1 (osea que no hay xd)

  //Inicializa al valor medio del ángulo de rango
int pos1 = 180;    // posicion del servo1
int pos2 = k*180;    // posicion del servo1

int realiment1 = 0; //realimentacion del servo1
int realiment2 = 0; //realimentacion del servo1

int i=0;
int j=0;
int v1[10];
int v2[10];
int suma1=0,suma2=0;


double media1, media2, conv_ang1, conv_ang2, offset1=59.6, pendiente1=2.765;//supuestamente 3
double pos_real1, offset2=66.5, pendiente2=3.4, pos_real2;
//HAY QUE AJUSTAR VALORES DE OFFSET Y PENDIENTE!!!!!


void setup() {
  Serial.begin(9600);
  
  servo1.attach(9);  // servo de 180 al pin digital 9
  servo2.attach(7);  // servo de 270 al pin digital 7

  //Inicializa la posición de los servos
  servo1.write(pos1);
  servo2.write(pos2);
  delay(3000);
}

void loop() {
  realiment1 =analogRead(A0);
  realiment2 =analogRead(A1);
 
  Serial.println(realiment1);
  Serial.println(realiment2);
  Serial.println("----------");
  
  if(i<10){
    v1[i]=realiment1;
    v2[i]=realiment2;
    suma1+=realiment1;
    suma2+=realiment2;
  }

  if(i==10){
    /*for(j=0;j<5;j++){
     Serial.println(v1[j]);
     Serial.println("///");
     Serial.println(v2[j]);*/
     media1=(suma1/10.0);
     media2=(suma2/10.0);
     pos_real1=(media1-offset1)/pendiente1;
     pos_real2=(media2-offset2)/pendiente2;
     Serial.println("MEDIAS");
     Serial.println(media1);
     Serial.println(media2);
     Serial.println("POSICIONES REALES");
     Serial.println(pos_real1);
     Serial.println(pos_real2);
     Serial.println("///");

     delay(2000);
     suma1=0;
     suma2=0;
     i=0;
  }
else
   i++;


  
}
