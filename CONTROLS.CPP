#include <string.h>
#include <stdarg.h>
#include <stdio.h>  //
#include <string.h>
#include "controls.h"

static Vector GroupList;  // Vector con todos los grupos existentes

/*--------------------------------- Group -----------------------------------*/

Group::Group() {
  act = NULL;
  numCtrls = 0;
}

void Group::newControl() {
  numCtrls++;
}

void Group::deleteControl() {
  if(! --numCtrls) {
    GroupList.remove(this);
    delete this;
  } else 
    act = NULL;
}

void Group::setActiveControl(GroupControl& ctrl) {
  if(numCtrls == 1) {
    if(ctrl.isActivated()) ctrl.desactivate();
    else ctrl.activate();
  } else {
    if(act) act->desactivate();
    act = &ctrl;
    act->activate();
  }
}   
  

/*----------------------------- GroupControl --------------------------------*/

GroupControl::GroupControl(Rect& r, int grp) :Control(r) {
  if(grp >= 0) {
    Group *g; 
    int max = GroupList.count() - 1;
    if(grp > max) {
      g = new Group;
      GroupList.add(g);
      //grp = max + 1;
    } else {
      g = (Group *)GroupList[grp];
    }       
    g->newControl();
  } 
  indexGroup = grp;
  activated = FALSE;
}

void GroupControl::activate() {
  activated = TRUE;
  application->getMouse().hide();
  paint();
  application->getMouse().show();
}

void GroupControl::desactivate() {
  activated = FALSE;
  application->getMouse().hide();
  paint();
  application->getMouse().show();
}

void GroupControl::setActive() {
  if(indexGroup >= 0)  
    ((Group *)GroupList[indexGroup])->setActiveControl(*this);
  else {
    activated = !activated;
    application->getMouse().hide();
    paint();
    application->getMouse().show();
  }
}

GroupControl::~GroupControl() {
  if(indexGroup >= 0)
    ((Group *)GroupList[indexGroup])->deleteControl();
}


/*------------------------------- Button -------------------------------------*/

Button::Button(Rect& r, char *t, windowMethod on, int grp, char *h) :GroupControl(r, grp) {
  dib = NULL;
  title = t;
  func = on;
  hint = h;
}

Button::Button(Rect& r, Bitmap& bmp, windowMethod on, int grp, char *h) :GroupControl(r, grp) {
  title = NULL;
  dib = &bmp;
  func = on;
  hint = h;
}

void Button::paint() {
  if(dib) drawbutton(rect, *dib, activated, activated, (activated && haveGroup()) ? DARKGRAY : LIGHTGRAY);
  else drawbutton(rect, title, activated, activated, (activated && haveGroup()) ? DARKGRAY : LIGHTGRAY);
}

void Button::onMouseEnter() {
  if(! activated) {
	 application->getMouse().hide();
	 application->setViewPort(parent->getClientRect());
	 if(dib) drawbutton(rect, *dib, 1, FALSE);
	 else drawbutton(rect, title, 1, FALSE);
	 application->getMouse().show();
	 application->destroyViewPort();
  }
  parent->showHint(hint);
}

void Button::onMouseExit() {
  if(! activated) {
	 application->getMouse().hide();
	 if(dib) drawbutton(rect, *dib, 0, FALSE);
	 else drawbutton(rect, title, 0, FALSE);
	 application->getMouse().show();
  }
  parent->hideHint();
}

void Button::onMouseDown(Point&) {
  setActive();
}

void Button::onMouseUp(Point&) {
  if(! haveGroup()) setActive();
  ((*parent).*func)();
}

Button::~Button() {
  if(dib) delete dib;
}


/*------------------------------ ButtonMenu ----------------------------------*/

MenuItem::MenuItem(char *a, windowMethod b, char *h) {
  cad = a;
  onClick = b;
  hint = h;
}

