/********** Archivo con la declaracion de las funciones del tema 3 ************/

#include <stdlib.h>
#include <graphics.h>
#include "window.h"
#include "p3ejers.h"

void DDA(int x1, int y1, int x2, int y2, int color)
{
	float dx, dy;   	   // Variaciones en 'x' y en 'y'
	int pasos;		      // Para calcular el incremento
	float incx, incy;    // Incremento en 'x' y en 'y'
	float x, y;		      // Coordenadas del proximo pixel a pintar

	dx = x2 - x1;
	dy = y2 - y1;
	if((abs(x2 - x1)) > (abs(y2 - y1)))
		pasos = abs(x2 - x1);
	else
		pasos = abs(y2 - y1);
	if(pasos){
		incx = dx / pasos;
		incy = dy / pasos;
	}
	x = x1;
	y = y1;
	putpixel((int)x, (int)y, color);
	for(int i = 0; i < pasos; i++){
		x += incx;
		y += incy;
		putpixel((int)x, (int)y, color);
	}
}

void Bresenham(int x1, int y1, int x2, int y2, int color)
{
	int x,y;     // Coordenadas del pixel a pintar
	int p;       // Parametro de decision
	int dx,dy;   // Variaciones en 'x' y en 'y'
	int inc1;    // 2 * (dy - dx)
	int inc2;	 // 2 * dy
	int incy;	 // Incremento de y
	int swap;	 // Intercambia 'x1' y 'x2'
	float m;     // Pendiente de la recta

	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	if((x2 - x1)){// Estos son los casos en los que no sea una l¡nea vertical
		m = (float)((float)(y2 - y1) / (float)(x2 - x1));
		if((m < -1) || (m > 1)){
			p = 2 * dx - dy;
			inc1 = 2 * (dx - dy);
			inc2 = 2 * dx;
			if(y1 > y2){
				swap = y1;
				y1 = y2;
				y2 = swap;
				swap = x1;
				x1 = x2;
				x2 = swap;
			}
			if(x2 > x1)
				incy = 1;
			else
				incy = -1;
			x = x1;
			y = y1;
			putpixel((int)x, (int)y, color);
			while(y <= y2){
				y++;
				if(p >= 0){
					x += incy;
					p += inc1;
				}
				else
					p += inc2;
				putpixel((int)x, (int)y, color);
			}
		}
		else{
			p = 2 * dy - dx;
			inc1 = 2 * (dy - dx);
			inc2 = 2 * dy;
			if(x1 > x2){
				swap = x1;
				x1 = x2;
				x2 = swap;
				swap = y1;
				y1 = y2;
				y2 = swap;
			}
			if(y2 > y1)
				incy = 1;
			else
				incy = -1;
			x = x1;
			y = y1;
			putpixel((int)x, (int)y, color);
			while(x <= x2){
				x++;
				if(p >= 0){
					y += incy;
					p += inc1;
				}
				else
					p += inc2;
				putpixel((int)x, (int)y, color);
			}// Fin del while
		}// Fin del else
	}// Fin del if
	else{// Aqui pinto las lineas verticales
		if(y1 > y2){
			swap = y1;
			y1 = y2;
			y2 = swap;
		}
		x = x1;
		y = y1;
		while(y++ <= y2)
			putpixel((int)x, (int)y, color);
	}// Fin del else
}

