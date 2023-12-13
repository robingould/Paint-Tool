#include "PaintMVC.h"

/*
============
Insert Command
============
*/

// helper functions
EC2DPoint SetTopLeft(const int x1in, const int y1in, const int x2in, const int y2in) {
    int top_left_x = ((x1in < x2in) ? x1in : x2in);
    int top_left_y = ((y1in < y2in) ? y1in : y2in);
    return EC2DPoint(top_left_x, top_left_y);
}

EC2DPoint SetBottomRight(const int x1in, const int y1in, const int x2in, const int y2in) {
    int bottom_right_x = ((x1in >= x2in) ? x1in : x2in);
    int bottom_right_y = ((y1in >= y2in) ? y1in : y2in);
    return EC2DPoint(bottom_right_x, bottom_right_y);
}

CmdInsert :: CmdInsert( PaintModel &modelIn, const int x1in, const int y1in, const int x2in, const int y2in, const int typein, const bool filledin, const ECGVColor colorin) : 
        model(modelIn),
        type(typein),
        filled(filledin),
        color(colorin),
        polygon_ptr(NULL)
{
    // select top left vs bottom right, then set points 
    top_left_point = SetTopLeft(x1in, y1in, x2in, y2in);
    bottom_right_point = SetBottomRight(x1in, y1in, x2in, y2in);
}

CmdInsert :: ~CmdInsert() {}

void CmdInsert :: Execute() {
    model.AddPolygon(top_left_point, bottom_right_point, type, filled, color, polygon_ptr);
    //polygon_ptr->Print();
    //Print();
}

void CmdInsert :: UnExecute() {
    //UnPrint();
    model.RemovePolygon(polygon_ptr);
}

void CmdInsert :: Print() {
    std::cout << "Inserted at: " << std::endl;
    top_left_point.Print();
    bottom_right_point.Print();
}

void CmdInsert :: UnPrint() {
    std::cout << "Undone Insert at: " << std::endl;
    top_left_point.Print();
    bottom_right_point.Print();
}

/*
============
Move Command (MOUSE)
============
*/
CmdMove :: CmdMove(PaintModel &modelIn, std::set<AbstractPolygon *> &selectedIn, int xmoveIn, int ymoveIn): 
    model(modelIn),
    polygons(selectedIn),
    xmove(xmoveIn),
    ymove(ymoveIn)
{}

CmdMove :: ~CmdMove() {}

void CmdMove :: SetTotals(const int x1, const int y1) {
    xtotal = x1;
    ytotal = y1;
}

void CmdMove :: Execute() {
    for (auto polygon : polygons) {
        model.MovePolygon(polygon, xmove, ymove);
    }
    //Print();
}

void CmdMove :: UnExecute() {
    for (auto polygon : polygons) {
        model.MovePolygon(polygon, -xtotal, -ytotal);
    }
    //UnPrint();
    xmove = xtotal;
    ymove = ytotal;
}

void CmdMove :: Print() {
    for ( auto polygon : polygons ) {
        std::cout << "Moved " << std::endl;
        polygon->Print();
        std::cout << "by " << xmove << ymove << std::endl;
    }
}

void CmdMove :: UnPrint() {
    for ( auto polygon : polygons ) {
        std::cout << "Undone Move " << std::endl;
        polygon->Print();
        std::cout << "by " << -xmove << -ymove << std::endl;
    }
}

/*
============
Move Key Command (move command but with keyboard)
============
*/
CmdKeyMove :: CmdKeyMove(PaintModel &modelIn, std::set<AbstractPolygon *> &selectedIn, int xmoveIn, int ymoveIn): 
    model(modelIn),
    polygons(selectedIn),
    xmove(xmoveIn),
    ymove(ymoveIn)
{}

CmdKeyMove :: ~CmdKeyMove() {}

void CmdKeyMove :: Execute() {
    for (auto polygon : polygons) {
        model.MovePolygon(polygon, xmove, ymove);
    }
    //Print();
}

