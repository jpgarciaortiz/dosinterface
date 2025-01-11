#ifndef __CONTROLS__H
#define __CONTROLS__H

/*-----------------------------------------------------------------------------
   CONTROLS
  
	  Controles implementados: Button, ButtonMenu, ColorsPanel, StatusBar,
                              Label, Line3D, Entry, CommandButton y PaintPanel

   by TRON
  ---------------------------------------------------------------------------*/

#include "graphix.h"
#include "window.h"

// Puntero a un metodo de una clase Window
typedef void (Window::*windowMethod) ();


/*------------------------------ Hintable -----------------------------------*/
class Hintable {
  protected:
	 char *hint;

  public:
	 inline void setHint(char *cad) { hint = cad; }
    inline char *getHint() { return hint; } 
};


/*--------------------------------- Group -----------------------------------*/

class GroupControl;

class Group {
  private:
    GroupControl *act;  // Control actualmnte activo
    int numCtrls;       // Numero de controles en el grupo

  
  public:
    Group();

    void newControl();
    void deleteControl();

    void setActiveControl(GroupControl&);
};


/*----------------------------- GroupControl --------------------------------*/

class GroupControl :public Control {
  private:
    int  indexGroup;    // Numero de grupo

  protected:
    bool activated;     // TRUE si esta activo, FALSE en caso contrario

  public:
    GroupControl(Rect&, int);

    virtual void activate();
    virtual void desactivate();
    
    virtual void paint() { }
    virtual void onMouseEnter() { }
    virtual void onMouseExit() { }
    virtual void onMouseDown(Point&) { }
    virtual void onMouseMove(Point&) { }
    virtual void onMouseUp(Point&) { }
    inline bool isActivated() { return activated; }
    inline bool haveGroup() { return (indexGroup >= 0); }

    void setActive();

    void onKeyPress(char) { }  // Los controles de grupo ignoraran las teclas

    virtual ~GroupControl();
};


/*------------------------------- Button -------------------------------------*/

class Button: public GroupControl, public Hintable {
  private:
	 char *title;           // Caption
	 Bitmap *dib;           // Puntero al bitmap
	 windowMethod func;     // Puntero al metodo a llamar al hacer click

  public:
	 Button(Rect&, char *, windowMethod, int = -1, char * = NULL);
	 Button(Rect&, Bitmap&, windowMethod, int = -1, char * = NULL);

    void paint();
    void onMouseEnter();
    void onMouseExit();
    void onMouseDown(Point&);
    void onMouseMove(Point&) { }
    void onMouseUp(Point&);

    ~Button();
};


/*------------------------------ ButtonMenu ----------------------------------*/

struct MenuItem {
  char *cad;
  windowMethod onClick;
  char *hint;
  Rect rect;

  MenuItem(char *, windowMethod, char * = NULL);
};

class ButtonMenu: public Control, public Hintable {
  private:
	 char *title;
	 Vector opcs;
	 int maxw;
	 GraphBuffer *gb;
	 MenuItem *antItem, *actItem;
	 Rect menuRct;
    bool selected;

  public:
	 ButtonMenu(Rect& r, char *, char *, MenuItem *, ...);

	 void paint();

	 void onMouseEnter();
	 void onMouseExit();
	 void onMouseDown(Point&);
	 void onMouseMove(Point&);
	 void onMouseUp(Point&);

	 void onKeyPress(char) { }

	 ~ButtonMenu();
};


/*----------------------------- ColorsPanel ----------------------------------*/

class ColorsPanel :public Control {
  private:
	 int color, bgcolor;

  public:
	 ColorsPanel(Rect&);

	 inline int getColor() { return color; }
	 inline int getBgColor() { return bgcolor; }

	 void paint();
	 void onMouseEnter();
	 void onMouseExit();
	 void onMouseDown(Point&);
	 void onMouseUp(Point&) { }
	 void onMouseMove(Point&) { }
	 void onKeyPress(char) { }
};


/*------------------------------ StatusBar -----------------------------------*/

#define SB_LEFT   0
#define SB_RIGHT  1

struct StatusBarEntry;

class StatusBar: public Control {
  private:
	 int orientation;
	 Vector entrys;
	 char *mainCaption;

	 void paintCaption(StatusBarEntry * = NULL);

