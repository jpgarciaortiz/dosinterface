#define __WINDOW_MAIN

#include <alloc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>    //
#include <stdarg.h>
#include "graphix.h"
#include "window.h"


/*--------------------------- Variables globales ----------------------------*/
GraphicApplication *application = NULL;   // Aplicacion actual
Vector windowsList;                       // Vector con las ventanas creadas
int backgroundColor = 3;                  // Color de fondo
char *driverDirectory = "";               // Directorio del driver grafico
Window *winActual = NULL;                 // Ventana actual


/*---------------------------- GraphicObject --------------------------------*/

void GraphicObject::setPosition(Point& p) {
  rect.setPosition(p);
}

void GraphicObject::setDimension(int w, int h) {
  rect.setDimension(w, h);
}


/*----------------------------- GraphBuffer ---------------------------------*/

GraphBuffer::GraphBuffer(Rect& r) :GraphicObject(r) {
  size_t yIncr = r.height() / NUM_MEM_BLOQUES;
  size_t yLast = r.height() % NUM_MEM_BLOQUES;
  size_t size;
  int ystart = r.y1();
  int yend = r.y1() + yIncr - 1;

  application->getMouse().hide();
  for(int i = 0; i < NUM_MEM_BLOQUES; i++) {
	 if(i == NUM_MEM_BLOQUES - 1) yend += yLast;
	 size = imagesize(r.x1(), ystart, r.x2(), yend);
	 if((buff[i] = malloc(size)) == NULL) {
		closegraph();
		printf("Error: Memoria insuficente para crear un buffer grafico.\n");
	 } else {
		getimage(r.x1(), ystart, r.x2(), yend, buff[i]);
		ystart = yend + 1;
		yend += yIncr + 1;
	 }
  }
  application->getMouse().show();
}

void GraphBuffer::setDimension(int, int) {
  // Anulamos la posibilidad de cambiar la dimension del objeto grafico
}

void GraphBuffer::paint() {
  size_t yIncr = rect.height() / NUM_MEM_BLOQUES;
  int ystart = rect.y1();
  int yend = rect.y1() + yIncr - 1;

  application->getMouse().hide();
  for(int i = 0; i < NUM_MEM_BLOQUES; i++) {
    putimage(rect.x1(), ystart, buff[i], COPY_PUT);
    ystart = yend + 1;
    yend += yIncr + 1;
  }
  application->getMouse().show();
}

GraphBuffer::~GraphBuffer() {
  for(int i = 0; i < NUM_MEM_BLOQUES; i++) free(buff[i]);
}


/*------------------------------- Bitmap ------------------------------------*/

Bitmap::Bitmap(int tcol, int width, int height, byte far *pic) :GraphicObject(Rect(0, 0, width, height)) {
  buff = FALSE;
  tcolor = tcol;
  data = pic;
}

Bitmap::Bitmap(int tcol, int width, int height, ...) :GraphicObject(Rect(0, 0, width, height)) {
  va_list p;
  int size = width * height;

  buff = TRUE;
  tcolor = tcol;

  if((data = (byte far *)malloc(size)) != NULL) {
    va_start(p, height);
    for(int i = 0; i < size; i++) data[i] = va_arg(p, int);
  }
}

void Bitmap::setDimension(int, int) {
  // Anulamos la posibilidad de cambiar la dimension del objeto grafico
}

void Bitmap::paint() {
  int i = 0;

  if(data) {
    for(int y = rect.y1(); y <= rect.y2(); y++)
      for(int x = rect.x1(); x <= rect.x2(); x++) {
        if(data[i] != tcolor) putpixel(x, y, data[i]);
        i++;
      }
  }
}

Bitmap::~Bitmap() {
  if(data && buff) farfree(data);
}


/*------------------------------- Window ------------------------------------*/

Window::Window(char *t, Rect& r, int ty, Bitmap *ic) :GraphicObject(r) {
  title = strdup(t);
  type = ty;
  icon = ic;
  gbuff = NULL;
  minWidth = 50;
  minHeight = 50;
}

void Window::addCtrl(Control *c) {
  ctrls.add(c);
  c->parent = this;
}

void Window::removeCtrl(Control *c) {
  ctrls.remove(c);
}

Rect Window::getTitlePanelRect() {
  if(! (type & W_TITLE)) return NULLRECT;
  return Rect(Point(rect.x1() + 4, rect.y1() + 4), Point(rect.x2() - 4, rect.y1() + 17));
}