void CmdKeyMove :: UnExecute() {
    for (auto polygon : polygons) {
        model.MovePolygon(polygon, -xmove, -ymove);
    }
    //UnPrint();
}

void CmdKeyMove :: Print() {
    for ( auto polygon : polygons ) {
        std::cout << "Moved " << std::endl;
        polygon->Print();
        std::cout << "by " << xmove << ymove << std::endl;
    }
}

void CmdKeyMove :: UnPrint() {
    for ( auto polygon : polygons ) {
        std::cout << "Undone Move " << std::endl;
        polygon->Print();
        std::cout << "by " << -xmove << -ymove << std::endl;
    
    }
}

/*
============
Group Command
============
*/

CmdGroup :: CmdGroup(PaintModel &modelIn, std::set<AbstractPolygon *> &selected) :
    model(modelIn),
    polygons(selected),
    polygon_ptr(NULL)
{}

CmdGroup :: ~CmdGroup() {
    polygons.clear();
}

void CmdGroup :: Execute() {
    model.AddPolygon(polygons, polygon_ptr);
}

void CmdGroup :: UnExecute() {
    model.RemovePolygon(polygon_ptr);
    for ( auto polygon : polygons ) {
        EC2DPoint top_left = polygon->GetTopLeft();
        EC2DPoint bottom_right = polygon->GetBotRight();
        int type = polygon->type;
        bool filled = polygon->filled;
        ECGVColor color = polygon->GetColor();
        model.AddPolygon(top_left, bottom_right, type, filled, color, polygon);
    }
}

void CmdGroup :: Print() {
    std::cout << "Group" << std::endl;
}

void CmdGroup :: UnPrint() {
    std::cout << "UnGrouped" << std::endl;
}

/*
============
UnGroup Command
============
*/
CmdUngroup :: CmdUngroup(PaintModel &modelIn, CompositePolygon *&selected) :
    model(modelIn),
    polygon_comp(selected),
    polygon_ptr(NULL)
{}

CmdUngroup :: ~CmdUngroup() {
}

void CmdUngroup :: Execute() {
    model.RemovePolygon(polygon_ptr);
    for ( auto polygon : polygon_comp->GetChildren() ) {
        EC2DPoint top_left = polygon->GetTopLeft();
        EC2DPoint bottom_right = polygon->GetBotRight();
        int type = polygon->type;
        bool filled = polygon->filled;
        ECGVColor color = polygon->GetColor();
        ungrouped.insert(polygon);
        model.AddPolygon(top_left, bottom_right, type, filled, color, polygon);
    }
}

void CmdUngroup :: UnExecute() {
    model.AddPolygon(ungrouped, polygon_ptr);
}

void CmdUngroup :: Print() {
    std::cout << "UnGrouped" << std::endl;
}

void CmdUngroup :: UnPrint() {
    std::cout << "Group" << std::endl;
}

/*
============
Remove Command
============
*/
CmdRemove :: CmdRemove( PaintModel &modelIn, std::set<AbstractPolygon *> &selectedIn ) :
    model(modelIn),
    selected(selectedIn)
{}

CmdRemove :: ~CmdRemove() {}

void CmdRemove :: Execute() {
    for ( auto polygon : selected ) {
        removed_polygons.push_back(polygon);
        model.RemovePolygon(polygon);
    }
    Print();
}

void CmdRemove :: UnExecute() {
    for ( auto polygon : selected ) {
        EC2DPoint top_left = polygon->GetTopLeft();
        EC2DPoint bottom_right = polygon->GetBotRight();
        int type = polygon->type;
        bool filled = polygon->filled;
        ECGVColor color = polygon->GetColor();
        model.AddPolygon(top_left, bottom_right, type, filled, color, polygon);
    }
    UnPrint();
}

void CmdRemove :: Print() {
    for ( auto polygon : selected ) {
        std::cout << "Removed " << std::endl;
        polygon->Print();
    }
}

