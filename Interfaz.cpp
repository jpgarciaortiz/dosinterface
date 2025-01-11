#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "graphix.h"
#include "window.h"
#include "controls.h"
#include "p1ejers.h"
#include "p3ejers.h"
#include "images.h"
#include "cursors.h"

/******************************************************************************/
/** Circulo de Bresenham empleado internamente (a parte del de la practica 3) */
/******************************************************************************/
void pintarpuntos(int xCentro, int yCentro, int x, int y)
{
	line((int)xCentro + x, (int)yCentro + y, (int)xCentro + x, (int)yCentro + y);
	line((int)xCentro - x, (int)yCentro + y, (int)xCentro - x, (int)yCentro + y);
	line((int)xCentro + x, (int)yCentro - y, (int)xCentro + x, (int)yCentro - y);
	line((int)xCentro - x, (int)yCentro - y, (int)xCentro - x, (int)yCentro - y);
	line((int)xCentro + y, (int)yCentro + x, (int)xCentro + y, (int)yCentro + x);
	line((int)xCentro - y, (int)yCentro + x, (int)xCentro - y, (int)yCentro + x);
	line((int)xCentro + y, (int)yCentro - x, (int)xCentro + y, (int)yCentro - x);
	line((int)xCentro - y, (int)yCentro - x, (int)xCentro - y, (int)yCentro - x);
}

void Puntomedio(int xCentro, int yCentro, int radio)
{
	int x, y;	// Coordenadas del punto
	int p;		// Factor de decision

	x = 0;
	y = radio;
	pintarpuntos(xCentro, yCentro, x, y);
	p = 1 - radio;
	while(x <= y) {
		if(p < 0) x++;
		else { x++; y--; 	}
		if(p < 0) p = p + 2 * x + 1;
		else p = p + 2 * (x - y) + 1;
		pintarpuntos(xCentro, yCentro, x, y);
	}
}

/******************************************************************************/
/*************** Ventana para la entrada de los archivos **********************/
/******************************************************************************/
class ArchivoWin : public Window{
  private:
	 Entry *entry;
	 Window *win;
	 windowMethod func;

  public:
	 char *fileName;

	 void hide() {
		fileName = entry->getText();
		Window::hide();
      application->setFocus(NULL);
		((*win).*func)();
	 }

	 WINDOW_BUILDER(ArchivoWin){
		NEW_CONTROL(new Label(30, 15, "Archivo:", BLACK));
		NEW_CONTROL(new CommandButton(Rect(100, 40, 102, 22), " Aceptar", (windowMethod)ArchivoWin::hide, &aceptar));
		NEW_CONTROL(CONTROL(entry) = new Entry(Rect(100, 10, 150, 15), ""));
	 }

	 ArchivoWin(Window *w, windowMethod f) :Window("", Rect(170, 190, 300, 92), W_TITLE | W_CLOSEBUTTON | W_MODAL){
		BUILD(ArchivoWin);
		win = w;
		func = f;
	 }

	 void execute(char *cad) {
		setTitle(cad);
		show();
      application->setFocus(entry);
	 }
};

/******************************************************************************/
/************************* Ventana acerca de... *******************************/
/******************************************************************************/
class AcercaDeWin :public Window {
  public:
	 WINDOW_BUILDER(AcercaDeWin) {
		NEW_CONTROL(new Label(15, 10, "Interfaz grafica 1.0", RED, TRIPLEX_FONT, 1, 2, 1, 1));
		NEW_CONTROL(new Label(15, 55, "Hecha por:", BLACK));
		NEW_CONTROL(new Label(20, 70, "Juan Pablo Garcia Ortiz", BLACK));
		NEW_CONTROL(new Label(20, 85, "Jose Domingo Alvarez Hervas", BLACK));
		NEW_CONTROL(new CommandButton(Rect(80, 110, 102, 22), " Aceptar", (windowMethod)AcercaDeWin::hide, &aceptar));
	 }