  public:
	 StatusBar(int = -1, int = -1, ...);

	 void paint();

	 void onMouseEnter() { }
	 void onMouseExit() { }
	 void onMouseDown(Point&) { }
	 void onMouseMove(Point&) { }
	 void onMouseUp(Point&) { }
	 void onKeyPress(char) { }

	 void setCaption(char *, int = -1);
};


/*--------------------------------- Label ------------------------------------*/

class Label: public Control {
  private:
	 char *cad;
	 int font;
	 int color;
	 int mulx, divx, muly, divy;

  public:
	 Label(int, int, char *, int, int = DEFAULT_FONT, int = 1, int = 1, int = 1, int = 1);

	 void paint();

	 void onMouseEnter() { }
	 void onMouseExit() { }
	 void onMouseDown(Point&) { }
	 void onMouseMove(Point&) { }
	 void onMouseUp(Point&) { }
	 void onKeyPress(char) { }

    inline void setCaption(char *s) { cad = s; }
};


/*-------------------------------- Line3D ------------------------------------*/

#define L_HORIZONTAL   1
#define L_VERTICAL     2

class Line3D :public Control {
  private:
	 int orientation;
	 int size;

  public:
	 Line3D(int, int, int, int);

	 void paint();

	 void onMouseEnter() { }
	 void onMouseExit() { }
	 void onMouseDown(Point&) { }
	 void onMouseMove(Point&) { }
	 void onMouseUp(Point&) { }
	 void onKeyPress(char) { }
};


/*------------------------------ PaintPanel ----------------------------------*/

class PaintPanel: public Control {
  private:
	 MouseHandler *handler;

  public:
	 PaintPanel(MouseHandler *);

	 void paint();
	 void onMouseEnter();
	 void onMouseExit();
	 void onMouseDown(Point&);
	 void onMouseMove(Point&);
	 void onMouseUp(Point&);
	 void onKeyPress(char) { }

    void setViewPort();
};


/*-------------------------------- Entry -------------------------------------*/

class Entry: public Control {
  private:
	 char cad[200];

  public:
	 Entry(Rect&, char *);

	 inline char *getText() { return cad; }

	 void paint();
	 void onMouseEnter() { }
	 void onMouseDown(Point&) { }
	 void onMouseMove(Point&) { }
	 void onMouseUp(Point&) { }
	 void onMouseExit() { }
	 void onKeyPress(char);
};


/*------------------------- CommandButton ------------------------------------*/

class CommandButton: public Control {
  private:
	 char *title;           // Caption
	 Bitmap *dib;           // Puntero al bitmap
	 windowMethod func;     // Puntero al metodo a llamar al hacer click
    int pressed;           // == 1 (Pulsado), == 0 (Normal)

  public:
	 CommandButton(Rect&, char *, windowMethod, Bitmap * = NULL);
	 CommandButton(Rect&, Bitmap&, windowMethod, Bitmap * = NULL);

	 void paint();
	 void onMouseEnter() { }
	 void onMouseExit() { }
	 void onMouseDown(Point&);
	 void onMouseMove(Point&) { }
	 void onMouseUp(Point&);
    void onKeyPress(char) { }

	 ~CommandButton();
};

/*----------------------------------------------------------------------------
	La forma de crear ventanas e insertar controles es la siguiente:
		1.- Creamos una clase nueva derivada de la clase Window
      2.- Definimos el 'BUILDER' de la ventana (*)
      3.- En el contructor de la ventana, llamamos a BUILD()
      4.- Instanciamos la clase creada 

   ejemplo:
      
      class MiVentana: public Window {
        public:
          void onClick() {
              // ...
          } 

          WINDOW_BUILDER(MiVentana) {
            NEW_CONTROL(new Button(0, 0, 100, 100, "TEST", (windowMethod)MiVentana::onClick);
          }

          MiVentana() :Window("EJEMPLO", Rect(100, 100, 100, 100), W_NORMAL) {
            BUILD(MiVentana);
          }      
       };
----------------------------------------------------------------------------*/

#define WINDOW_BUILDER(name) friend void __windowBuilder##name(name *_win)
#define CONTROL(ctrl)        _win->##ctrl
#define NEW_CONTROL(what)    _win->addCtrl(what)
#define BUILD(name)          __windowBuilder##name(this)
#define SELF                 (_win)

#endif