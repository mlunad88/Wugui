#include "realimentacion.h"
#include <math.h>

float realimentacion::calcula_error(float pos) {
	// CALCULAR ERROR RELATIVO
	err_rel = (double) fabs(err) / pos;
	return err_rel;
}
void realimentacion::ini(){
  k = 0; // inicializa el contador de ciclos
  tol = 0.005; // valor de la tolerancia
  err = lectura_pos - pos_env; // calcular distancia a la que se ha quedado de la dada
  err_rel = calcula_error(pos_env) - 1; // calcular el error rel a la posicion enviada
}
void realimentacion::recoge(int lectura_raw){
  v[cuenta_vector] = lectura_raw;// Introduce en el vector las lecturas para hacer la media
}
void realimentacion::media(float pendiente){
  int suma=0;
  float media = 0;
  for (int i = 0; i<=DIM; i++){
    if (i<DIM){
      suma+=v[i];
    }
    if (i==DIM){
     media=(float)(suma/DIM);
     pos_real=(float)(media-offset)/pendiente;
     pos_real = read_traduct(pos_real);
    }
  };
}
float realimentacion::read_traduct(float pos){
  if (num_servo == 3){
    if (pos <= 0) pos = (-90.0 / 280.0) * pos;
    else if (pos > 0) pos = (90.0 / 280.0) * pos;
  }
  else{
    if (pos < 0) pos = (-125.0 / 88.97) * pos;
    if (pos >= 0) pos = (125.0 / 84.99) * pos;
  };
  return pos;
}
int realimentacion::cadena(){
  // IMPLEMENTACION DEL LAZO CERRADO
  if ( (err_rel > tol && err_rel < 0.20 ) || k < 10){
    pos_rec = lectura_pos - err; // corregir posicion enviada
    err_rel = calcula_error(pos_rec) - 1; // recalcular tolerancia
    k++;
  }
  if (err_rel <= tol) return 0;
  else if (k >= 10) return 1;
  else if (err_rel >= 0.20) return 2;
  else return 3;
}
