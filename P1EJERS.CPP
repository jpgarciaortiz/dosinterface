#include <graphics.h>
#include <stdio.h> 
#include "window.h"

void P1Ejercicio1() {
	arc(100, 100, 0, 90, 20);
}

void P1Ejercicio2() {
   line(0, 0, application->getViewRect().width() / 2, application->getViewRect().height() / 2);
   lineto(0, application->getViewRect().height() / 2 );
	linerel(application->getViewRect().width() / 2, 0);
}

void P1Ejercicio3() {
	for(int i = 0; i < 5; i++)
		circle(application->getViewRect().width() / 2, application->getViewRect().height() / 2, 10 + (i * 10));
}

void P1Ejercicio4() {
   int forma[12] = {0, 100, 100, 100, 150, 100, 15, 35, 20, 40, 0, 100};

	drawpoly(6, forma);
}

void P1Ejercicio5() {
   setfillstyle(8, GREEN);
   fillellipse(application->getViewRect().width() / 2, application->getViewRect().height() / 2, 100, 50);
}

void P1Ejercicio6() {
	application->setViewPort(Rect(application->getViewRect().x1() + 60,
									      application->getViewRect().x1() + 60, 250, 100));
	rectangle(0, 0, 248, 98);
	outtextxy(0, 0, "Hola mundo.");
}

void P1Ejercicio7() {
   char mensaje[256];

	sprintf(mensaje, "Coodernadas maximas: (%d, %d)", getmaxx(), getmaxy());
	outtextxy(20, 20, mensaje);
}

void P1Ejercicio8() {
   for(int i = 0; i < 8; i++){
      setcolor(1 + i);
      setfillstyle(SOLID_FILL, 1 + i);
		sector(application->getViewRect().width() / 2, application->getViewRect().height() / 2, 0 + (45 * i), 45 + (45 * i), 50, 50);
	}
}

void P1Ejercicio9() {
   setfillstyle(LINE_FILL, RED);
	rectangle(3, 3, 200, 200);
	floodfill(50, 50, BLACK);
}

void P1Ejercicio10() {
	int inicial = 100;
	int radio = 50;

   arc(inicial, inicial, 0, 90, radio);
	line(inicial, inicial - radio, inicial + radio, inicial);
}


