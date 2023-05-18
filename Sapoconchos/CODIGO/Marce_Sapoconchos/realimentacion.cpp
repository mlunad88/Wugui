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
  err_rel = calcula_error(lectura_pos); // calcular tolerancia
}
void realimentacion::recoge(int lectura_raw){
  v[cuenta_vector] = lectura_raw;// Introduce en el vector las lecturas para hacer la media
}
float realimentacion::media(float offset, float pendiente){
  int suma=0;
  float media = 0;
    for (int i = 0; i<=DIM; i++){
      if (i<DIM){
        suma+=v[i];
      }
      if (i==DIM){
       media=(float)(suma/DIM);
       pos_real=(float)(media-offset)/pendiente;
      }
  }
  return media;
}

int realimentacion::cadena(){
  // IMPLEMENTACION DEL LAZO CERRADO
  if (err_rel > 0.01 || k <10){
    pos_rec = lectura_pos - err; // corregir posicion enviada
    err_rel = calcula_error(pos_rec); // recalcular tolerancia
    k++;
  }
  if (err_rel <= 0.01){
    return 0;
  }
  else if (k >= 10){
    return 1;
  }
  else{
    return 2;
  };
}