void CmdRemove :: UnPrint() {
    for ( auto polygon : selected ) {
        std::cout << "Undone remove of " << std::endl;
        polygon->Print();
    }
}

/*
============
PaintController Class
    // Note: call SetRedraw(true) once there is something changed in the view ***
============
*/

PaintController :: PaintController(ECGraphicViewImp &viewIn, PaintModel &modelIn ) :
    model(modelIn),
    view(viewIn), 
    mode(MODE_EDIT), // default mode is edit mode 
    type(MODE_RECTANGLE), // default polygon insert type is rectangle 
    fill_mode(false), // dont fill by default
    handle_mouse_down(false),
    handle_ctrl_down(false),
    different_move(true)
{ 
    std::cout << "Initialized in edit mode..." << std::endl;
    std::cout << "polygon insert type is rectangle..." << std::endl;
    std::cout << "fill mode is empty..." << std::endl;
}

PaintController :: ~PaintController() {}

// getters
ECGraphicViewImp& PaintController :: GetView() const {
    return view;
}        

PaintModel& PaintController :: GetModel() const {
    return model;
}  

bool PaintController :: GetMode() const {
    return mode;
}

int PaintController :: GetInsertType() const {
    return type;
}


bool PaintController :: GetFillMode() const {
    return fill_mode;
}

// state setters/state resetters
void PaintController :: SwitchMode(){
    // bitwise, 1->0, 0->1
    mode = !mode;
    if (mode == MODE_EDIT) {
        std::cout << "edit mode" << std::endl;
    } else {
        std::cout << "insert mode" << std::endl;
    }
    
    ClearStateRedraw();
}

void  PaintController :: SwitchInsertType() {
    // bitwise, 1->0, 0->1
    type = !type;
    if (type == MODE_RECTANGLE) {
        std::cout << "rectangle mode" << std::endl;
    } else {
        std::cout << "ellipse mode" << std::endl;
    }
}

void PaintController :: SwitchFillMode(){
    // bitwise, 1->0, 0->1
    fill_mode = !fill_mode;
    if ( !fill_mode ) {
        std::cout << "empty fill mode" << std::endl;
    } else {
        std::cout << "fill mode" << std::endl;
    }
}

void PaintController :: ClearStateRedraw() {
    handle_mouse_down = false;
    selected.clear();
    view.SetRedraw(true);
}

// command pattern related methods
void PaintController :: Insert(const int x, const int y) {
    switch ( type ) {
        case MODE_RECTANGLE:
            view.DrawRectangle(initial.GetX(), initial.GetY(), x, y);
            break;
        case MODE_ELLIPSE:
            int x1 = initial.GetX();
            int y1 = initial.GetY();
            int x2 = x;
            int y2 = y;
            int centerx = (x1 + x2)/2;
            int centery = (y1 + y2)/2;
            double radiusx = static_cast<double>(abs(centerx - x1));
            double radiusy = static_cast<double>(abs(centery - y1));
            view.DrawEllipse(centerx, centery, radiusx, radiusy);
            break;
    }
}

void PaintController :: Move(const int x, const int y) {
    if ( !selected.empty() ) {
        CmdMove* move = new CmdMove(model, selected, x, y);
        cmdHistory.ExecuteCmd(move);
        view.SetRedraw(true);
    }
}

void PaintController :: Remove() {
    if ( mode == MODE_EDIT && !selected.empty()) {
        CmdRemove* remove = new CmdRemove(model, selected);
        cmdHistory.ExecuteCmd(remove);
        ClearStateRedraw();
    }
}

bool PaintController :: Undo() {
    bool result = cmdHistory.Undo();
    ClearStateRedraw();
    return result;
}

bool PaintController :: Redo() {
    bool result = cmdHistory.Redo();
    ClearStateRedraw();
    return result;
}

    // views related methods