bool Bresenham_file(int x1, int y1, int x2, int y2, int color, char *nombre)
{
	int x,y;     		// Coordenadas del pixel a pintar
	int p;       		// Parametro de decision
	int dx,dy;   		// Variaciones en 'x' y en 'y'
	int inc1;    		// 2 * (dy - dx)
	int inc2;	 		// 2 * dy
	int incy;	 		// Incremento de y
	int swap;	 		// Intercambia 'x1' y 'x2'
	float m;     		// Pendiente de la recta
	datos registro;   // Aqui metere los valores que guardare en el archivo
	FILE *archivo;    // Archivo donde guardare la tabla

	// Lo primero de todo abro el archivo
	if ((archivo = fopen(nombre, "wb+")) == NULL) return FALSE;

	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	if((x2 - x1)){// Estos son los casos en los que no sea una l¡nea vertical
		m = (float)((float)(y2 - y1) / (float)(x2 - x1));
		if((m < -1) || (m > 1)){
			p = 2 * dx - dy;
			inc1 = 2 * (dx - dy);
			inc2 = 2 * dx;
			if(y1 > y2){
				swap = y1;
				y1 = y2;
				y2 = swap;
				swap = x1;
				x1 = x2;
				x2 = swap;
			}
			if(x2 > x1)
				incy = 1;
			else
				incy = -1;
			x = x1;
			y = y1;
			putpixel((int)x, (int)y, color);
			registro.k = -1;
			fseek(archivo, SEEK_END, 0);
			fprintf(archivo, "   K    X    Y    P    X'   Y'\n");
			while(y <= y2){
				registro.k++;
				registro.x = x;
				registro.y = y;
				registro.p = p;
				y++;
				if(p >= 0){
					x += incy;
					p += inc1;
				}
				else
					p += inc2;
				registro.x_sig = x;
				registro.y_sig = y;
				fprintf(archivo, "%4d %4d %4d %4d %4d %4d\n", registro.k, registro.x, registro.y, registro.p, registro.x_sig, registro.y_sig);
				putpixel((int)x, (int)y, color);
			}
		}
		else{
			p = 2 * dy - dx;
			inc1 = 2 * (dy - dx);
			inc2 = 2 * dy;
			if(x1 > x2){
				swap = x1;
				x1 = x2;
				x2 = swap;
				swap = y1;
				y1 = y2;
				y2 = swap;
			}
			if(y2 > y1)
				incy = 1;
			else
				incy = -1;
			x = x1;
			y = y1;
			putpixel((int)x, (int)y, color);
			registro.k = -1;
			fseek(archivo, SEEK_END, 0);
			fprintf(archivo, "   K    X    Y    P    X'   Y'\n");
			while(x <= x2){
				registro.k++;
				registro.x = x;
				registro.y = y;
				registro.p = p;
				x++;
				if(p >= 0){
					y += incy;
					p += inc1;
				}
				else
					p += inc2;
				registro.x_sig = x;
				registro.y_sig = y;
				fprintf(archivo,"%4d %4d %4d %4d %4d %4d\n", registro.k, registro.x, registro.y, registro.p, registro.x_sig, registro.y_sig);
				putpixel((int)x, (int)y, color);
			}// Fin del while
		}// Fin del else
	}// Fin del if
	else{// Aqui pinto las lineas verticales
		if(y1 > y2){
			swap = y1;
			y1 = y2;
			y2 = swap;
		}
		x = x1;
		y = y1;
		registro.k = -1;
		fseek(archivo, SEEK_END, 0);
		fprintf(archivo, "   K    X    Y    P    X'   Y'\n");
		registro.x = x;
		registro.x_sig = x;
		registro.p = 0;
		while(y <= y2){
			registro.k++;
			registro.y = y;
			y++;
			registro.y_sig = y;
			fprintf(archivo, "%4d %4d %4d %4d %4d %4d\n", registro.k, registro.x, registro.y, registro.p, registro.x_sig, registro.y_sig);
			putpixel((int)x, (int)y, color);
		}
	}// Fin del else

	// Cierro el archivo
	fclose(archivo);
   return TRUE;
}

void pintar_puntos(int xCentro, int yCentro, int x, int y, int color)
{
	putpixel((int)xCentro + x, (int)yCentro + y, color);
	putpixel((int)xCentro - x, (int)yCentro + y, color);
	putpixel((int)xCentro + x, (int)yCentro - y, color);
	putpixel((int)xCentro - x, (int)yCentro - y, color);
	putpixel((int)xCentro + y, (int)yCentro + x, color);
	putpixel((int)xCentro - y, (int)yCentro + x, color);
	putpixel((int)xCentro + y, (int)yCentro - x, color);
	putpixel((int)xCentro - y, (int)yCentro - x, color);
}

void Punto_medio(int xCentro, int yCentro, int radio, int color)
{
	int x, y;	// Coordenadas del punto
	int p;		// Factor de decision

	x = 0;
	y = radio;
	pintar_puntos(xCentro, yCentro, x, y, color);
	p = 1 - radio;
	while(x <= y){
		if(p < 0)
			x++;
		else{
			x++;
			y--;
		}
		/*if(p < 0) // Asi viene en los apuntes
			p = p + 2 * x + 3;
		else
			p = p + (2 * x - 2 * y + 5);*/
		if(p < 0) // Asi viene en el libro
			p = p + 2 * x + 1;
		else
			p = p + 2 * (x - y) + 1;
		pintar_puntos(xCentro, yCentro, x, y, color);
	}// Fin del while
}

bool Punto_medio_file(int xCentro, int yCentro, int radio, int color, char *nombre)
{
	int x, y;	          // Coordenadas del punto
	int p;		          // Factor de decision
	datos registro;       // Aqui metere los valores que guardare en el archivo
	FILE *archivo;			 // Aqui metere los datos de la circunferencia

	// Lo primero de todo abro el archivo
	if ((archivo = fopen(nombre, "wb+")) == NULL) return FALSE;

	x = 0;
	y = radio;
	pintar_puntos(xCentro, yCentro, x, y, color);
	registro.k = -1;
	fseek(archivo, SEEK_END,0);
	fprintf(archivo, "  Xc   Yc\n");
	fprintf(archivo, "%4d %4d\n", xCentro, yCentro);
	fprintf(archivo, "   K    X    Y    P    X'   Y'\n");
	p = 1 - radio;
	while(x <= y){
		registro.k++;
		registro.x = x;
		registro.y = y;
		registro.p = p;
		if(p < 0)
			x++;
		else{
			x++;
			y--;
		}
		if(p < 0)
			p = p + 2 * x + 1;
		else
			p = p + 2 * (x - y) + 1;
		registro.x_sig = x;
		registro.y_sig = y;
		fprintf(archivo,"%4d %4d %4d %4d %4d %4d\n", registro.k, registro.x, registro.y, registro.p, registro.x_sig, registro.y_sig);
		pintar_puntos(xCentro, yCentro, x, y, color);
	}// Fin del while

	// Cierro el archivo
	fclose(archivo);
	return TRUE;
}

