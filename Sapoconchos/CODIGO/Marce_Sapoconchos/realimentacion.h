#pragma once

class realimentacion {
public:
	float lectura_pos;
	float pos_env;
	int k=0;
	float err;
	float err_rel;

	float calcula_error(void);
  int cadena(void);
};