void PaintController :: Select(std::vector<AbstractPolygon *> polygons) {
    for ( auto polygon : polygons ) {
        if(polygon == NULL) {
            return;
        } else {
            Polygon *pgon = dynamic_cast<Polygon *>(polygon);
            if ( pgon != NULL ) {
                int type = pgon->type;
                switch ( type ) {
                    case MODE_RECTANGLE:
                        view.DrawRectangle(pgon->GetTopLeft().GetX(), pgon->GetTopLeft().GetY(), pgon->GetBotRight().GetX(), pgon->GetBotRight().GetY(), 5, ECGV_BLUE);
                        break;
                    case MODE_ELLIPSE:
                        int tlx = pgon->GetTopLeft().GetX();
                        int tly = pgon->GetTopLeft().GetY();
                        int brx = pgon->GetBotRight().GetX();
                        int bry = pgon->GetBotRight().GetY();
                        int centerx = (tlx + brx)/2;
                        int centery = (tly + bry)/2;
                        double radiusx = static_cast<double>(abs(centerx - tlx));
                        double radiusy = static_cast<double>(abs(centery - tly));
                        view.DrawEllipse(centerx, centery, radiusx, radiusy, 5, ECGV_BLUE);
                        break;
                        } 
                    }
                CompositePolygon *c = dynamic_cast<CompositePolygon*>(polygon);
                if ( c != NULL ) {
                    Select(c->GetChildren());
                }
        }
    }
}

void PaintController :: GroupUngroup() {
    // if we have more than 1 selected, group
    if ( selected.size() > 1 ) {
        CmdGroup* group = new CmdGroup(model, selected);
        cmdHistory.ExecuteCmd(group);
    }
    // if we have a group selected, and it is composite, ungroup
    if ( selected.size() == 1 ) {
        CompositePolygon *c = dynamic_cast<CompositePolygon *>(*selected.begin());
        if (c != NULL ) {
        std::cout << "ungrouping" << std::endl;
        CmdUngroup* ungroup = new CmdUngroup(model, c);
        cmdHistory.ExecuteCmd(ungroup);
        }
    }
    ClearStateRedraw();
}

void PaintController :: CtrlDown() {
    handle_ctrl_down = true;
}

void PaintController :: CtrlUp() {
    handle_ctrl_down = false;
}

void PaintController :: MouseDown() {
    handle_mouse_down = true;
    int x, y;
    view.GetCursorPosition(x, y);
    initial = EC2DPoint(x, y);
    intermediate = EC2DPoint(x, y);
    if ( mode == MODE_EDIT ) {
        AbstractPolygon * pgons = model.GetPolygonAt(initial);
        if ( !handle_ctrl_down ) {
            selected.clear();
        }
        //for ( auto polygon : pgons ) {
            if ( pgons == NULL ) {
                handle_mouse_down = false;
            } else {
                selected.insert(pgons);
            }
        //}
        view.SetRedraw(true);
    }
}

void PaintController :: MouseUp() {
    int x, y;
    different_move = true;
    view.GetCursorPosition(x, y);
    EC2DPoint eventual = EC2DPoint(x, y);
    view.SetRedraw(true);
    if ( handle_mouse_down && mode == MODE_INSERT ) {
        // insert then add to cmdslist
        CmdInsert* insert_poly = new CmdInsert(model, initial.GetX(), initial.GetY(), x, y, type, fill_mode, ECGV_BLACK);
        cmdHistory.ExecuteCmd(insert_poly);
    }

    view.SetRedraw(true);
    handle_mouse_down = false;
}

void PaintController :: MouseMoving() {
    int x, y;
    view.GetCursorPosition(x, y);
    EC2DPoint current = EC2DPoint(x, y);
    if ( handle_mouse_down && !selected.empty() && mode == MODE_EDIT && initial.Distance(current) > 1 ) {
        if ( !cmdHistory.Empty() && !different_move) {
            cmdHistory.DeleteLast();
        }
        different_move = false;
        int xmove_v = (x - intermediate.GetX());
        int ymove_v = (y - intermediate.GetY());
        CmdMove* move_rect_temp = new CmdMove(model, selected, xmove_v, ymove_v);
        int xtotal = intermediate.GetX() - initial.GetX();
        int ytotal = intermediate.GetY() - initial.GetY();
        move_rect_temp->SetTotals(xtotal, ytotal);
        cmdHistory.ExecuteCmd(move_rect_temp);
        intermediate = EC2DPoint(x, y);
    }
}

