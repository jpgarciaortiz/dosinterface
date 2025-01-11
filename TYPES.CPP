#include <math.h>
#include "types.h"

/*------------------------------- Point -------------------------------------*/

Point::Point() { }

Point::Point(Point& p) {
  x = p.x;
  y = p.y;
}

Point::Point(int px, int py) {
  x = px;
  y = py;
}

int Point::distance(Point& p) {
  return sqrt(((x - p.x)*(x - p.x)) + ((y - p.y)*(y - p.y)));
}

Point& Point::assign(Point& p) {
  x = p.x;
  y = p.y;
  return *this;
}

bool Point::isEqualTo(Point& p) {
  return ((x == p.x) && (y == p.y));
}

bool Point::isIncludedIn(Rect& r) {
  return r.isIncluded(*this);
}

Point& Point::relativeTo(Rect& r) {
  x -= r.x1();
  y -= r.y1();
  return *this;
}


/*-------------------------------- Rect -------------------------------------*/

Rect::Rect() { }

Rect::Rect(Rect& r) {
  assign(r);
}

Rect::Rect(Point& pp1, Point& pp2) {
  p1.assign(pp1);
  p2.assign(pp2);
  r_width = (int)abs(p2.x - p1.x) + 1;
  r_height = (int)abs(p2.y - p1.y) + 1;
}

Rect::Rect(int x, int y, int pwidth, int pheight) {
  p1.x = x;
  p1.y = y;
  r_width = pwidth;
  r_height = pheight;
  p2.x = x + r_width - 1;
  p2.y = y + r_height - 1;
}

Rect *Rect::intersection(Rect& r) {
  // Falta implementar la interseccion
  return &r;
}

Rect& Rect::assign(Rect& r) {
  p1.assign(r.p1);
  p2.assign(r.p2);
  r_width = r.r_width;
  r_height = r.r_height;
  return *this;
}

bool Rect::isEqualTo(Rect& r) {
  return (p1.isEqualTo(r.p1) && p2.isEqualTo(r.p2));
}

bool Rect::isIncluded(Point& p) {
  return ((p.x >= p1.x) && (p.x <= p2.x) && (p.y >= p1.y) && (p.y <= p2.y));
}

Rect& Rect::setDimension(int pwidth, int pheight) {
  r_width = pwidth;
  r_height = pheight;
  p2.x = p1.x + r_width - 1;
  p2.y = p1.y + r_height - 1;
  return *this;
}

Rect& Rect::setPosition(Point& p) {
  p1.assign(p);
  p2.x = p1.x + r_width - 1;
  p2.y = p1.y + r_height - 1;
  return *this;
}

Rect& Rect::posIncr(int inx, int iny) {
  p1.x += inx;
  p1.y += iny;
  p2.x = p1.x + r_width - 1;
  p2.y = p1.y + r_height - 1;
  return *this;
}

Rect& Rect::dimIncr(int inw, int inh) {
  r_width += inw;
  r_height += inh;
  p2.x = p1.x + r_width - 1;
  p2.y = p1.y + r_height - 1;
  return *this;
}



/*-------------------------------- Node -------------------------------------*/

Node::Node() {
  next = NULL;
}

Node::Node(void *it, Node *n) {
  item = it;
  next = n;
}


/*-------------------------------- Vector -----------------------------------*/

Vector::Vector() {
  list = NULL;
  num = 0;
}

Vector& Vector::add(void *item) {
  Node *aux = new Node(item, list);

  if(aux) {
    num++;
    list = aux;
  }

  return *this;
}

Vector& Vector::remove(void *item) {
  Node *ant = NULL;

  if(num) {
    for(Node *pos = list; pos; pos = pos->next) {
      if(pos->item == item) break;
      ant = pos;
    }

    if(pos) {
      if(! ant) list = list->next;
      else ant->next = pos->next;
      delete pos;
      num--;
    }
  }

  return *this;
}

void Vector::crossFromBegin(void (*func) (void *)) {
  Node *pos = list;

  while(pos) {
    (*func) (pos->item);
    pos = pos->next;
  }
}

static void recursiveCross(void (*func) (void *), Node *n) {
  if(n) {
    recursiveCross(func, n->next);
    (*func) (n->item);
  }
}

void Vector::crossFromEnd(void (*func) (void *)) {
  recursiveCross(func, list);
}

void *Vector::operator[](int n) {
  Node *pos = list;

  if(n >= 0 && n < num) {
    for(int i = 1; i <= n; i++) pos = pos->next;
    return pos->item;
  } else
    return NULL;
}

Vector::~Vector() {
  Node *pos;

  while(list) {
    pos = list;
    list = list->next;
    delete pos;
  }
}


