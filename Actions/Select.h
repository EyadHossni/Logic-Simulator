#pragma once
#include "Action.h"
#include "../Components/Component.h"
#include "../Components/Gate.h"
#include "../ApplicationManager.h"

const int MaxCompCount = 100;

class Select :
    public Action
{
    Component* CompList[MaxCompCount];
    int CompCount;

public:
    Select(ApplicationManager* pApp, Component** Comps, int n);
    ~Select();
    void DeselectAll();
    bool ReadActionParameters(int& x, int& y);
    virtual bool Execute();
    virtual void Undo();
    virtual void Redo();
    void DrawToolBar(bool& refresh);
    int GetSelectionAmount();
};