void PaintController :: HandleMouseEvents() {
    model.Draw(model.GetPolygons());
    int x, y;
    view.GetCursorPosition(x, y);
    if ( handle_mouse_down ) {
        if ( mode == MODE_INSERT ) {
            Insert(x, y);
        }
        //view.SetRedraw(true);
    }
    std::vector<AbstractPolygon *> polygons_selected(selected.begin(), selected.end());
    Select(polygons_selected);
    view.SetRedraw(true);
}

void PaintController :: MoveUp10() {
    if ( mode == MODE_EDIT && !selected.empty() ) {
        CmdKeyMove* move_pgon = new CmdKeyMove(model, selected, 0, -10);
        cmdHistory.ExecuteCmd(move_pgon);
    }
}
void PaintController :: MoveDown10() {
    if ( mode == MODE_EDIT && !selected.empty()) {
        CmdKeyMove* move_pgon = new CmdKeyMove(model, selected, 0, 10);
        cmdHistory.ExecuteCmd(move_pgon);
    }
}
void PaintController :: MoveRight10() {
    if ( mode == MODE_EDIT && !selected.empty() ) { 
        CmdKeyMove* move_pgon = new CmdKeyMove(model, selected, 10, 0);
        cmdHistory.ExecuteCmd(move_pgon);
    }
}
void PaintController :: MoveLeft10() {
    if ( mode == MODE_EDIT && !selected.empty() ) {
        CmdKeyMove* move_pgon = new CmdKeyMove(model, selected, -10, 0);
        cmdHistory.ExecuteCmd(move_pgon);
    }
}

/*
============
PaintModel Class
============
*/
PaintModel :: PaintModel( ECGraphicViewImp &viewIn ): 
    view(viewIn), 
    ctrl(view, *this)
{}

PaintModel :: ~PaintModel() {
    polygon_vec.clear();
}

    // getters
PaintController& PaintModel :: GetCtrl(){
    return ctrl;
}

AbstractPolygon* PaintModel :: GetPolygonAt(const EC2DPoint &p) {
    std::vector<AbstractPolygon*> polyg;
    for ( auto piter = polygon_vec.rbegin(); piter != polygon_vec.rend(); ++piter ) {
        auto polygon = *piter;
        if ( polygon->IsPointInside(p) ) {
            //std::cout << "clicked inside polygon" << std::endl;
            //polygon->Print();
            return polygon;
        }
    }
    return NULL;
}

std::vector<AbstractPolygon *> PaintModel :: GetPolygons() {
    return polygon_vec;
}

    // command pattern related methods
void PaintModel :: AddPolygon(const EC2DPoint &pa, const EC2DPoint &pb, const int typein, const bool filledin, const ECGVColor colorin, AbstractPolygon *&ptr) {
    if ( ptr == NULL ) {
        ptr = new Polygon(pa, pb, typein, colorin, filledin);
    }
    polygon_vec.push_back(ptr);
}

void PaintModel :: AddPolygon(const std::set<AbstractPolygon *> &polygonsIn, AbstractPolygon *&ptr) {
    if ( ptr != NULL ) {
        return;
    }
    ptr = new CompositePolygon(polygonsIn);
    polygon_vec.push_back(ptr);
}

void PaintModel :: MovePolygon(AbstractPolygon *ptr, int xmove, int ymove) {
    ptr->MoveBy(xmove, ymove);
}

void PaintModel :: RemovePolygon(AbstractPolygon *ptr) {
    polygon_vec.erase(std::remove(polygon_vec.begin(), polygon_vec.end(), ptr), polygon_vec.end());
}

    // connect to allegro drawing API