	 AcercaDeWin() :Window("Acerca de...", Rect(190, 155, 260, 170), W_TITLE | W_CLOSEBUTTON | W_MODAL) {
		BUILD(AcercaDeWin);
	 }
};

/******************************************************************************/
/***************************** Ventana Mensaje ********************************/
/******************************************************************************/
class MensajeWin: public Window {
  private:
	 Label *l;
	 CommandButton *b;

  public:
	 WINDOW_BUILDER(MensajeWin) {
		NEW_CONTROL(CONTROL(l) = new Label(10, 10, "", BLACK));
		NEW_CONTROL(CONTROL(b) = new CommandButton(Rect(15, 30, 102, 22), " Aceptar", (windowMethod)MensajeWin::hide, &aceptar));
	 }

	 MensajeWin() :Window("Atencion", Rect(190, 155, 260, 85), W_TITLE | W_CLOSEBUTTON | W_MODAL) {
		BUILD(MensajeWin);
	 }

	 void execute(char *cad) {
		l->setCaption(cad);
		setDimension(textwidth(cad) + 25, getRect().height());
		b->setPosition(Point((getClientRect().width() - b->getRect().width()) / 2, b->getRect().y1()));
		setPosition(Point((getmaxx() - getRect().width()) / 2, (getmaxy() - getRect().height()) / 2));
		show();
	 }
};

/******************************************************************************/
/*************************** Ventana de Salida ********************************/
/******************************************************************************/
class SalidaWin: public Window {
  private:
	 GraphBuffer *gbexit;

  public:
	 void onaceptar() { exit(0); }

	 void hide() {
		Window::hide();
		application->restoreScreen(gbexit);
      delete gbexit;
	 }

	 WINDOW_BUILDER(SalidaWin) {
		NEW_CONTROL(new Label(10, 10, "Esta seguro que desea salir del programa ?", BLACK));
		NEW_CONTROL(new CommandButton(Rect(60, 30, 102, 22), " Aceptar", (windowMethod)SalidaWin::onaceptar, &aceptar));
		NEW_CONTROL(new CommandButton(Rect(195, 30, 102, 22), " Cancelar", (windowMethod)SalidaWin::hide, &borrar_todo));
	 }

	 SalidaWin() :Window("Salir", Rect(137, 200, 366, 80), W_TITLE | W_CLOSEBUTTON | W_MODAL) {
		BUILD(SalidaWin);
	 }

	 void paint() {
		application->destroyViewPort();
		setcolor(BLACK);
		application->getMouse().hide();
		for(int i = 0; i < getmaxx(); i += 2)
		  line(i, 0, i, getmaxy());
		for(int j = 0; j < getmaxy(); j += 2)
		  line(0, j, getmaxx(), j);
		application->getMouse().show();
		Window::paint();
	 }

	 void execute() {
		gbexit = application->saveScreen(Rect(Point(0, 0), Point(getmaxx(), getmaxy())));
		setPosition(Point((getmaxx() - getRect().width()) / 2, (getmaxy() - getRect().height()) / 2));
		show();
	 }
};

/******************************************************************************/
/*************************** Ventana principal ********************************/
/******************************************************************************/
class MainWin: public Window, public MouseHandler {
  public:
	 Button *bdibujo, *blinea,
			  *bcirc,   *brect,
			  *bcirc_r, *brect_r,
			  *bpoli,   *brelle,
			  *bgoma,   *bborrar;
	 PaintPanel *panel;
	 ColorsPanel *colPanel;
	 StatusBar *sb;

	 ArchivoWin *aw;
	 AcercaDeWin *adw;
	 MensajeWin *mw;
    SalidaWin *exitw;

