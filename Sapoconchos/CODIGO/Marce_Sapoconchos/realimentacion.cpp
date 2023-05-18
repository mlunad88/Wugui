#include "realimentacion.h"
#include <math.h>

float realimentacion::calcula_error(float pos) {
	// CALCULAR ERROR RELATIVO
	err_rel = fabs(err) / pos;
	return err_rel;
}
void realimentacion::ini(){
  k = 0; // inicializa el contador de ciclos
  err = lectura_pos - pos_env; // calcular distancia a la que se ha quedado de la dada
  err_rel = calcula_error(); // calcular tolerancia
}

int realimentacion::cadena(){
  // IMPLEMENTACION DEL LAZO CERRADO
  if (err_rel > 0.01 || k <10){
    pos_rec = lectura_pos - err; // corregir posicion enviada
    err_rel = calcula_error(pos_rec); // recalcular tolerancia
    delay(50);
    k++;
  }
  else{
    if (err_rel <= 0.01){
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
