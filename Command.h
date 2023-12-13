#ifndef Command_h
#define Command_h

#include <forward_list>

/********************************************************************/
// Command class for command pattern
class PaintCmd {
public:
    virtual ~PaintCmd();
    virtual void Execute() = 0;
    virtual void UnExecute() = 0;
    virtual void Print() = 0;
};

/********************************************************************/
// Command history class for undo/redo
class PaintCmdHistory {
public:
    PaintCmdHistory();
    virtual ~PaintCmdHistory();
    bool Undo();
    bool Redo();
    void ExecuteCmd( PaintCmd *pCmd );
    void DeleteLast();
    bool Empty();

private:
    std::forward_list<PaintCmd *> executedCmdsList;
    std::forward_list<PaintCmd *> undoneCmdsList;
};

#endif