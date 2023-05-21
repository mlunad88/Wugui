#pragma once
class CInversa {
public:
	float x;
	float y;
	float z;

  float a1; //radianes
  float a2;
  float a3;
  float a4;

	float ang1; //grados
	float ang2;
	float ang3;
	float ang4;
	float alfa, beta, omega;

	// Longitudes de los eslabones del brazo rob�tico
	float l1 = 100.0;
	float l2 = 150.0;
	float l3 = 83.0;
	float l4 = 67.0;

	CInversa();
	void calcular_alfa_beta_omega();
	void calcular_angulos(float xi, float yi, float zi);
	void calcular_ang1();
	void calcular_ang2();
	void calcular_ang3();
	void calcular_ang4();
 void calcular_cdirecta();
 void set_angulos(float angulo1,float angulo2,float angulo3,float angulo4);

};
