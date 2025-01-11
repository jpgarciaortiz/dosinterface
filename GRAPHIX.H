#ifndef __GRAPHIX__H
#define __GRAPHIX__H

/*-----------------------------------------------------------------------------
   GRAPHIX

   by TRON
  ---------------------------------------------------------------------------*/

#include <graphics.h>
#include "window.h"
#include "types.h"

/* Sobrecargamos algunas funciones de la libreria grafica para adaptarlas a los
  tipos Point y Rect */
inline void arc(Point& p, int a, int b, int c) { arc(p.x, p.y, a, b, c); }
inline void bar(Rect& r) { bar(r.x1(), r.y1(), r.x2(), r.y2()); }
inline void bar3d(Rect& r, int a, int b) { bar3d(r.x1(), r.y1(), r.x2(), r.y2(), a, b); }
inline void circle(Point& p, int r) { circle(p.x, p.y, r); }
inline void ellipse(Point& p, int a, int b, int c, int d) { ellipse(p.x, p.y, a, b, c, d); }
inline void fillellipse(Point& p, int a, int b) { fillellipse(p.x, p.y, a, b); }
inline void floodfill(Point& p, int a) { floodfill(p.x, p.y, a); }
inline void getimage(Rect& r, void far *p) { getimage(r.x1(), r.y1(), r.x2(), r.y2(), p); }
inline Point getmaxpos() { return Point(getmaxx(), getmaxy()); }
inline int getpixel(Point& p) { return getpixel(p.x, p.y); }
inline Point getpos() { return Point(getx(), gety()); }
inline unsigned imagesize(Rect& r) { return imagesize(r.x1(), r.y1(), r.x2(), r.y2()); }
inline void line(Point& a, Point& b) { line(a.x, a.y, b.x, b.y); }
inline void lineto(Point& a) { lineto(a.x, a.y); }
inline void moveto(Point& a) { moveto(a.x, a.y); }
inline void outtextxy(Point& a, char far *msg) { outtextxy(a.x, a.y, msg); }
inline void pieslice(Point& p, int a, int b, int c) { pieslice(p.x, p.y, a, b, c); }
inline void putimage(Point& p, void far *a, int b) { putimage(p.x, p.y, a, b); }
inline void putpixel(Point& p, int c) { putpixel(p.x, p.y, c); }
inline void rectangle(Rect& r) { rectangle(r.x1(), r.y1(), r.x2(), r.y2()); }
inline void sector(Point& p, int a, int b, int c, int d) { sector(p.x, p.y, a, b, c, d); }
inline void setviewport(Rect& r, int c) { setviewport(r.x1(), r.y1(), r.x2(), r.y2(), c); }

/* Funciones del modulo graphix */
void outtextdxy(int, int, char far *);
void horline(int, int, int);
void drawbutton(int, int, int, int, char *, int, bool, int = LIGHTGRAY);
void drawbutton(int, int, int, int, Bitmap&, int, bool, int = LIGHTGRAY);
void drawpanel(int, int, int, int);

/* Y sobrecargamos tambien estas funciones */
inline void outtextdxy(Point& p, char far *msg) { outtextdxy(p.x, p.y, msg); }
inline void horline(Point& p, int width) { horline(p.x, p.y, width); }
inline void drawbutton(Rect& r, char *title, int grosor, bool push, int color = LIGHTGRAY) { drawbutton(r.x1(), r.y1(), r.width(), r.height(), title, grosor, push, color); }
inline void drawbutton(Rect& r, Bitmap& dib, int grosor, bool push, int color = LIGHTGRAY) { drawbutton(r.x1(), r.y1(), r.width(), r.height(), dib, grosor, push, color); }
inline void drawpanel(Rect& r) { drawpanel(r.x1(), r.y1(), r.width(), r.height()); }

#endif

