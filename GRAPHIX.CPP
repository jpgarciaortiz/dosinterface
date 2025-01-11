#include "window.h"
#include "graphix.h"

void outtextdxy(int x, int y, char far *msg) {
  setcolor(DARKGRAY);
  outtextxy(x, y, msg);
  setcolor(WHITE);
  outtextxy(x, y, msg);
}

void horline(int x, int y, int width) {
  setcolor(DARKGRAY);
  line(x, y, x + width - 1, y);
  setcolor(WHITE);
  line(x, y + 1, x + width - 1, y + 1);
}

void drawbutton(int x, int y, int width, int height, char *title, int grosor, bool push, int color) {
  int x2 = x + width - 1;
  int y2 = y + height - 1;
  int colour1, colour2;

  if(push){ colour1 = DARKGRAY; colour2 = WHITE; }
  else { colour1 = WHITE; colour2 = DARKGRAY; }

  for(int i = 0; i < grosor; i++) {
    setcolor(colour1);
    line(x + i, y + i, x2 - (i + 1), y + i);
    line(x + i, y + i, x + i, y2 - (i + 1));
    setcolor(colour2);
    line(x2 - i , y + i, x2 - i, y2 - i);
     line(x + i , y2 - i, x2 - i, y2 - i);
  }

  setfillstyle(SOLID_FILL, color);
  bar(x + grosor, y + grosor, x2 - grosor, y2 - grosor);
  setcolor(BLACK);
  outtextxy((x + (width - textwidth(title)) / 2), y + push + ((height - textheight(title)) / 2), title);
}

void drawbutton(int x, int y, int width, int height, Bitmap& dib, int grosor, bool push, int color) {
  int x2 = x + width - 1;
  int y2 = y + height - 1;
  int colour1, colour2;

  if(push){ colour1 = DARKGRAY; colour2 = WHITE; }
  else { colour1 = WHITE; colour2 = DARKGRAY; }

  for(int i = 0; i < grosor; i++) {
    setcolor(colour1);
    line(x + i, y + i, x2 - (i + 1), y + i);
    line(x + i, y + i, x + i, y2 - (i + 1));
    setcolor(colour2);
    line(x2 - i , y + i, x2 - i, y2 - i);
     line(x + i , y2 - i, x2 - i, y2 - i);
  }

  setfillstyle(SOLID_FILL, color);
  bar(x + grosor, y + grosor, x2 - grosor, y2 - grosor);
  setcolor(BLACK);
  dib.setPosition(Point(x + ((width - dib.width()) / 2), y + push + ((height - dib.height()) / 2)));
  dib.paint();
}

void drawpanel(int x, int y, int width, int height) {
  int x2 = x + width - 1;
  int y2 = y + height - 1;

  setcolor(LIGHTGRAY);
  line(x, y, x2 - 1, y);
  line(x, y, x, y2 - 1);
  setcolor(BLACK);
  line(x2, y, x2, y2);
  line(x, y2, x2, y2);
  setcolor(WHITE);
  line(x + 1, y + 1, x2 - 2, y + 1);
  line(x + 1, y + 1, x + 1, y2 - 2);
  setcolor(DARKGRAY);
  line(x2 - 1, y + 1, x2 - 1, y2 - 1);
  line(x + 1, y2 - 1, x2 - 1, y2 - 1);
  setfillstyle(SOLID_FILL, LIGHTGRAY);
  bar(x + 2, y + 2, x2 - 2, y2 - 2);
}





