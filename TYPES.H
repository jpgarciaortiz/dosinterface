#ifndef __TYPES__H
#define __TYPES__H

/*-----------------------------------------------------------------------------
   TYPES

   by TRON
  ---------------------------------------------------------------------------*/

#ifndef NULL
#define NULL (0)
#endif

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;

typedef int bool;
#define TRUE   (1)
#define FALSE  (0)

#define NULLRECT  (Rect(1, 1, -1, -1))


class Rect;


/*------------------------------- Point -------------------------------------*/

class Point {
  public:
    int x, y;

    Point();
    Point(Point&);
    Point(int, int);

    Point& assign(Point&);
    int    distance(Point&);
    bool   isEqualTo(Point&);
    bool   isIncludedIn(Rect&);
    Point& relativeTo(Rect&);
};


/*-------------------------------- Rect -------------------------------------*/

class Rect {
  private:
    Point p1, p2;
    int r_width;
    int r_height;

  public:
    Rect();
    Rect(Rect&);
    Rect(Point&, Point&);
    Rect(int, int, int, int);

    Rect& assign(Rect&);
    Rect *intersection(Rect&);
    bool  isIncluded(Point&);
    bool  isEqualTo(Rect&);
    Rect& setDimension(int, int);
    Rect& setPosition(Point&);
    Rect& posIncr(int, int);
    Rect& dimIncr(int, int);

    inline const int width() { return r_width; }
    inline const int height() { return r_height; }
    inline const int x1() { return p1.x; }
    inline const int y1() { return p1.y; }
    inline const int x2() { return p2.x; }
    inline const int y2() { return p2.y; }
};


/*-------------------------------- Node -------------------------------------*/

class Node {
  public:
    void *item;
    Node *next;

    Node();
    Node(void *, Node *);
};


/*-------------------------------- Vector -----------------------------------*/

class Vector {
    Node *list;
    int num;

  public:
    Vector();

    inline const int count() { return num; }

    Vector& add(void *);
    Vector& remove(void *);

    void crossFromBegin(void (*) (void *));
    void crossFromEnd(void (*) (void *));

    void *operator[](int);

    ~Vector();
};

#endif
