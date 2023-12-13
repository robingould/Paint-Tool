#ifndef PaintMVC_h
#define PaintMVC_h

#include "Polygon.h"
#include "Command.h"
#include "ECObserver.h"
#include "ECGraphicViewImp.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

#include <string>
#include <set>

enum paint_mode_t
{
    MODE_EDIT = 0,
    MODE_INSERT = 1
};

enum polygon_type_mode_t
{
    MODE_RECTANGLE = 0,
    MODE_ELLIPSE = 1
};

class PaintModel;


/*
============
Insert Command
============
*/
class CmdInsert : public PaintCmd {
public:
                        CmdInsert( 
                            PaintModel      &modelIn, 
                            const int       x1in, 
                            const int       y1in, 
                            const int       x2in, 
                            const int       y2in,
                            const int       typein,
                            const bool      filledin,
                            const ECGVColor colorin);
    virtual             ~CmdInsert();
    virtual void        Execute();
    virtual void        UnExecute();
    virtual void        Print();
    virtual void        UnPrint();
private:
    PaintModel          &model;
    EC2DPoint           top_left_point;
    EC2DPoint           bottom_right_point;
    int                 type;
    bool                filled;
    ECGVColor           color;
    AbstractPolygon *   polygon_ptr;
};

/*
============
Move Command (MOUSE)
============
*/
class CmdMove : public PaintCmd {
public:
                        CmdMove(
                            PaintModel      &modelIn, 
                            std::set<AbstractPolygon *> 
                                            &selectedIn, 
                            const int       xmoveIn, 
                            const int       ymoveIn);
    virtual             ~CmdMove();
    virtual void        SetTotals(const int x1, const int y1);
    virtual void        Execute();
    virtual void        UnExecute();
    virtual void        Print();
    virtual void        UnPrint();
private:
    PaintModel &        model;
    std::set<AbstractPolygon *>   
                        polygons;
    int                 xmove;
    int                 ymove;
    int                 xtotal;
    int                 ytotal;
};

/*
============
Move Key Command (move command but with keyboard)
============
*/
class CmdKeyMove : public PaintCmd {
public:
                        CmdKeyMove(
                            PaintModel      &modelIn, 
                            std::set<AbstractPolygon *> 
                                            &selectedIn, 
                            const int       xmoveIn, 
                            const int       ymoveIn);
    virtual             ~CmdKeyMove();
    virtual void        Execute();
    virtual void        UnExecute();
    virtual void        Print();
    virtual void        UnPrint();
private:
    PaintModel &        model;
    std::set<AbstractPolygon *>   
                        polygons;
    int                 xmove;
    int                 ymove;
};

/*
============
Group Command
============
*/
class CmdGroup : public PaintCmd {
public:
                        CmdGroup( 
                            PaintModel      &modelIn, 
                            std::set<AbstractPolygon *> 
                                            &selected);
    virtual             ~CmdGroup();
    virtual void        Execute();
    virtual void        UnExecute();
    virtual void        Print();
    virtual void        UnPrint();
private:
    PaintModel          &model;
    std::set<AbstractPolygon *> polygons;
    AbstractPolygon *   polygon_ptr;
};
/*
============
UnGroup Command
============
*/
class CmdUngroup : public PaintCmd {
public:
                        CmdUngroup( 
                            PaintModel      &modelIn, 
                            CompositePolygon * &selected);
    virtual             ~CmdUngroup();
    virtual void        Execute();
    virtual void        UnExecute();
    virtual void        Print();
    virtual void        UnPrint();
private:
    PaintModel          &model;
    AbstractPolygon * polygon_comp;
    std::set<AbstractPolygon *> ungrouped;
    AbstractPolygon *   polygon_ptr;
};

/*
============
Remove Command
============
*/
class CmdRemove : public PaintCmd {
public:
                        CmdRemove(
                            PaintModel      &modelIn, 
                            std::set<AbstractPolygon *> 
                                            &selectedIn);
    virtual             ~CmdRemove();
    virtual void        Execute();
    virtual void        UnExecute();
    virtual void        Print();
    virtual void        UnPrint();
private:
    PaintModel &        model;
    std::set<AbstractPolygon *>
                        selected;

    std::vector<AbstractPolygon *> 
                        removed_polygons;
};

/*
============
PaintController Class
============
*/
class PaintController {
public:
                        PaintController(
                            ECGraphicViewImp &viewIn, 
                            PaintModel       &modelIn );
    virtual             ~PaintController();

