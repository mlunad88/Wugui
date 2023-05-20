#pragma once
class CInversa {
public:
float x;
float y;
float z;

double ang1;
double ang2;
double ang3;
double ang4;
double theta;

// Longitudes de los eslabones del brazo robótico
float l1 = 100.0;
float l2 = 150.0;
float l3 = 83.0;
float l4 = 67.0;

CInversa(float xi, yi, zi);
void calcular_theta();
void calcular_angulos();
void calcular_ang1();
void calcular_ang2();
void calcular_ang3();
void calcular_ang4();

};
