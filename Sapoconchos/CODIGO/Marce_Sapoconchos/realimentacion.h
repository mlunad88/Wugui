#pragma once

class realimentacion {
public:
	float lectura_pos;
	float pos_env;
	int k;
	float err;
	float err_rel;

	float calculapos(void);
	float calcula_error(void);
};