ButtonMenu::ButtonMenu(Rect& r, char *cad, char *h, MenuItem *a, ...) :Control(r) {
  va_list p;

  hint = h;
  maxw = textwidth(a->cad);
  title = cad;
  opcs.add(a);

  va_start(p, a);
  while(a = (MenuItem *)va_arg(p, MenuItem *), a != NULL) {
	 if(textwidth(a->cad) > maxw) maxw = textwidth(a->cad);
	 opcs.add(a);
  }
  va_end(p);

  for(int i = 0; i < opcs.count(); i++) {
	 MenuItem *mi = (MenuItem *)opcs[i];
	 mi->rect.assign(Rect(2, 2 + (i * (textheight("H") + 8)), maxw + 8, textheight("H") + 8));
  }

  antItem = actItem = NULL;
  selected = FALSE;
}

void ButtonMenu::paint() {
  drawbutton(rect, title, 0, FALSE);
}

void ButtonMenu::onMouseEnter() {
  application->getMouse().hide();
  drawbutton(rect, title, 1, FALSE);
  application->getMouse().show();
  parent->showHint(hint);
}

void ButtonMenu::onMouseExit() {
  application->getMouse().hide();
  drawbutton(rect, title, 0, FALSE);
  application->getMouse().show();
  parent->hideHint();
}

void ButtonMenu::onMouseDown(Point&) {
  selected = TRUE;
  menuRct.assign(Rect(parent->getClientRect().x1() + rect.x1(),
							 parent->getClientRect().y1() + rect.y2(),
							 maxw + 12, ((textheight("H") + 8) * opcs.count()) + 4));

  parent->hideHint();
  application->getMouse().hide();
  drawbutton(rect, title, 1, TRUE);
  application->getMouse().show();
  application->destroyViewPort();
  gb = application->saveScreen(menuRct);
  application->getMouse().hide();
  drawbutton(menuRct, "", 1, FALSE);

  int posy = menuRct.y1() + 2;
  for(int i = 0; i < opcs.count(); i++) {
	 MenuItem *mi = (MenuItem *)opcs[i];
	 posy += 4;
	 outtextxy(menuRct.x1() + 6, posy, mi->cad);
	 posy += textheight("H");
	 posy += 4;
  }

  application->getMouse().show();
}

void ButtonMenu::onMouseMove(Point& p) {
  if(selected) {
	 actItem = NULL;
	 p.y -= rect.height();
	 for(int i = 0; i < opcs.count(); i++) {
		MenuItem *im = (MenuItem *)opcs[i];

		if(p.isIncludedIn(im->rect)) {
		  actItem = im;
		  break;
		}
	 }

	 if(actItem != antItem) {
		application->getMouse().hide();
		if(antItem) {
		  setfillstyle(SOLID_FILL, LIGHTGRAY);
		  bar(menuRct.x1() + antItem->rect.x1(), menuRct.y1() + antItem->rect.y1(),
				menuRct.x1() + antItem->rect.x2(), menuRct.y1() + antItem->rect.y2());
		  setcolor(BLACK);
		  outtextxy(menuRct.x1() + antItem->rect.x1() + 4, menuRct.y1() + antItem->rect.y1() + 4,
						antItem->cad);
		  parent->hideHint();
		}
		if(actItem) {
		  setfillstyle(SOLID_FILL, BLUE);
		  bar(menuRct.x1() + actItem->rect.x1(), menuRct.y1() + actItem->rect.y1(),
				menuRct.x1() + actItem->rect.x2(), menuRct.y1() + actItem->rect.y2());
		  setcolor(WHITE);
		  outtextxy(menuRct.x1() + actItem->rect.x1() + 4, menuRct.y1() + actItem->rect.y1() + 4,
						actItem->cad);
		  parent->showHint(actItem->hint);
		}
		application->getMouse().show();
	 }
	 antItem = actItem;
  }
}

void ButtonMenu::onMouseUp(Point&) {
  selected = FALSE;
  application->restoreScreen(gb);
  delete gb;
  application->getMouse().hide();
  drawbutton(Rect(parent->getClientRect().x1() + rect.x1(),
						parent->getClientRect().y1() + rect.y1(),
						rect.width(), rect.height()),
						title, 0, FALSE);
  application->getMouse().show();
  parent->hideHint();
  if(actItem) ((*parent).*(actItem->onClick))();
  actItem = NULL;
}