void PaintModel :: Draw(const std::vector<AbstractPolygon *> &polygon_vec_in) {
    for ( auto x : polygon_vec_in) {
            Polygon* p = dynamic_cast<Polygon*>(x);
            if ( p != NULL ) {
            int type = p->type;
            switch ( type ) {
                case MODE_RECTANGLE:
                    if ( !p->filled ) {
                        view.DrawRectangle(p->GetTopLeft().GetX(), p->GetTopLeft().GetY(), p->GetBotRight().GetX(), p->GetBotRight().GetY(), 3, p->GetColor());
                    } else {
                        view.DrawFilledRectangle(p->GetTopLeft().GetX(), p->GetTopLeft().GetY(), p->GetBotRight().GetX(), p->GetBotRight().GetY(), p->GetColor());
                    }
                    break;
                case MODE_ELLIPSE:
                    int x1 = p->GetTopLeft().GetX();
                    int y1 = p->GetTopLeft().GetY();
                    int x2 = p->GetBotRight().GetX();
                    int y2 = p->GetBotRight().GetY();
                    int centerx = (x1 + x2)/2;
                    int centery = (y1 + y2)/2;
                    double radiusx = static_cast<double>(abs(centerx - x1));
                    double radiusy = static_cast<double>(abs(centery - y1));
                    if ( !p->filled ) {
                        view.DrawEllipse(centerx, centery, radiusx, radiusy,  3, p->GetColor());
                    } else {
                        view.DrawFilledEllipse(centerx, centery, radiusx, radiusy, p->GetColor());
                    }
                    break;
                }
            } else {
                CompositePolygon* c = dynamic_cast<CompositePolygon*>(x);
                Draw(c->GetChildren());
            }
        }
}

void PaintModel :: LoadFromFile(const std::string &f) {
    std::ifstream file;
    file.open(f);
    if ( file.is_open() ) {
        std::string line;
        std::getline(file, line);
        int num_to_load = stoi(line);
        std::cout << num_to_load << " polygons to load" << std::endl;

        for (int i = 0; i < num_to_load; ++i ) {
            std::getline(file, line);
            LoadPolygon(line, file);
        }
        file.close();
    }
    view.SetRedraw(true);

}
AbstractPolygon* PaintModel :: LoadPolygon(const std::string &line, std::ifstream &f) {
    std::vector<int> parsed_line;
    std::istringstream iss(line);
    int current;
    while ( iss >> current ) {
        parsed_line.push_back(current);
    }

    int type = parsed_line[0];
    int fill = type < 2 ? 0 : 1;
    AbstractPolygon * next_polygon = NULL;

    if ( type == 4 ){
        std::cout << "composite with ";
        std::set<AbstractPolygon *> composite_children;

        int num_children = parsed_line[1];
        std::cout << num_children << " polygons" << std::endl;
        std::string child_line;

        while ( num_children ) {
            std::getline(f, child_line);
            AbstractPolygon *child = LoadPolygon(child_line, f);
            composite_children.insert(child);
            --num_children;
            std::cout << num_children << " left" << std::endl;
        }
        AddPolygon(composite_children, next_polygon);
        return next_polygon;
    }

        ECGVColor color = static_cast<ECGVColor>(parsed_line.back()); 
        /***
        switch ( parsed_line.back() ) {
            case 0:
                color = ECGV_BLACK;
                break;
            case 1:
                color = ECGV_WHITE;
                break;
            case 2:
                color = ECGV_RED;
                break;
            case 3: 
                color = ECGV_GREEN;
                break;
            case 4:
                color = ECGV_BLUE;
                break;
            case 5:
                color = ECGV_YELLOW;
                break;
            case 6:
                color = ECGV_PURPLE;
                break;
            case 7:
                color = ECGV_CYAN;
                break;
        }*/

        if ( type == 0 || type == 2 ) {
            std::cout << "rectangle" << std::endl;
            EC2DPoint rect_top_left(parsed_line[2], parsed_line[3]);
            EC2DPoint rect_bottom_right(parsed_line[6], parsed_line[7]);
            AddPolygon(rect_top_left, rect_bottom_right, 0, fill, color, next_polygon);
        } else {
            std::cout << "ellipse" << std::endl;
            EC2DPoint ellipse_top_left(parsed_line[1] - parsed_line[3], parsed_line[2] - parsed_line[4]);
            EC2DPoint ellipse_bot_right(parsed_line[1] + parsed_line[3], parsed_line[2] + parsed_line[4]);
            AddPolygon(ellipse_top_left, ellipse_bot_right, 1, fill, color, next_polygon);
        }
        
    return next_polygon;
}


