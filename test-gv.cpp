//
//  Created by Yufeng Wu on 3/2/22.
//
#include "ECGraphicViewImp.h"
#include "PaintMVC.h"

// Test graphical view code
int real_main(int argc, char **argv)
{
    bool fileIn = argc-1;
    
    const int widthWin = 1000, heightWin = 1000;
    ECGraphicViewImp view(widthWin, heightWin);
  
    // create model & observers.
    PaintModel paintModel(view);
    
    if ( fileIn ) {
        paintModel.LoadFromFile(argv[1]);
    }
    PaintController paintCtrl = paintModel.GetCtrl();
    ECObserver* keyboard = new KeyboardObserver(view, paintCtrl);
    ECObserver* mouse = new MouseObserver(view, paintCtrl);

    view.Attach(keyboard);
    view.Attach(mouse);

    view.Show();

    view.Detach(keyboard);
    view.Detach(mouse);

    if ( fileIn ) {
        paintModel.WriteToFile(argv[1]);
    }
    delete keyboard;
    delete mouse;

    return 0;
}

int main(int argc, char **argv)
{
    return real_main(argc, argv);
    //return al_run_main(argc, argv, real_main);
}