Rect Window::getResizeCornerRect() {
  if(! (type & W_SIZEABLE)) return NULLRECT;
  return Rect(rect.x2() - 9, rect.y2() - 9, 9, 9);
}

Rect Window::getClientRect() {
  int incr = (type & W_NORMAL) ? 20 : 4;
  return Rect(Point(rect.x1() + 4, rect.y1() + incr), Point(rect.x2() - 4, rect.y2() - 4));
}

Rect Window::getCloseButtonRect() {
  if(! (type & W_CLOSEBUTTON)) return NULLRECT;
  return Rect(rect.x2() - 17, rect.y1() + 5, 12, 12);
}

void draw_button(int, int, int, int, int);

static void doCtrlPaint(void *ctrl) {
  ((Control *)ctrl)->paint();
}

void Window::saveBackground() {
  if(gbuff) delete gbuff;
  gbuff = NULL;
  if(windowsList.count() > 1) gbuff = application->saveScreen(rect);
  else downRect.assign(getRect());
}

void Window::restoreBackground() {
  if(gbuff)
	 application->restoreScreen(gbuff);
  else {
	 setfillstyle(SOLID_FILL, backgroundColor);
	 application->getMouse().hide();
	 bar(downRect);
    application->getMouse().show();
  }
}

void Window::drawTitle(bool active) {
	int x = rect.x1(), y = rect.y1();

	if(type & W_TITLE) {
	  setfillstyle(SOLID_FILL, active ? BLUE : CYAN);
	  bar(getTitlePanelRect());
	  setcolor(WHITE);
	  if(icon) {
		 outtextxy(x + 8 + icon->width(), y + 8, title);
		 icon->setPosition(Point(x + 5, y + 5));
		 icon->paint();
	  } else
		 outtextxy(x + 8, y + 8, title);
	}

	if(type & W_CLOSEBUTTON)
	  drawbutton(getCloseButtonRect(), "x", 1, FALSE);
}

void Window::paint() {
   int x2 = rect.x2(), y2 = rect.y2();

   drawpanel(rect);
	drawTitle(TRUE);

   if(type & W_SIZEABLE) {
     setcolor(DARKGRAY);
     line(x2 - 5, y2 - 4, x2 - 4, y2 - 5);
     line(x2 - 6, y2 - 4, x2 - 4, y2 - 6);
     setcolor(WHITE);
     line(x2 - 7, y2 - 4, x2 - 4, y2 - 7);

     setcolor(DARKGRAY);
     line(x2 - 9, y2 - 4, x2 - 4, y2 - 9);
     line(x2 - 10, y2 - 4, x2 - 4, y2 - 10);
     setcolor(WHITE);
     line(x2 - 11, y2 - 4, x2 - 4, y2 - 11);
   }

   application->setViewPort(getClientRect());
   ctrls.crossFromBegin(doCtrlPaint);
   application->destroyViewPort();
}

void draw_button(int x, int y, int width, int height, int) {
   int x2 = x + width - 1;
   int y2 = y + height - 1;

   setcolor(WHITE);
   line(x, y, x2 - 1, y);
   line(x, y, x, y2 - 1);
   setcolor(BLACK);
   line(x2, y, x2, y2);
   line(x, y2, x2, y2);
   setcolor(DARKGRAY);
   line(x2 - 1, y + 1, x2 - 1, y2 - 1);
   line(x + 1, y2 - 1, x2 - 1, y2 - 1);
   setfillstyle(SOLID_FILL, LIGHTGRAY);
   bar(x + 1, y + 1, x2 - 2, y2 - 2);
}

void Window::show() {
  application->destroyViewPort();
  windowsList.add(this);
  if(windowsList.count() > 1) {
	 application->getMouse().hide();
	 winActual->drawTitle(FALSE);
	 application->getMouse().show();
  }
  saveBackground();
  application->getMouse().hide();
  paint();
  application->getMouse().show();
  winActual = this;
}

void Window::hide() {
  application->destroyViewPort();
  windowsList.remove(this);
  restoreBackground();
  if(windowsList.count()) {
	 winActual = (Window *)windowsList[0];
	 winActual->drawTitle(TRUE);
  }
}

static Window *self;
static bool ok;
static void doWinPaint(void *w) {
  Window *win = (Window *)w;
  if(ok) {
    win->restoreBackground();   //
    self->restoreBackground();  //
    win->saveBackground();      //
    win->paint();
  }
  if(win == self) ok = TRUE;
}