void PaintModel :: WriteToFile(const std::string &f) {
    //std::ofstream file;
    //file.open(f);
    //if ( file.is_open() ) {
    unsigned num_polygons = polygon_vec.size();
    for ( auto polygon : polygon_vec ) {
        CompositePolygon* c = dynamic_cast<CompositePolygon*>(polygon);
        if ( c != NULL ) {
            for ( auto x : c->GetChildren() ) {
                num_polygons -= 1;
            }
        }
    }

    std::cout << num_polygons << std::endl;
    std::set<AbstractPolygon *> test;
    std::vector<AbstractPolygon *> result;
    // each individual line

    for ( auto polygon : polygon_vec ) {
        WritePolygon(polygon);
        test.insert(polygon);
        result.push_back(polygon);
        for ( auto c : polygon->GetChildren() ) {
            if ( test.find(c) != test.end() ) {
                test.erase(test.find(c));
            }
            test.insert(polygon);
            result.push_back(c);
        }
    }
    std::cout << std::endl;
    const char *cstr = f.data(); 
    freopen(cstr, "w", stdout);
    std::cout << num_polygons << std::endl;
    for ( auto x : result ) {
        if ( test.find(x) != test.end() ){
            WritePolygon(x);
        } 
    }
    //    file.close();
    //}
}

void PaintModel :: WritePolygon(AbstractPolygon *polygon) {   
        Polygon* p = dynamic_cast<Polygon*>(polygon);
        if ( p != NULL ) {
            std::cout << p->type + p->filled*2 << " " ;
            if ( !p->type ) {
                std::cout << "4 ";
                std::cout << p->GetTopLeft().GetX() << " " << p->GetTopLeft().GetY() << " ";
                std::cout << p->GetTopLeft().GetX() << " " << p->GetBotRight().GetY() << " ";
                std::cout << p->GetBotRight().GetX() << " " << p->GetBotRight().GetY() << " ";
                std::cout << p->GetBotRight().GetX() << " " << p->GetTopLeft().GetY() << " ";
            }
            if ( p->type ) {
                int centerx = (p->GetTopLeft().GetX() + p->GetBotRight().GetX())/2;
                int centery = (p->GetTopLeft().GetY() + p->GetBotRight().GetY())/2;
                double radiusx = abs(centerx - p->GetTopLeft().GetX());
                double radiusy = abs(centery - p->GetTopLeft().GetY());
                std::cout << centerx << " " << centery << " ";
                std::cout << radiusx << " " << radiusy << " ";
            }
            int color = 0;
            switch ( p->color ) {
            case ECGV_BLACK:
                color = 0;
                break;
            case ECGV_WHITE:
                color = 1;
                break;
            case ECGV_RED:
                color = 2;
                break;
            case ECGV_GREEN: 
                color = 3;
                break;
            case ECGV_BLUE:
                color = 4;
                break;
            case ECGV_YELLOW:
                color = 5;
                break;
            case ECGV_PURPLE:
                color = 6;
                break;
            case ECGV_CYAN:
                color = 7;
                break;
            }
            std::cout << color << " ";

            std::cout << std::endl;
        }
        CompositePolygon* c = dynamic_cast<CompositePolygon*>(polygon);
        if ( c != NULL ) {
            std::cout << "4 ";
            std::cout << c->GetChildren().size();
            std::cout << std::endl; 
            for ( auto i : c->GetChildren() ) {
                WritePolygon(i);
            }
        }
}





