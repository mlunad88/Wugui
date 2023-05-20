#pragma once
class CInversa {
public:
	int x;
	int y;
	int z;

	double ang1;
	double ang2;
	double ang3;
	double ang4;
	double theta;

	// Longitudes de los eslabones del brazo rob�tico
	int l1 = 100;
	int l2 = 150;
	int l3 = 83;
	int l4 = 67;

	CInversa();
	void calcular_theta();
	void calcular_angulos(float xi, float yi, float zi);
	void calcular_ang1();
	void calcular_ang2();
	void calcular_ang3();
	void calcular_ang4();

};