bool pintar_linea(char *nombre, int color)
{
	datos registro;				// Para guardar los registros leidos del archivo
	FILE *archivo;			 		// De aqui leere los datos de la linea

	// Lo primero de todo abro el archivo
	if ((archivo = fopen(nombre, "rb")) == NULL) return FALSE;

	while(fgetc(archivo) != '\n');
	while(!feof(archivo)){
		fscanf(archivo,"%d",&registro.k);
		fscanf(archivo,"%d",&registro.x);
		fscanf(archivo,"%d",&registro.y);
		fscanf(archivo,"%d",&registro.p);
		fscanf(archivo,"%d",&registro.x_sig);
		fscanf(archivo,"%d",&registro.y_sig);
		fgetc(archivo);
		putpixel(registro.x,registro.y, color);
	}

	// Ahora cierro el archivo
	fclose(archivo);
   return TRUE;
}

bool pintar_circunferencia(char *nombre, int color)
{
	datos registro;				// Para guardar los registros leidos del archivo
	int xCentro, yCentro;	 	// Puntos del centro de la circunferencia
	FILE *archivo;			 		// De aqui leere los datos de la circunferencia

	// Lo primero de todo abro el archivo
	if ((archivo = fopen(nombre, "rb")) == NULL) return FALSE;

	while(fgetc(archivo) != '\n');
	fscanf(archivo, "%d", &xCentro);
	fscanf(archivo, "%d", &yCentro);
	while(fgetc(archivo) != 'K');
	while(fgetc(archivo) != '\n');
	do{
		fscanf(archivo,"%d", &registro.k);
		fscanf(archivo,"%d", &registro.x);
		fscanf(archivo,"%d", &registro.y);
		fscanf(archivo,"%d", &registro.p);
		fscanf(archivo,"%d", &registro.x_sig);
		fscanf(archivo,"%d", &registro.y_sig);
		pintar_puntos(xCentro, yCentro, registro.x, registro.y, color);
	} while(registro.x_sig <= registro.y_sig);
	fclose(archivo);
   return TRUE;
}

void P3Ejercicio2()
{
	int maxx, maxy;		       	// M´aximos de x y de y
	int posx, posy;               // Punto a pintar
	int i;

	maxx = application->getViewRect().width(); maxy = application->getViewRect().height();
	posx = maxx / 2; posy = maxy / 2;
	for(i = posy; i >= 0; i--) DDA(posx, posy, maxx, i, 1);
	for(i = maxx; i >= 0; i--) DDA(posx, posy, i, 0, 2);
	for(i = 0; i <= maxy; i++) DDA(posx, posy, 0, i, 3);
	for(i = 0; i <= maxx; i++) DDA(posx, posy, i, maxy, 4);
	for(i = maxy; i >= posy; i--) DDA(posx, posy, maxx, i, 1);
}

void P3Ejercicio4()
{
	int maxx, maxy;		       	// M´aximos de x y de y
	int posx, posy;               // Punto a pintar
	int i;

	maxx = application->getViewRect().width(); maxy = application->getViewRect().height();
	posx = maxx / 2; posy = maxy / 2;
	for(i = posy; i >= 0; i--) Bresenham(posx, posy, maxx, i, 1);
	for(i = maxx; i >= posx; i--) Bresenham(posx, posy, i, 0, 2);
	for(i = posx; i >= 0; i--) Bresenham(posx, posy, i, 0, 3);
	for(i = 0; i <= posy; i++) Bresenham(posx, posy, 0, i, 4);
	for(i = posy; i <= maxy; i++) Bresenham(posx, posy, 0, i, 5);
	for(i = 0; i <= posx; i++) Bresenham(posx, posy, i, maxy, 6);
	for(i = posx; i <= maxx; i++) Bresenham(posx, posy, i, maxy, 7);
	for(i = maxy; i >= posy; i--) Bresenham(posx, posy, maxx, i, 8);
}

void P3Ejercicio5()
{
	DDA(0, 0, application->getViewRect().width(), application->getViewRect().height(), 3);
	Bresenham(0, 0, application->getViewRect().width(), application->getViewRect().height(), 4);
}

void P3Ejercicio7()
{
  Punto_medio((application->getViewRect().width() / 2), (application->getViewRect().height() / 2), 50, 10);
}