static void doFreeItem(void *i) { delete (MenuItem *)i; }
ButtonMenu::~ButtonMenu() {
  opcs.crossFromBegin(doFreeItem);
}


/*----------------------------- ColorsPanel ----------------------------------*/

ColorsPanel::ColorsPanel(Rect& r) :Control(r) {
  bgcolor = WHITE;
  color = BLACK;
}

void ColorsPanel::paint() {
  int w, h;
  drawbutton(Rect(rect.x1() + ((rect.width() - 30) / 2) + 10, rect.y1() + 5 + 10, 20, 20), "", 2, TRUE, bgcolor);
  drawbutton(Rect(rect.x1() + ((rect.width() - 30) / 2), rect.y1() + 5, 20, 20), "", 2, TRUE, color);

  w = rect.width() / 2;
  h = (rect.height() - 40) / 8;
  for(int i = 0; i < 8; i++)
	 for(int j = 0; j < 2; j++)
		drawbutton(Rect(rect.x1() + (j * w), rect.y1() + 40 + (i * h), w, h), "", 1, TRUE, (i * 2) + j);
}

void ColorsPanel::onMouseEnter() {
  parent->showHint("Paleta de colores");
}

void ColorsPanel::onMouseExit() {
  parent->hideHint();
}

void ColorsPanel::onMouseDown(Point& p) {
  int w = rect.width() / 2;
  int h = (rect.height() - 40) / 8;
  int col = (((p.y - 40) / h) * 2) + (p.x / w);

  if(col >= 0 && col <= 15) {
	 application->getMouse().hide();
	 if(application->getMouse().leftPressed) color = col;
	 else bgcolor = col;
	 drawbutton(Rect(rect.x1() + ((rect.width() - 30) / 2) + 10, rect.y1() + 5 + 10, 20, 20), "", 2, TRUE, bgcolor);
	 drawbutton(Rect(rect.x1() + ((rect.width() - 30) / 2), rect.y1() + 5, 20, 20), "", 2, TRUE, color);
	 application->getMouse().show();
  }
}


/*------------------------------ StatusBar -----------------------------------*/

struct StatusBarEntry {
  int width;
  int left;
  char *caption;

  StatusBarEntry(int a, int b) {
	 width = a;
	 left = b;
    caption = "";
  }
};

StatusBar::StatusBar(int or, int val, ...) :Control(NULLRECT) {
  va_list p;
  int suml = 0;

  if(val >= 0) {
	 orientation = or;
	 entrys.add(new StatusBarEntry(val, suml));
	 suml += val + 1;
	 va_start(p, val);
	 while((val = va_arg(p, int)) != -1) {
		entrys.add(new StatusBarEntry(val, suml));
		suml += val + 1;
	 }
	 va_end(p);
  }
  mainCaption = "";
}