	 char cad[20], msg[100];
	 char *actHint;
	 bool pressed;
	 enum { BEGINPOLI, DOPOLI, ENDPOLI } dopoli;
	 enum { NOEJERC, DDA_LINE, BRESE_LINE, BRESE_LINE_FILE,
	        BRESE_CIRC, BRESE_CIRC_FILE, BRESE_LINE_READ, BRESE_CIRC_READ } ejerc;
	 int lineSize;
	 Point iniPos, antPos;

	 Rect antRect;
    GraphBuffer *gb;

	 /****************************** Manejo del raton **************************/
	 void onMouseEnter() {
		if(bgoma->isActivated()) application->getMouse().setCursor(crgoma);
		else if(bdibujo->isActivated()) application->getMouse().setCursor(crdibujo);
		else if(brelle->isActivated()) application->getMouse().setCursor(crrelleno);
      else application->getMouse().setCursor(crlinea);
	 }

	 void onMouseDown(Point& p) {
		pressed = TRUE;

		if(bdibujo->isActivated()) {
		  application->getMouse().hide();
		  setcolor(colPanel->getColor());
		  setlinestyle(SOLID_LINE, 0, lineSize);
		  line(p.x, p.y, p.x, p.y);
		  application->getMouse().show();
		  iniPos.assign(p);
		} else if(blinea->isActivated() || bcirc->isActivated() || bcirc_r->isActivated() ||
					 brect->isActivated() || brect_r->isActivated() || bpoli->isActivated()) {
		  if(bpoli->isActivated() && dopoli == DOPOLI) {
			 setcolor(colPanel->getColor());
			 setlinestyle(SOLID_LINE, 0, lineSize);
			 application->getMouse().hide();
			 if(application->getMouse().leftPressed) {
				line(antPos, p);
				antPos.assign(p);
			 } else {
				line(iniPos, antPos);
				dopoli = ENDPOLI;
			 }
			 application->getMouse().show();
			 setlinestyle(SOLID_LINE, 0, 1);
		  } else {
			 if(bpoli->isActivated()) dopoli = BEGINPOLI;
			 iniPos.assign(p);
			 antPos.assign(iniPos);
			 setcolor(~colPanel->getColor());
			 setlinestyle(SOLID_LINE, 0, lineSize);
			 setwritemode(XOR_PUT);
		  }
		} else if(brelle->isActivated()) {
		  setfillstyle(SOLID_FILL, colPanel->getBgColor());
		  application->getMouse().hide();
		  floodfill(p.x, p.y, colPanel->getColor());
		  application->getMouse().show();
		} else if(bgoma->isActivated()) {
		  setwritemode(XOR_PUT);
		  setcolor(WHITE);
        setfillstyle(SOLID_FILL, colPanel->getBgColor());
		  application->getMouse().hide();
		  bar(p.x - (lineSize * 4), p.y - (lineSize * 4), p.x + (lineSize * 4), p.y + (lineSize * 4));
		  rectangle(p.x - (lineSize * 4), p.y - (lineSize * 4), p.x + (lineSize * 4), p.y + (lineSize * 4));
        application->getMouse().show();
		  antPos.assign(p);
		}
	 }

