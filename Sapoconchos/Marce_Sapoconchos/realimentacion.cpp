#include "realimentacion.h"
#include <math.h>

float realimentacion::calculapos() {
	pos_env = lectura_pos - err;
	return pos_env;
}

float realimentacion::calcula_error() {
	err_rel = fabs(err) / pos_env;
	return err_rel;
}