void StatusBar::paint() {
  int i, suml;
  StatusBarEntry *sbe;

  switch(orientation) {
	 case SB_LEFT:
		for(i = entrys.count() - 1; i >= 0; i--) {
		  sbe = (StatusBarEntry *)entrys[i];
		  drawbutton(Rect(sbe->left, parent->getClientRect().height() - textheight("H") - 6, sbe->width, textheight("H") + 6), "", 1, TRUE);
		  paintCaption(sbe);
		}
		drawbutton(Rect(sbe->left + sbe->width + 1, parent->getClientRect().height() - textheight("H") - 6,
							 parent->getClientRect().width() - sbe->left - sbe->width - 1, textheight("H") + 6), "", 1, TRUE);
		paintCaption();
		break;

	 case SB_RIGHT:
		suml = parent->getClientRect().width();
		for(i = entrys.count() - 1; i >= 0; i--) {
		  sbe = (StatusBarEntry *)entrys[i];
		  suml -= sbe->width;
		  sbe->left = suml;
		  drawbutton(Rect(sbe->left, parent->getClientRect().height() - textheight("H") - 6, sbe->width, textheight("H") + 6), "", 1, TRUE);
		  paintCaption(sbe);
		  suml -= 1;
		}
		drawbutton(Rect(0, parent->getClientRect().height() - textheight("H") - 6, suml, textheight("H") + 6), "", 1, TRUE);
		paintCaption();
		break;

	 default:
		drawbutton(Rect(0, parent->getClientRect().height() - textheight("H") - 6, parent->getClientRect().width(), textheight("H") + 6), "", 1, TRUE);
      paintCaption();
  }

  if(parent->getType() & W_SIZEABLE) {
	 int x2 = parent->getClientRect().width(), y2 = parent->getClientRect().height();
	 setcolor(LIGHTGRAY);
	 line(x2 - 1, y2 - 1, x2 - 1, y2 - 1);
	 setcolor(DARKGRAY);
	 line(x2 - 2, y2 - 1, x2 - 1, y2 - 2);
	 line(x2 - 3, y2 - 1, x2 - 1, y2 - 3);
	 setcolor(WHITE);
	 line(x2 - 4, y2 - 1, x2 - 1, y2 - 4);

	 setcolor(LIGHTGRAY);
	 line(x2 - 5, y2 - 1, x2 - 1, y2 - 5);
	 setcolor(DARKGRAY);
	 line(x2 - 6, y2 - 1, x2 - 1, y2 - 6);
	 line(x2 - 7, y2 - 1, x2 - 1, y2 - 7);
	 setcolor(WHITE);
	 line(x2 - 8, y2 - 1, x2 - 1, y2 - 8);
  }
}

void StatusBar::setCaption(char *cad, int pos) {
  StatusBarEntry *sbe;

  Rect aux(application->getViewRect());
  application->setViewPort(parent->getClientRect());

  if(pos > entrys.count() || pos < 0) sbe = NULL;
  else sbe = (StatusBarEntry *)entrys[entrys.count() - 1 - pos];

  if(sbe) sbe->caption = cad;
  else mainCaption = cad;

  application->getMouse().hide();
  paintCaption(sbe);
  application->getMouse().show();

  application->setViewPort(aux);
}

void StatusBar::paintCaption(StatusBarEntry *sbe) {
  if(entrys.count() == 0) {
	 outtextxy(3, parent->getClientRect().height() - textheight("H") - 3, mainCaption);
  } else {
	 if(sbe)
		outtextxy(sbe->left + 3, parent->getClientRect().height() - textheight("H") - 3, sbe->caption);
	 else {
		if(orientation == SB_LEFT) {
		  StatusBarEntry *sbe = (StatusBarEntry *)entrys[0];
		  outtextxy(sbe->left + sbe->width + 4, parent->getClientRect().height() - textheight("H") - 3, mainCaption);
		} else outtextxy(3, parent->getClientRect().height() - textheight("H") - 3, mainCaption);
	 }
  }

}


/*--------------------------------- Label ------------------------------------*/

Label::Label(int x, int y, char *c, int col, int f, int mx, int dx, int my, int dy) :Control(Rect(x, y, 1, 1)) {
  color = col;
  cad = c;
  font = f;
  mulx = mx;
  divx = dx;
  muly = my;
  divy = dy;
}

void Label::paint() {
  int aux = getcolor();
  setcolor(color);
  settextstyle(font, HORIZ_DIR, 0);
  setusercharsize(mulx, divx, muly, divy);
  outtextxy(rect.x1(), rect.y1(), cad);
  settextstyle(DEFAULT_FONT, HORIZ_DIR, 0);
  setusercharsize(1, 1, 1, 1);
  setcolor(aux);
}


/*-------------------------------- Line3D ------------------------------------*/

Line3D::Line3D(int x, int y, int w, int or) :Control(Rect(x, y, 1, 1)) {
  orientation = or;
  size = w;
}

void Line3D::paint() {
  if(orientation == L_HORIZONTAL) {
	 setcolor(DARKGRAY);
	 line(rect.x1(), rect.y1(), rect.x1() + size - 1, rect.y1());
	 setcolor(WHITE);
	 line(rect.x1(), rect.y1() + 1, rect.x1() + size - 1, rect.y1() + 1);
  } else {
	 setcolor(DARKGRAY);
	 line(rect.x1(), rect.y1(), rect.x1(), rect.y1() + size - 1);
	 setcolor(WHITE);
	 line(rect.x1() + 1, rect.y1(), rect.x1() + 1, rect.y1() + size - 1);
  }
}


