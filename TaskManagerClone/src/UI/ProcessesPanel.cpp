#include "ProcessesPanel.h"

ProcessesPanel::ProcessesPanel(wxWindow* Parent)
    : wxPanel(Parent)
{
    wxBoxSizer* PanelSizer = new wxBoxSizer(wxVERTICAL);

    Search = new wxSearchCtrl(this, wxID_ANY);
    ProcGrid = new wxGrid(this, wxID_ANY);
    ProcTable = new ProcessesTable();

    Search->ShowSearchButton(true);
    Search->ShowCancelButton(true);
    Search->SetMinClientSize(wxSize(150, 30));

    ProcGrid->AssignTable(ProcTable);
    ProcGrid->EnableEditing(false);
    ProcGrid->HideRowLabels();
    ProcGrid->SetSelectionMode(wxGrid::wxGridSelectNone);
    ProcGrid->SetMinClientSize(wxSize(400, 400));

    PanelSizer->Add(Search);
    PanelSizer->AddSpacer(8);
    PanelSizer->Add(ProcGrid, 0, wxEXPAND);

    this->SetSizer(PanelSizer);

    this->Bind(wxEVT_GRID_COL_SORT, &ProcessesPanel::OnGridColSort, this);
    this->Bind(wxEVT_SEARCHCTRL_SEARCH_BTN, &ProcessesPanel::OnSearch, this);
    this->Bind(wxEVT_SEARCHCTRL_CANCEL_BTN, &ProcessesPanel::OnSearchCancel, this);
}

void ProcessesPanel::UpdateState()
{
    ProcTable->Update();
    ProcGrid->Refresh();
}

void ProcessesPanel::OnGridColSort(wxGridEvent& Ev)
{
    if (!Ev.IsSameAs(*ProcGrid))
        return;

    const int Col = Ev.GetCol();

    bool SortOrderDescending = !(ProcGrid->IsSortingBy(Col) && ProcGrid->IsSortOrderAscending());

    if (SortOrderDescending)
        ProcTable->Sort<1>(Col);
    else
        ProcTable->Sort<0>(Col);

    ProcGrid->Refresh();
}

void ProcessesPanel::OnSearch(wxCommandEvent& Ev)
{
    std::string SearchPromt = std::string(Ev.GetString().mb_str());

    ProcTable->Search(SearchPromt);

    ProcGrid->Refresh();
}

void ProcessesPanel::OnSearchCancel(wxCommandEvent& Ev)
{
    Search->Clear();

    ProcTable->Search("");

    ProcGrid->Refresh();
}