	 void onMouseMove(Point& p) {
		setcolor(LIGHTGRAY);
		sb->setCaption(cad, 0);
		sprintf(cad, "x:%d y:%d", p.x, p.y);
		setcolor(BLACK);
		sb->setCaption(cad, 0);

		if(pressed) {
		  if(bdibujo->isActivated()) setcolor(colPanel->getColor());
		  else if(bgoma->isActivated()) setcolor(WHITE);
        else setcolor(~colPanel->getColor());

		  if(bdibujo->isActivated()) {
			 application->getMouse().hide();
			 line(iniPos, p);
			 application->getMouse().show();
			 iniPos.assign(p);
		  } else if(blinea->isActivated() || brect->isActivated() || brect_r->isActivated() || (bpoli->isActivated() && dopoli == BEGINPOLI)) {
			 application->getMouse().hide();
			 if(! antPos.isEqualTo(iniPos))
				if(blinea->isActivated() || bpoli->isActivated()) line(iniPos, antPos);
				else rectangle(Rect(iniPos, antPos));
			 if(blinea->isActivated() || bpoli->isActivated()) line(iniPos, p);
			 else rectangle(Rect(iniPos, p));
			 application->getMouse().show();
			 antPos.assign(p);
		  } else if(bcirc->isActivated() || bcirc_r->isActivated()) {
			 application->getMouse().hide();
			 if(! antPos.isEqualTo(iniPos))
				Puntomedio(iniPos.x, iniPos.y, abs(iniPos.x - antPos.x));
			 Puntomedio(iniPos.x, iniPos.y, abs(iniPos.x - p.x));
			 application->getMouse().show();
			 antPos.assign(p);
		  } else if(bgoma->isActivated()) {
			 application->getMouse().hide();
			 rectangle(antPos.x - (lineSize * 4), antPos.y - (lineSize * 4), antPos.x + (lineSize * 4), antPos.y + (lineSize * 4));
			 bar(p.x - (lineSize * 4), p.y - (lineSize * 4), p.x + (lineSize * 4), p.y + (lineSize * 4));
			 rectangle(p.x - (lineSize * 4), p.y - (lineSize * 4), p.x + (lineSize * 4), p.y + (lineSize * 4));
			 application->getMouse().show();
			 antPos.assign(p);
		  }
		}
	 }

	 void onMouseUp(Point& p) {
		pressed = FALSE;
		if(bdibujo->isActivated()) {
        line(iniPos, p); 
		  setlinestyle(SOLID_LINE, 0, 1);
		} else if(blinea->isActivated() || brect->isActivated() || brect_r->isActivated() || (bpoli->isActivated() && dopoli == BEGINPOLI)) {
		  if(bpoli->isActivated()) dopoli = DOPOLI;
		  application->getMouse().hide();
		  if(blinea->isActivated() || bpoli->isActivated()) line(iniPos, antPos);
		  else rectangle(Rect(iniPos, antPos));
		  setwritemode(COPY_PUT);
		  setcolor(colPanel->getColor());
		  if(blinea->isActivated() || bpoli->isActivated()) {
			 if(blinea->isActivated() && ejerc == DDA_LINE)
				DDA(iniPos.x, iniPos.y, p.x, p.y, colPanel->getColor());
			 else if(blinea->isActivated() && ejerc == BRESE_LINE)
				Bresenham(iniPos.x, iniPos.y, p.x, p.y, colPanel->getColor());
			 else if(blinea->isActivated() && ejerc == BRESE_LINE_FILE) {
				if(! Bresenham_file(iniPos.x, iniPos.y, p.x, p.y, colPanel->getColor(), aw->fileName)) {
				  sprintf(msg, "Imposible escribir linea en el archivo '%s'", aw->fileName);
				  mw->execute(msg);
				}
			 } else
            line(iniPos, p);
		  } else {
			 if(brect_r->isActivated()) {
				setfillstyle(SOLID_FILL, colPanel->getBgColor());
				bar(Rect(iniPos, p));
			 }
			 rectangle(Rect(iniPos, p));
		  }
		  setlinestyle(SOLID_LINE, 0, 1);
		  application->getMouse().show();
		} else if(bcirc->isActivated() || bcirc_r->isActivated()) {
		  application->getMouse().hide();
		  Puntomedio(iniPos.x, iniPos.y, abs(iniPos.x - antPos.x));
		  setwritemode(COPY_PUT);
		  setcolor(colPanel->getColor());
		  if(bcirc->isActivated() && ejerc == BRESE_CIRC)
			 Punto_medio(iniPos.x, iniPos.y, abs(iniPos.x - antPos.x), colPanel->getColor());
		  else if(bcirc->isActivated() && ejerc == BRESE_CIRC_FILE) {
			 if(! Punto_medio_file(iniPos.x, iniPos.y, abs(iniPos.x - antPos.x), colPanel->getColor(), aw->fileName)) {
				sprintf(msg, "Imposible escribir circulo en el archivo '%s'", aw->fileName);
				mw->execute(msg);
			 }
		  } else {
			 if(bcirc_r->isActivated()) {
				setfillstyle(SOLID_FILL, colPanel->getBgColor());
				fillellipse(iniPos.x, iniPos.y, abs(iniPos.x - p.x), abs(iniPos.x - p.x));
			 }
			 ellipse(iniPos.x, iniPos.y, 0, 360, abs(iniPos.x - p.x), abs(iniPos.x - p.x));
			 setlinestyle(SOLID_LINE, 0, 1);
		  }
		  application->getMouse().show();
		} else if(bgoma->isActivated()) {
		  setwritemode(COPY_PUT);
		  application->getMouse().hide();
		  bar(antPos.x - (lineSize * 4), antPos.y - (lineSize * 4), antPos.x + (lineSize * 4), antPos.y + (lineSize * 4));
		  application->getMouse().show();
		}
	 }