void Window::bringToFront() {
  //restoreBackground();
  if((Window *)windowsList[0] == this) {
	 application->getMouse().hide();
	 paint();
    application->getMouse().show();
  } else {
	 application->getMouse().hide();
    self = this;
    ok = FALSE;
    windowsList.crossFromEnd(doWinPaint);
    application->getMouse().show();
    show();
  }
}

static void doFreeCtrl(void *c) { delete (Control *)c; }

Window::~Window() {
  free(title);
  ctrls.crossFromBegin(doFreeCtrl);
  windowsList.remove(this);
  restoreBackground();
  application->refresh();
}


/*------------------------ GraphicApplication -------------------------------*/

GraphicApplication::GraphicApplication() {
  int gdriver = DETECT, gmode;

  initgraph(&gdriver, &gmode, driverDirectory);
  if(graphresult() != grOk) {
    printf("Error: No es posible activar el modo grafico.\n"
           "       Asegurese de que el controlador .BGI este en el directorio '%s'.\n", driverDirectory);
    exit(1);
  }

  mouse = new Mouse();

  if(! mouse->installed) {
    delete mouse;
    closegraph();
    printf("Error: No hay un raton instalado.\n"
           "       No es posible ejecutar la aplicacion sin este dispositivo.\n");
    exit(1);
  }

  mouse->show();

  maxX = getmaxx();
  maxY = getmaxy();
  actView = new Rect(Point(0, 0), Point(maxX, maxY));
  cFocus = NULL;

  clearBackground();

  application = this;
}

void GraphicApplication::setViewPort(Rect& r) {
  setviewport(r, 1);
  actView->assign(r);
}

void GraphicApplication::destroyViewPort() {
  setviewport(0, 0, maxX, maxY, 1);
  actView->assign(Rect(0, 0, maxX, maxY));
}

GraphBuffer *GraphicApplication::saveScreen(Rect& r) {
  mouse->hide();
  GraphBuffer *gb = new GraphBuffer(r);
  mouse->show();
  return gb;
}

void GraphicApplication::restoreScreen(GraphBuffer *gb) {
  mouse->hide();
  gb->paint();
  mouse->show();
}

void GraphicApplication::clearBackground() {
  setfillstyle(SOLID_FILL, backgroundColor);
  mouse->hide();
  bar(0, 0, maxX, maxY);
  mouse->show();
}

