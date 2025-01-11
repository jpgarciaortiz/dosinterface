#ifndef __WINDOW__H
#define __WINDOW__H

/*-----------------------------------------------------------------------------
   WINDOW

   by TRON
  ---------------------------------------------------------------------------*/

#include "mouse.h"


/*---------------------------- GraphicObject --------------------------------*/

class GraphicObject {
  protected:
    Rect rect;    // Rectangulo que define el objeto grafico

  public:
    GraphicObject(Rect& r) { rect.assign(r); }

    inline Rect getRect() { return Rect(rect); }
    inline int width() { return rect.width(); }
    inline int height() { return rect.height(); }
    inline Point position() { return Point(rect.x1(), rect.y1()); }

    virtual void setPosition(Point&);
    virtual void setDimension(int, int);

    virtual void paint() = 0;
};


/*----------------------------- GraphBuffer ---------------------------------*/

#define NUM_MEM_BLOQUES 4  // Numero de bloques en el que dividir un un buffer

class GraphBuffer: public GraphicObject {
  private:
    void *buff[4];  // Buffer con el contenido de una porcion de pantalla

  public:
    GraphBuffer(Rect&);

    void setDimension(int, int);
    void paint();

    ~GraphBuffer();
};


/*------------------------------- Bitmap ------------------------------------*/

class Bitmap: public GraphicObject {
  private:
    int tcolor;        // Color transparente
    byte far *data;    // Puntero al buffer de datos
    bool buff;         // Indica si el buffer es externo o no

  public:
    Bitmap(int, int, int, byte far *);
    Bitmap(int, int, int, ...);

    void setDimension(int, int);
    void paint();

    ~Bitmap();
};


/*------------------------------- Window ------------------------------------*/

class Control;
class GraphicApplication;

/* Definimos los tipos de ventanas posibles */
#define W_SIZEABLE       1    // La ventana se podra redimensionar
#define W_TITLE          2    // La ventana tendra titulo
#define W_CLOSEBUTTON    4    // La ventana tendra boton de cerrar
#define W_MODAL          8    // La ventana sera de tipo modal

#define W_NORMAL        (W_SIZEABLE | W_TITLE | W_CLOSEBUTTON)

class Window: public GraphicObject {
  protected:
	 char        *title;      // Titulo
	 int          type;       // Tipo de ventana
	 Bitmap      *icon;       // Icono de la ventana
	 Vector       ctrls;      // Controles que contiene la ventana
	 GraphBuffer *gbuff;      // Buffer con lo que tapa la ventana
	 Rect         downRect;   // Area que tapa la ventana

    void drawTitle(bool = TRUE);

  public:
	 int minWidth;            // Anchura minima de la ventana
	 int minHeight;           // Altura minima de la ventana

	 Window(char *, Rect&, int = W_NORMAL, Bitmap * = NULL);

	 inline int getType() { return type; }
	 inline int ctrlCount() { return ctrls.count(); }
	 inline Control *ctrl(int i) { return ((Control *)ctrls[i]); }
	 inline void setTitle(char *cad) { title = cad; }
	 inline char *getTitle() { return title; }

	 Rect getTitlePanelRect();
	 Rect getResizeCornerRect();
	 Rect getClientRect();
	 Rect getCloseButtonRect();

	 void addCtrl(Control *);
	 void removeCtrl(Control *);

	 void saveBackground();
	 void restoreBackground();

	 virtual void paint();
	 virtual void show();
	 virtual void hide();
	 virtual void bringToFront();

	 virtual void showHint(char *) { }
	 virtual void hideHint() { }

	 virtual ~Window();
};


/*------------------------------ MouseHandler -------------------------------*/

class MouseHandler {
  public:
	 virtual void onMouseEnter() = 0;
	 virtual void onMouseExit() = 0;
	 virtual void onMouseDown(Point&) = 0;
	 virtual void onMouseMove(Point&) = 0;
	 virtual void onMouseUp(Point&) = 0;
};


/*------------------------------- Control -----------------------------------*/

class Control: public GraphicObject, public MouseHandler {
  protected:
	 Window *parent;    // Ventana a la que pertenece

  public:
	 Control(Rect& r) :GraphicObject(r) { }

	 inline Window& getParent() { return *parent; }

	 virtual void paint() = 0;
	 virtual void onKeyPress(char) = 0;

    virtual ~Control() { }

  friend Window;
};

/*------------------------ GraphicApplication -------------------------------*/

class GraphicApplication {
  protected:
    Mouse    *mouse;            // Informacion sobre el raton
    Rect     *actView;          // Vista actual
	 int       maxX, maxY;       // Posicion maxima de la pantalla
    Control  *cFocus;           // Control con el foco actual

  public:
    GraphicApplication();

    inline Rect& getViewRect() { return *actView; }
    inline Mouse& getMouse() { return *mouse; }

	 inline void setFocus(Control *c) { cFocus = c; }

    void setViewPort(Rect&);
    void destroyViewPort();
    void clearBackground();

    GraphBuffer *saveScreen(Rect&);
    void restoreScreen(GraphBuffer *);

    void run();
    void refresh();

    ~GraphicApplication();
};


/*--------------------------- Variables globales ----------------------------*/
#ifndef __WINDOW_MAIN
extern GraphicApplication *application;   // Aplicacion actual
extern Vector windowsList;                // Lista de ventanas creadas
extern int backgroundColor;               // Color del fondo
extern char *driverDirectory;             // Directorio del driver grafico
extern Window *winActual;                 // Ventana actual
#endif


#endif