	 void onMouseExit() {
		actHint = cad;
		hideHint();
		application->getMouse().reset();
	 }

	 /************************** Respuestas a clicks ***************************/
	 void doAcercaDe() {
		actHint = "Acerca de...";
		hideHint();
		adw->show();
	 }

	 void doArchivoAceptar() {
		bool ok;

		if(ejerc == BRESE_LINE_READ || ejerc == BRESE_CIRC_READ) {
		  panel->setViewPort();
		  ok = ((ejerc == BRESE_LINE_READ) ? pintar_linea(aw->fileName, colPanel->getColor()) :
														 pintar_circunferencia(aw->fileName, colPanel->getColor()));

		  if(! ok) {
			 sprintf(msg, "No se ha podido abrir el archivo '%s'", aw->fileName);
			 mw->execute(msg);
		  }
		}
	 }

	 void doBorrarTodo() {
		setfillstyle(SOLID_FILL, WHITE);
		application->getMouse().hide();
		bar(panel->getRect().x1() + 1, panel->getRect().y1() + 1, panel->getRect().x2() - 1, panel->getRect().y2() - 1);
		application->getMouse().show();
	 }

	 void doNada() { }
	 void doSetSizeLine1() { lineSize = 1; }
	 void doSetSizeLine3() { lineSize = 3; }

	 void dolinea() {
		blinea->setHint("Linea");
		ejerc = NOEJERC;
	 }

	 void docirc() {
		bcirc->setHint("Circulo");
		ejerc = NOEJERC;
	 }

	 /********************* Funciones para los ejercicios  *********************/
	 void inicioEjercicio() {
		panel->setViewPort();
		setfillstyle(SOLID_FILL, WHITE);
		application->getMouse().hide();
		bar(0, 0, application->getViewRect().width(), application->getViewRect().height());
		setcolor(BLACK);
	 }

	 void finEjercicio() {
		application->destroyViewPort();
		application->getMouse().show();
	 }

	 /************************ Ejercicios de la practica 1 *********************/
	 void doP1Ejercicio1() { inicioEjercicio(); P1Ejercicio1(); finEjercicio(); }
	 void doP1Ejercicio2() { inicioEjercicio(); P1Ejercicio2(); finEjercicio(); }
	 void doP1Ejercicio3() { inicioEjercicio(); P1Ejercicio3(); finEjercicio(); }
	 void doP1Ejercicio4() { inicioEjercicio(); P1Ejercicio4(); finEjercicio(); }
	 void doP1Ejercicio5() { inicioEjercicio(); P1Ejercicio5(); finEjercicio(); }
	 void doP1Ejercicio6() { inicioEjercicio(); P1Ejercicio6(); finEjercicio(); }
	 void doP1Ejercicio7() { inicioEjercicio(); P1Ejercicio7(); finEjercicio(); }
	 void doP1Ejercicio8() { inicioEjercicio(); P1Ejercicio8(); finEjercicio(); }
	 void doP1Ejercicio9() { inicioEjercicio(); P1Ejercicio9(); finEjercicio(); }
	 void doP1Ejercicio10() { inicioEjercicio(); P1Ejercicio10(); finEjercicio(); }

