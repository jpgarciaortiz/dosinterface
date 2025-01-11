#include <dos.h>
#include "mouse.h"

Mouse::Mouse() {
  REGS ir, or;
  SREGS sr;

  ir.x.ax = 0;
  int86x(0x33, &ir, &or, &sr);

  Mouse::numButtons = or.x.bx;
  Mouse::installed = or.x.ax;
}


Mouse& Mouse::reset() {
  REGS ir, or;
  SREGS sr;
  Point aux(refresh().pos);

  ir.x.ax = 0;
  int86x(0x33, &ir, &or, &sr);
  setPosition(aux);
  show();

  return *this;
}

Mouse& Mouse::show() {
  REGS ir, or;
  SREGS sr;

  ir.x.ax = 1;
  int86x(0x33, &ir, &or, &sr);
  return *this;
}

Mouse& Mouse::hide() {
  REGS ir, or;
  SREGS sr;

  ir.x.ax = 2;
  int86x(0x33, &ir, &or, &sr);
  return *this;
}

Mouse& Mouse::refresh() {
  REGS ir, or;
  SREGS sr;

  ir.x.ax = 3;
  int86x(0x33, &ir, &or, &sr);
  pos.x = or.x.cx;
  pos.y = or.x.dx;
  leftPressed = (or.x.bx & 1);
  rightPressed = (or.x.bx & 2) >> 1;

  return *this;
}

Mouse& Mouse::setCursor(Cursor& c) {
  REGS ir, or;
  SREGS sr;

  cursor = &c;

  ir.x.ax = 9;
  ir.x.bx = c.hotx;
  ir.x.cx = c.hoty;
  ir.x.dx = FP_OFF(c.mask);
  sr.es = FP_SEG(c.mask);

  int86x(0x33, &ir, &or, &sr);  

  return *this;
}


Mouse& Mouse::setPosition(Point& p) {
  REGS ir, or;
  SREGS sr;

  ir.x.ax = 4;
  ir.x.cx = p.x;
  ir.x.dx = p.y;
  
  int86x(0x33, &ir, &or, &sr); 

  return *this;
}

Mouse& Mouse::setRectangle(Rect& r) {
  REGS ir, or;
  SREGS sr;


  ir.x.ax = 7;
  ir.x.cx = r.x1();
  ir.x.dx = r.y1();
  int86x(0x33, &ir, &or, &sr);

  ir.x.ax = 8;
  ir.x.cx = r.x2();
  ir.x.dx = r.y2();
  int86x(0x33, &ir, &or, &sr);

  return *this;
}

