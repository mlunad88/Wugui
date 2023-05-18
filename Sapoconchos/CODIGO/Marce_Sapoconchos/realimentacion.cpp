#include "realimentacion.h"
#include <math.h>

float realimentacion::calcula_error() {
	err_rel = fabs(err) / pos_env;
	return err_rel;
}

int realimentacion::cadena(){
  err = lectura_pos - pos_env;
  err_rel = calcula_error(); //calcular tolerancia
  if (err_rel < 0.01 || k <10){
    pos_env = lectura_pos - err; // corregir posicion enviada
    err_rel = calcula_error(); // recalcular tolerancia
    delay(50);
    k++;
  }
  else{
    if (err_rel >= 0.01){
      Serial.println("TOLERANCIA OK");
      return 0;
    }
    if (k >= 10){
      Serial.println("CICLOS MAXIMOS ALCANZADOS, REVISAR TOLERANCIA");
      return 0;
    }
    else{
      Serial.println("ERROR");
      return 1;
      }
  }
}
