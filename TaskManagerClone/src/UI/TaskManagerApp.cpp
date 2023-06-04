#include "TaskManagerApp.h"

#include "ProcessesPanel.h"
#include "SysInfoPanel.h"

#include <wx/timer.h>
#include <wx/notebook.h>

// ----------------------------------------------------------------------------
// TaskManagerApp
// ----------------------------------------------------------------------------

bool TaskManagerApp::OnInit()
{
	Frame = new TaskManagerFrame();
	Frame->Show();

	return true;
}

int TaskManagerApp::OnExit()
{
    Frame->Close();
    return 0;
}

// ----------------------------------------------------------------------------
// TaskManagerFrame
// ----------------------------------------------------------------------------

TaskManagerFrame::TaskManagerFrame()
	: wxFrame(nullptr, wxID_ANY, "CrippledTaskManager")
{
    wxNotebook* TabControl = new wxNotebook(this, wxID_ANY, { 10, 10 }, { 370, 250 });

    ProcPanel = new ProcessesPanel(TabControl);
    SysPanel = new SysInfoPanel(TabControl);

    Timer = new wxTimer(this);
    
    TabControl->AddPage(ProcPanel, "Processes", true);
    TabControl->AddPage(SysPanel, "SystemInfo");

    Timer->Start(Interval);

    this->Bind(wxEVT_TIMER, &TaskManagerFrame::OnTimer, this);
    this->Bind(wxEVT_CLOSE_WINDOW, &TaskManagerFrame::OnClose, this);
    SetClientSize(FromDIP(wxSize(500, 450)));
}

void TaskManagerFrame::OnTimer(wxTimerEvent& Ev)
{
    ProcPanel->UpdateState();
    SysPanel->UpdateState();
}

void TaskManagerFrame::OnClose(wxCloseEvent& Ev)
{
    delete Timer;

    Destroy();
}