    // getters
    ECGraphicViewImp&   GetView() const;            // get view
    PaintModel&         GetModel() const;           // get model
    bool                GetMode() const;            // get mode
    int                 GetInsertType() const;      // get type of polygon to insert
    bool                GetFillMode() const;        // get fill mode

    // state setters/state resetters
    void                SwitchMode();              // switch mode edit->insert, insert->edit
    void                SwitchInsertType();        // switch between rect or ellipse
    void                SwitchFillMode();          // switch fill mode empty->fill, fill->empty
    void                ClearStateRedraw();        // clear all states and redraw scene

    // command pattern related methods
    void                Insert(const int x, const int y);      // handle insert at x, y
    void                Move(const int x, const int y);        // move by x, y
    void                Remove();
    bool                Undo();
    bool                Redo();

    // views related methods
    void                Select(std::vector<AbstractPolygon *> polygons);    // select 
    void                GroupUngroup();         // handle grouping and ungrouping polygons
    void                CtrlDown();             // handle select like mouse down
    void                CtrlUp();               // handle select like mouse down
    void                MouseDown();            // handle mouse down (either on to insert or to select)
    void                MouseUp();              // handle mouse up (either on to insert or to select)
    void                MouseMoving();          // handling moving the mouse, in order to move polygons
    void                HandleMouseEvents();    // wrap up handling all mouse events
    void                MoveUp10();
    void                MoveDown10();
    void                MoveRight10();
    void                MoveLeft10();

private:
    PaintModel          &model;
    ECGraphicViewImp    &view;
    PaintCmdHistory     cmdHistory;
    std::set<AbstractPolygon *>     
                        selected;

    // edit mode is 0, the default. Insert mode is 1. Press spacebar to swaps
    // Use enum MODE_EDIT and MODE_INSERT 
    int                 mode;               // mode swapping btwn MODE_EDIT and insert, starts on 0 aka MODE_EDIT
    int                 type;               // polygon type swap between MODE_RECTANGLE = 0, MODE_ELLIPSE = 1
    bool                fill_mode;          // mode swapping btwn fill (true) empty (false)

    EC2DPoint           initial;            // initial position storage if we drag the mouse
    EC2DPoint           intermediate;       // shortcut for undo
    bool                handle_mouse_down;  // if we are in edit mode and click on nothing, we don't want to do anything on mouse up
    bool                handle_ctrl_down;   // if ctrl is held down we don't refresh selected every click
    bool                different_move;
};

/*
============
PaintModel Class
============
*/
class PaintModel {
public:
                        PaintModel( 
                            ECGraphicViewImp    &viewIn);
    virtual             ~PaintModel();

    // getters
    PaintController&    GetCtrl();
    AbstractPolygon *  
                        GetPolygonAt(const EC2DPoint &p);
    std::vector<AbstractPolygon *>
                        GetPolygons();

    // command pattern related methods
    void                AddPolygon(const EC2DPoint &pa, const EC2DPoint &pb, const int typein, const bool filled, const ECGVColor colorin, AbstractPolygon *&ptr); // add polygon
    void                AddPolygon(const std::set<AbstractPolygon *> &polygons, AbstractPolygon *&ptr);
    void                MovePolygon(AbstractPolygon * polygon, const int xmove, const int ymove);
    void                RemovePolygon(AbstractPolygon *ptr);

    // connect to allegro drawing API
    void                Draw(const std::vector<AbstractPolygon *> &polygon_vec_in);

    // methods for file reading
    void                LoadFromFile(const std::string &f);
    AbstractPolygon*    LoadPolygon(const std::string &line, std::ifstream &f);

    // methods for file writing
    void                WritePolygon(AbstractPolygon *polygon);
    void                WriteToFile(const std::string &f);
    

private:
    ECGraphicViewImp    &view;
    PaintController     ctrl;
    std::vector<AbstractPolygon *>
                        polygon_vec;

};

/*
============
Views/Observers for MVC Pattern
============
*/
class MouseObserver : public ECObserver
{
public:
                        MouseObserver(
                            ECGraphicViewImp    &viewIn,
                            PaintController     &ctrlIn );
    virtual void        Update();
    
private:
    ECGraphicViewImp    &view;
    PaintController     &ctrl;
};

class KeyboardObserver : public ECObserver
{
public:
                        KeyboardObserver(
                            ECGraphicViewImp    &viewIn, 
                            PaintController     &ctrlIn);
    virtual void        Update();
    
private:
    ECGraphicViewImp    &view;
    PaintController     &ctrl;
};

#endif /* PaintMVC_h */
