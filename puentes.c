#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

#define PI 3.141592654

/**
 *Los puentes se construyen en base al metodo Euler–Maruyama
 *
**/


/*
 *Implementacion de box muller de http://c-faq.com/lib/gaussian.html
 */
double gaussrand()
{
	static double U, V;
	static int phase = 0;
	double Z;

	if(phase == 0) {
		U = (rand() + 1.) / (RAND_MAX + 2.);
		V = rand() / (RAND_MAX + 1.);
		Z = sqrt(-2 * log(U)) * sin(2 * PI * V);
	} else
		Z = sqrt(-2 * log(U)) * cos(2 * PI * V);

	phase = 1 - phase;

	return Z;
}

/*
 * Funcion que genera trayectorias de izquierda a derecha para la creacion de puentes
 * Recibe como parametros, un tiempo inicial y final, un valor inicial y0,
 * un buffer y su longitud, asi como theta, mu y sigma que determinan a la ecuacion 
 * dXt = theta * (mu - Xt) * dt + sigma * dWt
 * donde dWt es la raiz cuadrada del salto multiplicado por un numero generado por box muller
 */
void trayectoriaLR(float t_0, float t_f, float y0, float * buff, int n,float theta, float mu, float sigma){
	float h = (t_f - t_0)/n;
	float sqrtdt = sqrt(h);
	buff[0] = y0;
	int i;	
	
	for(i = 1; i<n ; i++){
		buff[i] = buff[i-1] + h*(theta*(mu-buff[i-1])) + sigma*sqrtdt*gaussrand();
	}
}

/*
 * Lo mismo que la funcion anterior pero ahora de derecha a izquierda
 */
void trayectoriaRL(float t_0, float t_f, float y0, float * buff, int n,float theta, float mu, float sigma){
	float h = (t_f - t_0)/n;
	float sqrtdt = sqrt(h);
	buff[n-1] = y0;
	int i;	
	for(i = n-2; i>=0 ; i--){
		buff[i] = buff[i+1] + h*(theta*(mu-buff[i+1])) + sigma*sqrtdt*gaussrand();
	}
}

/*
 * Funcion que dados dos vectores, genera en otro un vector de signos,
 * el cual representa las distancias entre los puntos.
 * La n denota el tamaño del vector.
 * Devuelve el momento en el cual ocurrio un cruce.
 */
int cruce(float * buff,float * buff1,float * buff2, int n) {
	int i;	
	int position = -1;
	for(i = 0; i<n ; i++){
		if(buff1[i]-buff[i]>=0){
			buff2[i]=1;
		}
		else{
			if(position<0){
				position = i;
			}
			buff2[i]=-1;
		}
	}

	return position;
}


int main(void)
{
	int size = 100;
	float buffer[size];
	float buffer1[size];
	float buffer2[size];
	int i;

	trayectoriaLR(0, 2, 0, buffer, size,1, 1.2, 0.3);

	printf("Imprimiendo trayectoria izquierda derecha...\n");

	for(i = 0; i<size ; i++){
		printf("El valor de i=%d es: %f\n" ,i, buffer[i]);
	}

	trayectoriaRL(0, 2, 0, buffer1, size,1, 1.2, 0.3);

	printf("Imprimiendo trayectoria derecha izquierda...\n");

	for(i = 0; i<size ; i++){
		printf("El valor de i=%d es: %f\n" ,i, buffer1[i]);
	}

	int c = cruce(buffer,buffer1,buffer2,size);

	printf("Imprimiendo signos de las distancias...\n");

	for(i = 0; i<size ; i++){
		printf("El valor de i=%d es: %f\n" ,i, buffer2[i]);
	}

	printf("El primer cruce ocurrio en el momento %d\n", c);

}
