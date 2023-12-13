#include "Command.h"

/********************************************************************/
// Only destructor is not abstract
PaintCmd :: ~PaintCmd() {}

/********************************************************************/
// Command history class for undo/redo

PaintCmdHistory :: PaintCmdHistory() {}

PaintCmdHistory :: ~PaintCmdHistory() {
    for ( auto x : executedCmdsList ) {
        delete x;
    }
    for ( auto x : undoneCmdsList ) {
        delete x;
    }
    executedCmdsList.clear();
    undoneCmdsList.clear();
}
bool PaintCmdHistory :: Undo() {
    if ( executedCmdsList.empty() ) {
        return false;
    }
    undoneCmdsList.push_front( executedCmdsList.front() );
    executedCmdsList.front()->UnExecute();
    executedCmdsList.pop_front();
    return true;
}

bool PaintCmdHistory :: Redo() {
    if ( undoneCmdsList.empty() ) {
        return false;
    }
    executedCmdsList.push_front( undoneCmdsList.front() );
    undoneCmdsList.front()->Execute();
    undoneCmdsList.pop_front();
    return true;
}

void PaintCmdHistory :: ExecuteCmd( PaintCmd *pCmd ) {
    pCmd->Execute();
    executedCmdsList.push_front( pCmd );
}

void  PaintCmdHistory :: DeleteLast() {
    executedCmdsList.pop_front();
}

bool PaintCmdHistory :: Empty() {
    return executedCmdsList.empty();
}