	 /************************ Ejercicios de la practica 3 *********************/
	 void doP3Ejercicio1() { application->setViewPort(getClientRect()); blinea->onMouseDown(Point(0, 0)); ejerc = DDA_LINE; blinea->setHint("Linea DDA"); }
	 void doP3Ejercicio2() { inicioEjercicio(); P3Ejercicio2(); finEjercicio(); }
	 void doP3Ejercicio3() { application->setViewPort(getClientRect()); blinea->onMouseDown(Point(0, 0)); ejerc = BRESE_LINE; blinea->setHint("Linea Bresenham"); }
	 void doP3Ejercicio4() { inicioEjercicio(); P3Ejercicio4(); finEjercicio(); }
	 void doP3Ejercicio5() { inicioEjercicio(); P3Ejercicio5(); finEjercicio(); }
	 void doP3Ejercicio6() { aw->execute("Archivo BHL a guardar"); application->setViewPort(getClientRect()); blinea->onMouseDown(Point(0, 0)); ejerc = BRESE_LINE_FILE; blinea->setHint("Linea Bresenham (Guardar)"); }
	 void doP3Ejercicio7() { application->setViewPort(getClientRect()); bcirc->onMouseDown(Point(0, 0)); ejerc = BRESE_CIRC; bcirc->setHint("Circulo Bresenham"); }
	 void doP3Ejercicio8() { aw->execute("Archivo BHC a guardar"); application->setViewPort(getClientRect()); bcirc->onMouseDown(Point(0, 0)); ejerc = BRESE_CIRC_FILE; bcirc->setHint("Circulo Bresenham (Guardar)"); }
	 void doP3Ejercicio9() { ejerc = BRESE_LINE_READ; aw->execute("Archivo BHL a leer"); }
	 void doP3Ejercicio10() { ejerc = BRESE_CIRC_READ; aw->execute("Archivo BHC a leer"); }