void GraphicApplication::run() {
  Window *wAct;
  Control *cAct, *cAnt = NULL;
  Point pos, antPos(-1, -1);
  Rect rMov;

  while(TRUE) {
    if(kbhit()) {
		char key = getch();
      if(cFocus) cFocus->onKeyPress(key);
    }

    pos.assign(mouse->refresh().pos);

    wAct = NULL;
    for(int i = 0; i < windowsList.count(); i++) {
		Window *wAux = (Window *)windowsList[i];
      if(pos.isIncludedIn(wAux->getRect())) {
        wAct = wAux;
        break;
      }
    }

    cAct = NULL;
    if(wAct) {
      pos.relativeTo(wAct->getClientRect());
      for(int i = 0; i < wAct->ctrlCount(); i++) {
        Control *cAux = (Control *)wAct->ctrl(i);
        if(pos.isIncludedIn(cAux->getRect())) {
          cAct = cAux;
          break;
        }
      }
    }

	 if(!mouse->leftPressed && !mouse->rightPressed) {
		if(cAct && wAct == winActual) {
		  Point auxPos(pos);
		  if(! antPos.isEqualTo(mouse->pos)) {
			 setViewPort(cAct->getParent().getClientRect());
			 cAct->onMouseMove(auxPos.relativeTo(cAct->getRect()));
          destroyViewPort();
		  }
		  antPos.assign(mouse->pos);
		}

		if((winActual == wAct || wAct == NULL) && cAnt != cAct) {
		  if(cAnt) {
			  if(winActual == (&cAnt->getParent())) {
				  setViewPort(cAnt->getParent().getClientRect());
				  cAnt->onMouseExit();
			  }
		  }
        if(cAct) { 
			 setViewPort(cAct->getParent().getClientRect());
          cAct->onMouseEnter();
		  }
		  destroyViewPort();
		  cAnt = cAct;
		}
	 } else {
      if(wAct && wAct != winActual) {
        if(winActual->getType() & W_MODAL) continue;
        winActual = wAct;
        wAct->bringToFront();
      }

		if(mouse->leftPressed && wAct && mouse->pos.isIncludedIn(wAct->getCloseButtonRect())) {
        mouse->hide();
        drawbutton(wAct->getCloseButtonRect(), "x", 1, TRUE);
        mouse->show();
		  while(mouse->refresh().leftPressed);
        mouse->hide();
		  drawbutton(wAct->getCloseButtonRect(), "x", 1, FALSE);
		  mouse->show();
		  wAct->hide();
		} else if(mouse->leftPressed && wAct && mouse->pos.isIncludedIn(wAct->getTitlePanelRect())) {
        rMov.assign(wAct->getRect());
        pos.assign(mouse->pos);
        setwritemode(XOR_PUT);
        setcolor(WHITE);
        mouse->hide();
        rectangle(rMov);
        mouse->show();
        Point ant(mouse->refresh().pos);
        while(mouse->refresh().leftPressed)
          if(! pos.isEqualTo(mouse->pos)) {
            mouse->hide();
            rectangle(rMov);
            rMov.setPosition(
                 Point(max(0, min(maxX - rMov.width() + 1, rMov.x1() + (mouse->pos.x - pos.x))),
                       max(0, min(maxY - rMov.height() + 1, rMov.y1() + (mouse->pos.y - pos.y)))));
            rectangle(rMov);
            mouse->show();
            pos.assign(mouse->pos);
          }
        mouse->hide();
        rectangle(rMov);
        mouse->show();
        setwritemode(COPY_PUT);
		  if(! mouse->pos.isEqualTo(ant)) {
			 wAct->setPosition(Point(rMov.x1(), rMov.y1()));
			 wAct->restoreBackground();
			 wAct->saveBackground();
			 mouse->hide();
			 wAct->paint();
          mouse->show();
        }
      } else if(mouse->leftPressed && wAct && mouse->pos.isIncludedIn(wAct->getResizeCornerRect())) {
        rMov.assign(wAct->getRect());
        pos.assign(mouse->pos);
        setwritemode(XOR_PUT);
        setcolor(WHITE);
        mouse->hide();
        rectangle(rMov);
        mouse->show();
        Point ant(mouse->refresh().pos);
        while(mouse->refresh().leftPressed)
          if(! pos.isEqualTo(mouse->pos)) {
            mouse->hide();
            rectangle(rMov);
				rMov.setDimension(max(wAct->minWidth, min(maxX - rMov.x1() + 1, rMov.width() + (mouse->pos.x - pos.x))),
										max(wAct->minHeight, min(maxY - rMov.y1() + 1, rMov.height() + (mouse->pos.y - pos.y))));
            rectangle(rMov);
            mouse->show();
            pos.assign(mouse->pos);
          }
        mouse->hide();
		  rectangle(rMov);
		  mouse->show();
		  setwritemode(COPY_PUT);
		  if(! mouse->pos.isEqualTo(ant)) {
			 wAct->setDimension(rMov.width(), rMov.height());
			 wAct->restoreBackground();
			 wAct->saveBackground();
			 mouse->hide();
			 wAct->paint();
			 mouse->show();
		  }
		} else {
		  cFocus = cAct;
		  if(cAct) setViewPort(cAct->getParent().getClientRect());
		  if(cAct) cAct->onMouseDown(pos.relativeTo(cAct->getRect()));
		  Point ant(mouse->pos);
		  while(mouse->refresh(), mouse->leftPressed || mouse->rightPressed) {
			 if(! ant.isEqualTo(mouse->pos)) {
				if(cAct) cAct->onMouseMove(pos.assign(mouse->pos).relativeTo(wAct->getClientRect()).relativeTo(cAct->getRect()));
			 }
			 ant.assign(mouse->pos);
		  }
		  if(cAct) cAct->onMouseUp(pos.assign(mouse->pos).relativeTo(wAct->getClientRect()).relativeTo(cAct->getRect()));
		  destroyViewPort();

		}
    }
  }
}

static void doPaint(void *w) {
  Window *win = (Window *)w;
  win->saveBackground();
  win->paint();
}

void GraphicApplication::refresh() {
  winActual = NULL;
  if(windowsList.count()) winActual = (Window *)windowsList[0];
  clearBackground();
  mouse->hide();
  windowsList.crossFromEnd(doPaint);
  mouse->show();
}

GraphicApplication::~GraphicApplication() {
  while(windowsList.count()) delete (Window *)windowsList[0];
  mouse->hide();
  delete mouse;
  closegraph();
  printf("Bye.\n");
}