/*
============
MouseObserver Class
============
*/
MouseObserver :: MouseObserver(  ECGraphicViewImp &viewIn, PaintController &ctrlIn ) : 
    view(viewIn), 
    ctrl(ctrlIn)
{}

void MouseObserver :: Update()
{
    ECGVEventType evt = view.GetCurrEvent();

    switch ( evt ) {
            int x;
            int y;
        case ECGV_EV_MOUSE_BUTTON_DOWN:
            view.GetCursorPosition(x, y);
            std::cout << "Mouse Down @ (" << x << ", "<< y << ")" << std::endl;
            ctrl.MouseDown();
            break;
        case ECGV_EV_MOUSE_BUTTON_UP:
            view.GetCursorPosition(x, y);
            std::cout << "Mouse Up @ (" << x << ", " << y << ")"<< std::endl;
            ctrl.MouseUp();
            break;
        case ECGV_EV_MOUSE_MOVING:
            ctrl.MouseMoving();
            break;
    }
    ctrl.HandleMouseEvents();
}

/*
============
KeyboardObserver Class
============
*/
KeyboardObserver :: KeyboardObserver( ECGraphicViewImp &viewIn, PaintController &ctrlIn ) : 
    view(viewIn), 
    ctrl(ctrlIn)
{}

void KeyboardObserver :: Update()
{
    ECGVEventType evt = view.GetCurrEvent();
    int mode = ctrl.GetMode();

    switch ( evt ) {
        case ECGV_EV_KEY_UP_UP:
            return;
        case ECGV_EV_KEY_UP_DOWN:
            return;
        case ECGV_EV_KEY_UP_LEFT:
            return;
        case ECGV_EV_KEY_UP_RIGHT:
            return;
        case ECGV_EV_KEY_UP_ESCAPE:
            return;
        case ECGV_EV_KEY_DOWN_UP:
            ctrl.MoveUp10();
            return;
        case ECGV_EV_KEY_DOWN_DOWN:
            ctrl.MoveDown10();
            return;
        case ECGV_EV_KEY_DOWN_LEFT:
            ctrl.MoveLeft10();
            return;
        case ECGV_EV_KEY_DOWN_RIGHT:
            ctrl.MoveRight10();
            return;
        case ECGV_EV_KEY_DOWN_ESCAPE :
            return;
        case ECGV_EV_KEY_UP_Z:
            return;
        case ECGV_EV_KEY_DOWN_Z:
            ctrl.Undo();
            return;
        case ECGV_EV_KEY_UP_Y:
            return;
        case ECGV_EV_KEY_DOWN_Y:
            ctrl.Redo();
            return;
        case ECGV_EV_KEY_UP_D:
            return;
        case ECGV_EV_KEY_DOWN_D:
            ctrl.Remove();
            return;
        case ECGV_EV_KEY_UP_SPACE:
            return;
        case ECGV_EV_KEY_DOWN_SPACE:
            ctrl.SwitchMode();
            return;
        case ECGV_EV_KEY_DOWN_G:
            switch ( mode ) {
                case MODE_INSERT:
                    ctrl.SwitchInsertType();
                    break;
                case MODE_EDIT:
                    break;
            }
            return;
        case ECGV_EV_KEY_UP_G:
            if ( mode == MODE_EDIT ) {
                ctrl.GroupUngroup();
            }
            return;
        case ECGV_EV_KEY_DOWN_F:
            ctrl.SwitchFillMode();
            return;
        case ECGV_EV_KEY_UP_F:
            return;
        case ECGV_EV_KEY_DOWN_CTRL:
            ctrl.CtrlDown();
            return;
        case ECGV_EV_KEY_UP_CTRL:
            ctrl.CtrlUp();
            return;
    }
}