	 /************************ Definicion de la ventana ************************/
	 WINDOW_BUILDER(MainWin) {
		NEW_CONTROL(new ButtonMenu(Rect(0, 0, 90, 16), "Practica 1", "Ejercicios de la practica 1",
									  new MenuItem("Ejercicio 10", (windowMethod)MainWin::doP1Ejercicio10, "Ejercicio 10"),
									  new MenuItem("Ejercicio 9", (windowMethod)MainWin::doP1Ejercicio9, "Ejercicio 9"),
									  new MenuItem("Ejercicio 8", (windowMethod)MainWin::doP1Ejercicio8, "Ejercicio 8"),
									  new MenuItem("Ejercicio 7", (windowMethod)MainWin::doP1Ejercicio7, "Ejercicio 7"),
									  new MenuItem("Ejercicio 6", (windowMethod)MainWin::doP1Ejercicio6, "Ejercicio 6"),
									  new MenuItem("Ejercicio 5", (windowMethod)MainWin::doP1Ejercicio5, "Ejercicio 5"),
									  new MenuItem("Ejercicio 4", (windowMethod)MainWin::doP1Ejercicio4, "Ejercicio 4"),
									  new MenuItem("Ejercicio 3", (windowMethod)MainWin::doP1Ejercicio3, "Ejercicio 3"),
									  new MenuItem("Ejercicio 2", (windowMethod)MainWin::doP1Ejercicio2, "Ejercicio 2"),
									  new MenuItem("Ejercicio 1", (windowMethod)MainWin::doP1Ejercicio1, "Ejercicio 1"),
									  NULL));

		NEW_CONTROL(new ButtonMenu(Rect(90, 0, 90, 16), "Practica 3", "Ejercicios de la practica 3",
									  new MenuItem("Ejercicio 10", (windowMethod)MainWin::doP3Ejercicio10, "Cargar archivo BHC"),
									  new MenuItem("Ejercicio 9", (windowMethod)MainWin::doP3Ejercicio9, "Cargar archivo BHL"),
									  new MenuItem("Ejercicio 8", (windowMethod)MainWin::doP3Ejercicio8, "Guardar archivo BHC"),
									  new MenuItem("Ejercicio 7", (windowMethod)MainWin::doP3Ejercicio7, "Circulo Bresenham"),
									  new MenuItem("Ejercicio 6", (windowMethod)MainWin::doP3Ejercicio6, "Guardar archivo BHL"),
									  new MenuItem("Ejercicio 5", (windowMethod)MainWin::doP3Ejercicio5, "DDA vs. Bresenham"),
									  new MenuItem("Ejercicio 4", (windowMethod)MainWin::doP3Ejercicio4, "Test linea Bresenham"),
									  new MenuItem("Ejercicio 3", (windowMethod)MainWin::doP3Ejercicio3, "Linea Bresenham"),
									  new MenuItem("Ejercicio 2", (windowMethod)MainWin::doP3Ejercicio2, "Test linea DDA"),
									  new MenuItem("Ejercicio 1", (windowMethod)MainWin::doP3Ejercicio1, "Linea DDA"),
									  NULL));

		NEW_CONTROL(new Button(Rect(180, 0, 105, 16), "Acerca de...", (windowMethod)MainWin::doAcercaDe, -1, "Acerca de..."));

		NEW_CONTROL(new Line3D(0, 17, 40, L_HORIZONTAL));

		NEW_CONTROL(CONTROL(bdibujo) = new Button(Rect(0, 22, 20, 20), dibujo, (windowMethod)MainWin::doNada, 1, "Dibujo libre"));
		NEW_CONTROL(CONTROL(blinea) = new Button(Rect(20, 22, 20, 20), linea, (windowMethod)MainWin::dolinea, 1, "Linea"));
		NEW_CONTROL(CONTROL(bcirc) = new Button(Rect(0, 42, 20, 20), circulo, (windowMethod)MainWin::docirc, 1, "Circulo"));
		NEW_CONTROL(CONTROL(brect) = new Button(Rect(20, 42, 20, 20), rectangulo, (windowMethod)MainWin::doNada, 1, "Rectangulo"));
		NEW_CONTROL(CONTROL(bcirc_r) = new Button(Rect(0, 62, 20, 20), circulo_relleno, (windowMethod)MainWin::doNada, 1, "Circulo con relleno"));
		NEW_CONTROL(CONTROL(brect_r) = new Button(Rect(20, 62, 20, 20), rectangulo_relleno, (windowMethod)MainWin::doNada, 1, "Rectangulo con relleno"));
		NEW_CONTROL(CONTROL(bpoli) = new Button(Rect(0, 82, 20, 20), poligono, (windowMethod)MainWin::doNada, 1, "Poligono"));
		NEW_CONTROL(CONTROL(brelle) = new Button(Rect(20, 82, 20, 20), relleno, (windowMethod)MainWin::doNada, 1, "Relleno"));
		NEW_CONTROL(CONTROL(bgoma) = new Button(Rect(0, 102, 20, 20), goma, (windowMethod)MainWin::doNada, 1, "Goma de borrar"));
		NEW_CONTROL(CONTROL(bborrar) = new Button(Rect(20, 102, 20, 20), borrar_todo, (windowMethod)MainWin::doBorrarTodo, -1, "Borrar todo"));

		NEW_CONTROL(new Line3D(0, 124, 40, L_HORIZONTAL));

		NEW_CONTROL(new Button(Rect(0, 127, 40, 10), fino, (windowMethod)MainWin::doSetSizeLine1, 0, "Pincel fino (1 pixel)"));
		NEW_CONTROL(new Button(Rect(0, 139, 40, 10), grueso, (windowMethod)MainWin::doSetSizeLine3, 0, "Pincel grueso (3 pixels)"));

		NEW_CONTROL(new Line3D(0, 150, 40, L_HORIZONTAL));

		NEW_CONTROL(CONTROL(colPanel) = new ColorsPanel(Rect(1, 153, 38, 168)));

		NEW_CONTROL(CONTROL(panel) = new PaintPanel(SELF));

		NEW_CONTROL(CONTROL(sb) = new StatusBar(SB_RIGHT, 100, -1));

		CONTROL(aw) = new ArchivoWin(SELF, (windowMethod)MainWin::doArchivoAceptar);
		CONTROL(adw) = new AcercaDeWin();
		CONTROL(mw) = new MensajeWin();
      CONTROL(exitw) = new SalidaWin();
	 }

