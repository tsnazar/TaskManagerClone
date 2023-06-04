#include "SysInfoPanel.h"

#include <wx/sizer.h>

SysInfoPanel::SysInfoPanel(wxWindow* Parent)
    : wxPanel(Parent)
{
    wxBoxSizer* PanelSizer = new wxBoxSizer(wxVERTICAL);

    SysGrid = new wxGrid(this, wxID_ANY);
    SysTable = new SysInfoTable();

    SysGrid->AssignTable(SysTable);
    SysGrid->EnableEditing(false);
    SysGrid->HideColLabels();
    SysGrid->SetSelectionMode(wxGrid::wxGridSelectNone);
    SysGrid->SetMinClientSize(wxSize(300, 300));
    SysGrid->SetRowLabelSize(200);

    PanelSizer->Add(SysGrid, 0, wxEXPAND);

    this->SetSizer(PanelSizer);
}

void SysInfoPanel::UpdateState()
{
    SysTable->Update();
    SysGrid->Refresh();
}
