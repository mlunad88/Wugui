#pragma once
#define DIM 10
class realimentacion {
public:
	/*VARIABLES*/
	float lectura_pos;
	float pos_env;
  float pos_rec;
  float pos_real;
	int k=0;
	float err;
	float err_rel;
  int dim = DIM;
  int v[DIM];
  int num_servo;
  int cuenta_vector=0;
  
  /*METODOS*/
  void ini(void);
	float calcula_error(float);
  int cadena(void);
  void recoge(int);
  float media(float, float);
};