	 /************************ Constructor/Destructor **************************/
	 MainWin() :Window("Informatica Grafica", Rect(0, 0, 640, 480), W_NORMAL, &pc) {
		BUILD(MainWin);
		actHint = NULL;
		pressed = FALSE;
		lineSize = 1;
		dopoli = ENDPOLI;
		ejerc = NOEJERC;
		strcpy(cad, "");
		gb = NULL;
	 };

	 ~MainWin() {}

	 /********************* Funciones de Window sobrecargadas ******************/
	 void setPosition(Point& p) {
		gb = new GraphBuffer(Rect(getClientRect().x1() + panel->getRect().x1() + 1,
										  getClientRect().y1() + panel->getRect().y1() + 1,
										  antRect.width(), antRect.height()));
		GraphicObject::setPosition(p);
		sb->setCaption("", 1);
		sb->setCaption("", 0);
	 }

	 void setDimension(int w, int h) {
		GraphicObject::setDimension(w, h);
		gb = new GraphBuffer(Rect(getClientRect().x1() + panel->getRect().x1() + 1,
										  getClientRect().y1() + panel->getRect().y1() + 1,
											min(antRect.width(), getClientRect().width() - 45),
											min(antRect.height(), getClientRect().height() - 40)));
		sb->setCaption("", 1);
		sb->setCaption("", 0);
	 }

	 void hide() {
      exitw->execute();
	 }

	 void showHint(char *str) {
		if(str) {
		  setcolor(BLACK);
		  sb->setCaption(str, (str[0] == 'x' ? 0 : 1));
		  actHint = str;
		}
	 }

	 void paint() {
		Window::paint();
		antRect.assign(getClientRect()).dimIncr(-45, -40);
		if(gb) {
		  application->getMouse().hide();
		  gb->setPosition(Point(getClientRect().x1() + panel->getRect().x1() + 1,
										getClientRect().y1() + panel->getRect().y1() + 1));
		  application->getMouse().show();
		  gb->paint();
		  delete gb;
		}
	 }

	 void hideHint() {
		if(actHint) {
		  setcolor(LIGHTGRAY);
		  sb->setCaption(actHint, (actHint[0] == 'x' ? 0 : 1));
		  sb->setCaption("", 1);
		  actHint = NULL;
		}
	 }
};

/******************************************************************************/
/*************************** Programa principal *******************************/
/******************************************************************************/
void main() {
  GraphicApplication g;  // Creamos la 'aplicacion grafica'

  MainWin *mw = new MainWin();  // Creamos la ventana

  mw->minWidth = 400;    // Establecemos la anchura y la altura
  mw->minHeight = 363;   // minima para evitar problemas al redimensionar

  mw->show(); // Mostramos la ventana

  g.run(); // Ejecutamos el administrador de ventanas 
}
