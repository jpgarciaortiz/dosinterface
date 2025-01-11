#ifndef __PRACTICA3_EJERCICIOS__H
#define __PRACTICA3_EJERCICIOS__H

#include <stdio.h>
#include "window.h"

/******* Archivo con los prototipos de las funciones de la practica 3 *********/

struct datos {
	int k;    	// Numero de iteracion
	int x;      // Coordenada en x del punto
	int y;      // Coordenada en y del punto
	int p;      // Factor de decision
	int x_sig;  // Coordenada en x siguiente
	int y_sig;  // Coordenada en y siguiente
};

/******************* Pinta una linea por el metodo DDA ************************/
void DDA(int x1, int y1, int x2, int y2, int color);

/*********** Pinta una linea mediante el algoritmo de Bresenham ***************/
void Bresenham(int x1, int y1, int x2, int y2, int color);

/* Pinta una linea mediante el algoritmo de Bresenham y guarda su tabla en un archivo */
bool Bresenham_file(int x1, int y1, int x2, int y2, int color, char *nombre);

/* Pinta los puntos simetricos respecto a los ejes 'x' e 'y' de un punto dado */
void pintar_puntos(int xCentro, int yCentro, int x, int y, int color);

/******* Pinta una circunferencia mediante el algoritmo del punto medio *******/
void Punto_medio(int xCentro, int yCentro, int radio, int color);

/* Pinta una circunferencia mediante el algoritmo del punto medio y guarda su tabla en un archivo */
bool Punto_medio_file(int xCentro, int yCentro, int radio, int color, char *nombre);

/************** Pinta una linea, leida sus datos de un archivo ****************/
bool pintar_linea(char *nombre, int color);

/********* Pinta una circunferencia, leidos sus datos de un archivo ***********/
bool pintar_circunferencia(char *nombre, int color);

/******************************* Ejercicio 2 **********************************/
void P3Ejercicio2();

/******************************* Ejercicio 4 **********************************/
void P3Ejercicio4();

/******************************* Ejercicio 5 **********************************/
void P3Ejercicio5();

/******************************* Ejercicio 7 **********************************/
void P3Ejercicio7();

#endif



