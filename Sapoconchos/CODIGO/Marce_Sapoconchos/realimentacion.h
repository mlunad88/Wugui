#pragma once

class realimentacion {
public:
	float lectura_pos;
	float pos_env;
  float pos_rec;
	int k=0;
	float err;
	float err_rel;

  void ini(void);
	float calcula_error(float);
  int cadena(void);
};
