#ifndef __MOUSE__H
#define __MOUSE__H

/*-----------------------------------------------------------------------------
   MOUSE

   by TRON
  ---------------------------------------------------------------------------*/

#include "types.h"

struct Cursor {
  int hotx, hoty;
  int mask[32];
};

class Mouse {
  public:
    Point   pos;
    bool    leftPressed;
    bool    rightPressed;
    int     numButtons;
    bool    installed;
    Cursor *cursor;

    Mouse();

    Mouse& reset();
    Mouse& show();
    Mouse& hide();
    Mouse& refresh();
    Mouse& setPosition(Point&);
    Mouse& setCursor(Cursor&);
    Mouse& setRectangle(Rect&);
};

#endif