/*------------------------------ PaintPanel ----------------------------------*/

PaintPanel::PaintPanel(MouseHandler *h) :Control(NULLRECT) {
  handler = h;
}

void PaintPanel::paint() {
  rect.assign(Rect(42, 17, parent->getClientRect().width() - 43, parent->getClientRect().height() - 35));
  drawbutton(rect, "", 1, TRUE, WHITE);
}

void PaintPanel::onMouseEnter() {
  handler->onMouseEnter();
}

void PaintPanel::onMouseExit() {
  handler->onMouseExit();
}

void PaintPanel::onMouseDown(Point& p) {
  p.x--; p.y--;
  setViewPort();
  handler->onMouseDown(p);
}

void PaintPanel::onMouseMove(Point& p) {
  p.x--; p.y--;
  if(!application->getMouse().leftPressed && !application->getMouse().rightPressed) setViewPort();
  handler->onMouseMove(p);
}

void PaintPanel::onMouseUp(Point& p) {
  p.x--; p.y--;
  handler->onMouseUp(p);
}

void PaintPanel::setViewPort() {
  application->setViewPort(Rect(parent->getClientRect().x1() + rect.x1() + 1,
                                parent->getClientRect().y1() + rect.y1() + 1,
										  rect.width() - 2, rect.height() - 2));
}


/*-------------------------------- Entry -------------------------------------*/

Entry::Entry(Rect& r, char *def) :Control(r) {
  strcpy(cad, def);
}

void Entry::paint() {
  Rect aux(application->getViewRect());
  application->setViewPort(parent->getClientRect());
  drawbutton(rect, "", 2, TRUE, WHITE);
  application->setViewPort(Rect(parent->getClientRect().x1() + rect.x1() + 2,
										  parent->getClientRect().y1() + rect.y1() + 2,
										  rect.width() - 4, rect.height() - 4));
  setcolor(BLACK);
  outtextxy(1, 1, cad);
  line(1 + textwidth(cad), 1, 1 + textwidth(cad), textheight("H") + 1);
  application->setViewPort(aux);
}

void Entry::onKeyPress(char c) {
  int len = strlen(cad);
  if(c == 8) {
	 if(len) cad[len - 1] = 0;
  } else if(c >= 32) {
	 cad[len] = c;
	 cad[len + 1] = 0;
  }
  application->getMouse().hide();
  paint();
  application->getMouse().show();
}


/*--------------------------- CommandButton ----------------------------------*/

CommandButton::CommandButton(Rect& r, char *t, windowMethod on, Bitmap *bmp) :Control(r) {
  dib = NULL;
  title = t;
  func = on;
  dib = bmp;
  pressed = 0;
}

void CommandButton::paint() {
  drawbutton(rect, "", 2, pressed);

  int x = ((rect.width() - 4) - ((dib ? dib->getRect().width() : 0) + (title ? textwidth(title) : 0))) / 2;
  if(dib) {
	 dib->setPosition(Point(rect.x1() + pressed + x, rect.y1() + pressed + ((rect.height() - dib->getRect().height()) / 2)));
	 dib->paint();
  }
  if(title) {
	 setcolor(BLACK);
	 outtextxy(rect.x1() + x + pressed + (dib ? dib->getRect().width() : 0), rect.y1() + pressed + ((rect.height() - textheight(title)) / 2), title);
  }
}

void CommandButton::onMouseDown(Point&) {
  application->getMouse().hide();
  application->setViewPort(parent->getClientRect());
  pressed++; paint();
  application->getMouse().show();
  application->destroyViewPort();
}

void CommandButton::onMouseUp(Point&) {
  application->getMouse().hide();
  application->setViewPort(parent->getClientRect());
  pressed--; paint();
  application->getMouse().show();
  application->destroyViewPort();
  ((*parent).*func)();
}

CommandButton::~CommandButton() {
}


