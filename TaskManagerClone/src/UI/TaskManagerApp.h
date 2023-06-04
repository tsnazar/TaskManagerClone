#pragma once
#include <wx/wxprec.h>
#include <wx/wx.h>

#include <wx/frame.h>

class wxTimerEvent;

class ProcessesPanel;
class SysInfoPanel;
class ProcessList;
class TaskManagerFrame;

class TaskManagerApp : public wxApp
{
public:
    virtual bool OnInit() override;
    virtual int OnExit() override;
private:
    TaskManagerFrame* Frame;
};


class TaskManagerFrame: public wxFrame
{
public:
    static const int Interval = 500;
public:
    TaskManagerFrame();

    void OnTimer(wxTimerEvent& Ev);
    void OnClose(wxCloseEvent& Ev);
private:
    wxTimer* Timer;

    ProcessesPanel* ProcPanel;
    SysInfoPanel* SysPanel;
};
