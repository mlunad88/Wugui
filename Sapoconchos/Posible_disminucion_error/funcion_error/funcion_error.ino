
//FUNCION FEEDBACK CONTROL K: REDUCIR EL ERROR DE POSICION
//error = pos_s - pos_e
//pos_e = error * k


#include <Servo.h>
#define PIN_POTEN A0  //pin posicion potenciometro (entrada)
#define PIN_SERVO 9  //pin posicion servo (salida)
#define PIN_POSOUT 12 //GIRO DCHA
#define PIN_NEGOUT 13 //GIRO IZDA
#define Nivel_servo 90
Servo out_servo;

double input, output, ang_serv, setpoint;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PIN_POSOUT, OUTPUT);
  pinMode(PIN_NEGOUT, OUTPUT);
  pinMode(PIN_SERVO, OUTPUT);
  pinMode(PIN_POTEN, INPUT);

  out_servo.attach(11);
}

void loop() {
  // put your main code here, to run repeatedly:
  ang_serv = out_servo.read();
  input=analogRead(PIN_POTEN);
  int e = (ang_serv - input)*255/1023;

  if(e<0){
    analogWrite(PIN_SERVO, e*50);
    digitalWrite(PIN_POSOUT, HIGH);
    digitalWrite (PIN_NEGOUT, LOW);
  }
  else{
    analogWrite(PIN_SERVO, e*50);
    digitalWrite(PIN_POSOUT, LOW);
    digitalWrite (PIN_NEGOUT, HIGH);
  }
}
