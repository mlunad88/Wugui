// Clase para la realimentación y corrección del error de posición
#pragma once
#define DIM 10
class realimentacion {
public:
  /*VARIABLES*/
  float lectura_pos;
  float pos_env;
  float pos_rec;
  float pos_real;
  float offset;
  int k=0;
  float tol = 0.005;
  float err;
  double err_rel;
  int dim = DIM;
  int v[DIM];
  int num_servo;
  int cuenta_vector=0;
  bool flag;
  
  /*METODOS*/
  void ini(void);
  float calcula_error(float);
  int cadena(void);
  void recoge(int);
  void media(float);
};
