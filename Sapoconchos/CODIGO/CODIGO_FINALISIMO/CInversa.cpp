#include "CInversa.h"
#include <math.h>
#define PI 3.15
CInversa::CInversa() {
	x = 0.0;
	y = 0.0;
	z = 0.0;
	l1 = 100.0;
	l2 = 150.0;
	l3 = 83.0;
	l4 = 67.0;
	ang1 = 0.0;  //GRADOS//
	ang2 = 0.0;
	ang3 = 0.0;
	ang4 = 0.0;
  a1=0;       //RADIANES//
  a2=0;
  a3=0;
  a4=0;
}


void CInversa::calcular_angulos(float xi, float yi, float zi) {
  x = xi;
  y = yi;
  z = zi;

  calcular_ang1();
  calcular_alfa_beta_omega();
  calcular_ang2();
  calcular_ang3();
  calcular_ang4();

}


void CInversa::calcular_ang1() {
  a1 = atan2(x,y);
  ang1 = a1*180.0/PI;
}

void CInversa::calcular_alfa_beta_omega() {
  alfa = sqrt(pow(x,2)+pow(y,2));
  beta = z-l1-l4*sin(a1);
  omega=(pow(alfa,2)+pow(beta,2)-pow(l2,2)-pow(l3,2))/(2*l2*l3);
}


void CInversa::calcular_ang2() {
  a2 = atan2(beta,alfa)-atan2((sqrt(1-pow(omega,2))),omega);
  ang2 = a2*180.0/PI;
}


void CInversa::calcular_ang3() {
  double a3;
  a3 = atan2((sqrt(1-pow(omega,2))),omega);
  ang3 = a3*180.0/PI;
}


void CInversa::calcular_ang4() {
  double a4;
  a4 = a1-a2-a3;
  ang4 = a4*180.0/PI;
}



void CInversa::calcular_cdirecta(){
  x=l4 * cos(a1) + l3 * cos(a1 + a2) + l2 * cos(a1 + a2 + a3);
 y = l4 * sin(a1) + l3 * sin(a1 + a2) + l2 * sin(a1 + a2 + a3);
 z = l1 + l4 * sin(a1) + l3 * sin(a1 + a2) + l2 * sin(a1 + a2 + a3);
}

void CInversa::set_angulos(float angulo1,float angulo2,float angulo3,float angulo4)
{
  ang1=angulo1;
  ang2=angulo2;
  ang3=angulo3;
  ang4=angulo4;
  
  a1=ang1*180/3.15;
  a2=ang2*180/3.15;
  a3=ang3*180/3.15;
  a4=ang4*180/3.15;    
}

/*
void CInversa::calcular_angulos(float xi, float yi, float zi) {
	x = xi;
	y = yi;
	z = zi;
  calcular_theta();
	calcular_ang1();
	calcular_ang2();
	calcular_ang3();
	calcular_ang4()

}

void CInversa::calcular_theta() {
	theta = ang2 + ang3 + ang4;
}

void CInversa::calcular_ang1() {
	double a1;
	a1 = atan(x/y);
	ang1 = a1*180.0/PI;
}

void CInversa::calcular_ang3() {
	double a3;
	a3 = atan(/*+-(sqrt(1 - cos(ang3) * cos(ang3)) / cos(ang3)));
	ang3 = a3*180.0/PI;
}

void CInversa::calcular_ang2() {
	double a2, beta, alfa;
	beta = atan((l2 + (l3 * cos(ang3) * cos(ang3))) / sqrt(pow((l3 * sin(ang3)), 2) + pow((l2 + (l3 * cos(ang3))), 2) - pow((z - l1 - (l4 * sin(theta))), 2))); //mas o menos en el denominador
	alfa = atan((l3 * sin(ang3)) / (l2 + (l3 * cos(ang3))));
	a2 = beta - alfa;
	ang2 = a2*180.0/PI;
}

void CInversa::calcular_ang4() {
	double a4;
	a4 = theta - ang3 - ang2;
	ang4 = a4*180.0/PI;
}*/
