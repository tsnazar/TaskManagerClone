#include "TaskManagerApp.h"

#include "RabinKarpAlg.h"
#include <numeric>

bool TaskManagerApp::OnInit()
{
    ProcList = new ProcessList;
    ProcList->Scan();

	TaskManagerFrame* Frame = new TaskManagerFrame(ProcList);
	Frame->Show(true);

	return true;
}

// ----------------------------------------------------------------------------
// TaskManagerProcessTable
// ----------------------------------------------------------------------------


wxString TaskManagerProcessTable::GetTypeName(int WXUNUSED(Row), int Col)
{
    switch (Col)
    {
    case Col_Id:
    case Col_Status:
        return wxGRID_VALUE_NUMBER;
    case Col_CpuPercentage:
    case Col_MemoryUsage:
        return wxGRID_VALUE_FLOAT;
    case Col_UserName:
    case Col_Name:
        return wxString::Format("%s",wxGRID_VALUE_STRING);
    }

    wxFAIL_MSG("unknown column");

    return wxEmptyString;
}

TaskManagerProcessTable::TaskManagerProcessTable(ProcessList* ProcessList)
    : ProcList(ProcessList)
{
    const int NumItems = ProcList->GetCount();

    SortedOrder.reserve(NumItems);
    DisplayIds.resize(NumItems);

    std::iota(DisplayIds.begin(), DisplayIds.end(), 0);
    
    auto& Map = ProcList->GetMap();
    for (auto& Info : Map)
        SortedOrder.push_back(&Info.second);
}

void TaskManagerProcessTable::Update()
{
    ProcList->Scan();

    SortedOrder.clear();
 
    auto& Map = ProcList->GetMap();
    for (auto& Info : Map)
        SortedOrder.push_back(&Info.second);

    const int PrevRowsCount = DisplayIds.size();

    SortInternal();

    SearchInternal(this->SearchPromt);

    HandleResize(PrevRowsCount, DisplayIds.size());
}

template<bool SortOrderDescending>
void TaskManagerProcessTable::Sort(int Col)
{
    switch (Col)
    {
    case Col_Id:
        SortFunc = SortById<SortOrderDescending>;
        break;

    case Col_UserName:
        SortFunc = SortByUserName<SortOrderDescending>;
        break;

    case Col_Status:
        SortFunc = SortByStatus<SortOrderDescending>;
        break;

    case Col_CpuPercentage:
        SortFunc = SortByCpuPercentage<SortOrderDescending>;
        break;

    case Col_MemoryUsage:
        SortFunc = SortByMemoryPercentage<SortOrderDescending>;
        break;

    case Col_Name:
        SortFunc = SortByName<SortOrderDescending>;
        break;
    }
    
    SortInternal();
}

void TaskManagerProcessTable::SortInternal()
{
    if (SortFunc)
        std::sort(SortedOrder.begin(), SortedOrder.end(), SortFunc);
}

void TaskManagerProcessTable::Search(const std::string& SearchPromt)
{
    this->SearchPromt = std::move(SearchPromt);

    const int PrevRowsCount = DisplayIds.size();

    SearchInternal(this->SearchPromt);

    HandleResize(PrevRowsCount, DisplayIds.size());
}

void TaskManagerProcessTable::HandleResize(int PrevRowsCount, int RowsCount)
{
    const int AddedRows = RowsCount - PrevRowsCount;
    const bool SizeChanged = AddedRows != 0;

    if (SizeChanged)
        GetView()->ProcessTableMessage(this, AddedRows > 0 ? wxGRIDTABLE_NOTIFY_ROWS_APPENDED : wxGRIDTABLE_NOTIFY_ROWS_DELETED,
            AddedRows > 0 ? AddedRows : RowsCount,
            AddedRows > 0 ? -1 : -AddedRows);
}

void TaskManagerProcessTable::SearchInternal(const std::string& SearchPromt)
{
    if (!this->SearchPromt.size() > 0)
    {
        const int NumItems = SortedOrder.size();
        DisplayIds.resize(NumItems);

        std::iota(DisplayIds.begin(), DisplayIds.end(), 0);
    }
    else
    {
        DisplayIds.clear();
        for (int i = 0; i < SortedOrder.size(); ++i)
        {
            bool Match = RabinKarp(this->SearchPromt, SortedOrder[i]->Name) != -1;
            if (Match)
                DisplayIds.push_back(i);
        }
    }
}

int TaskManagerProcessTable::GetNumberRows()
{
    return DisplayIds.size();
}

int TaskManagerProcessTable::GetNumberCols()
{
    return Col_Max;
}

bool TaskManagerProcessTable::IsEmptyCell(int WXUNUSED(Row), int WXUNUSED(Col))
{
    return false;
}

wxString TaskManagerProcessTable::GetValue(int Row, int Col)
{
    if(Row > DisplayIds.size())
        wxFAIL_MSG("unexpected row");

    const ProcessInfo& Info = *SortedOrder[DisplayIds[Row]];

    switch (Col)
    {
    case Col_Id:
        return wxString::Format("%d", Info.Id);

    case Col_UserName:
        return wxString::Format("%s", Info.UserName);

    case Col_Status:
        return wxString::Format("%s", Info.Status);

    case Col_CpuPercentage:
        return wxString::Format("%d %", Info.CpuPercentage);

    case Col_MemoryUsage:
        return wxString::Format("%d Mb", Info.MemorySize);

    case Col_Name:
        return wxString::Format("%s", Info.Name);
    }

    return wxEmptyString;
}

bool TaskManagerProcessTable::CanGetValueAs(int WXUNUSED(Row), int Col, const wxString& TypeName)
{
    if (TypeName == wxGRID_VALUE_STRING)
    {
        return true;
    }
    else if (TypeName == wxGRID_VALUE_NUMBER)
    {
        return Col == Col_Id;
    }
    else if (TypeName == wxGRID_VALUE_FLOAT)
    {
        return Col == Col_CpuPercentage || Col == Col_MemoryUsage;
    }
    else
    {
        return false;
    }
}

long TaskManagerProcessTable::GetValueAsLong(int Row, int Col)
{
    if (Row > DisplayIds.size())
        wxFAIL_MSG("unexpected row");

    const ProcessInfo& Info = *SortedOrder[DisplayIds[Row]];
    
    switch(Col)
    {

    case Col_Id:
        return Info.Id;

    default:
        wxFAIL_MSG("unexpected column");
        return -1;
    }
}

double TaskManagerProcessTable::GetValueAsDouble(int Row, int Col)
{
    if (Row > DisplayIds.size())
        wxFAIL_MSG("unexpected row");

    const ProcessInfo& Info = *SortedOrder[DisplayIds[Row]];
    
    switch (Col)
    {
    case Col_CpuPercentage:
        return Info.CpuPercentage;

    case Col_MemoryUsage:
        return Info.MemorySize;

    default:
        wxFAIL_MSG("unexpected column");
        return -1;
    }
}

wxString TaskManagerProcessTable::GetColLabelValue(int Col)
{
    static const wxString headers[] = { "Id",
        "Name",
        "UserName",
        "Status",
        "CpuPercentage",
        "MemorySize" };
    return headers[Col];
}

// ----------------------------------------------------------------------------
// TaskManagerFrame
// ----------------------------------------------------------------------------

TaskManagerFrame::TaskManagerFrame(ProcessList* ProcessList)
	: wxFrame(nullptr, wxID_ANY, "CrippledTaskManager"), ProcList(ProcessList)
{
    Search = new wxSearchCtrl(this, wxID_ANY);
	ProcGrid = new wxGrid(this, wxID_ANY);
	ProcTable = new TaskManagerProcessTable(ProcList);

    SysGrid = new wxGrid(this, wxID_ANY);
    SysTable = new TaskManagerSysInfoTable(ProcList);

    wxTimer* Timer = new wxTimer(this);
    wxBoxSizer* MainSizer = new wxBoxSizer(wxVERTICAL);
	
    Search->ShowSearchButton(true);
    Search->ShowCancelButton(true);
    Search->SetMinClientSize(wxSize(150, 30));

    ProcGrid->AssignTable(ProcTable);
    ProcGrid->EnableEditing(false);
    ProcGrid->HideRowLabels();
    ProcGrid->SetSelectionMode(wxGrid::wxGridSelectNone);

    SysGrid->AssignTable(SysTable);
    SysGrid->EnableEditing(false);
    SysGrid->HideColLabels();
    SysGrid->SetSelectionMode(wxGrid::wxGridSelectNone);

    Timer->Start(Interval);

    MainSizer->Add(Search);
    MainSizer->AddSpacer(8);
    MainSizer->Add(SysGrid, 0, wxEXPAND);
    MainSizer->AddSpacer(8);
    MainSizer->Add(ProcGrid, 0, wxEXPAND);

    this->Bind(wxEVT_TIMER, &TaskManagerFrame::OnTimer, this);
    this->Bind(wxEVT_GRID_COL_SORT, &TaskManagerFrame::OnGridColSort, this);
    this->Bind(wxEVT_SEARCHCTRL_SEARCH_BTN, &TaskManagerFrame::OnSearch, this);
    this->Bind(wxEVT_SEARCHCTRL_CANCEL_BTN, &TaskManagerFrame::OnSearchCancel, this);
    this->SetSizer(MainSizer);

    SetClientSize(FromDIP(wxSize(500, 450)));
}

void TaskManagerFrame::OnTimer(wxTimerEvent& Ev)
{
    ProcTable->Update();

    SysTable->Update();

    ProcGrid->Refresh();

    SysGrid->Refresh();
}

void TaskManagerFrame::OnGridColSort(wxGridEvent& Ev)
{
    const int Col = Ev.GetCol();
    
    if (!Ev.IsSameAs(*ProcGrid))
        return;

    bool SortOrderDescending = !(ProcGrid->IsSortingBy(Col) && ProcGrid->IsSortOrderAscending());

    if(SortOrderDescending)
        ProcTable->Sort<1>(Col);
    else
        ProcTable->Sort<0>(Col);

    ProcGrid->Refresh();
}

void TaskManagerFrame::OnSearch(wxCommandEvent& Ev)
{
    std::string SearchPromt = std::string(Ev.GetString().mb_str());

    ProcTable->Search(SearchPromt);
    
    ProcGrid->Refresh();
}

void TaskManagerFrame::OnSearchCancel(wxCommandEvent& Ev)
{
    Search->Clear();

    ProcTable->Search("");

    ProcGrid->Refresh();
}

// ----------------------------------------------------------------------------
// TaskManagerSysInfoTable
// ----------------------------------------------------------------------------

TaskManagerSysInfoTable::TaskManagerSysInfoTable(ProcessList* ProcessList)
    : ProcList(ProcessList)
{
}

void TaskManagerSysInfoTable::Update()
{
    ProcList->ScanSysInfo();

    SysInfo = ProcList->GetSysInfo();
}

int TaskManagerSysInfoTable::GetNumberRows()
{
    return Row_Max;
}

int TaskManagerSysInfoTable::GetNumberCols()
{
    return Col_Max;
}

bool TaskManagerSysInfoTable::IsEmptyCell(int Row, int Col)
{
    return false;
}

wxString TaskManagerSysInfoTable::GetValue(int Row, int WXUNUSED(Col))
{
    switch (Row)
    {
    case Row_PageSize:
        return wxString::Format("%d", SysInfo.PageSize);
    case Row_TotalPhys:
        return wxString::Format("%d", SysInfo.TotalPhys);
    case Row_UsedPhys:
        return wxString::Format("%d", SysInfo.UsedPhys);
    case Row_TotalVirtual:
        return wxString::Format("%d", SysInfo.TotalVirtual);
    case Row_UsedVirtual:
        return wxString::Format("%d", SysInfo.UsedVirtual);
    }

    return wxEmptyString;
}

wxString TaskManagerSysInfoTable::GetRowLabelValue(int Row)
{
    static const wxString headers[] = { "PageSize",
         "TotalPhys",
         "UsedPhys",
         "TotalVirtual",
         "UsedVirtual"
    };
    return headers[Row];
}

wxString TaskManagerSysInfoTable::GetTypeName(int Row, int WXUNUSED(Col))
{
    switch (Row)
    {
    case Row_PageSize:
    case Row_TotalPhys:
    case Row_UsedPhys:
    case Row_TotalVirtual:
    case Row_UsedVirtual:
        return wxGRID_VALUE_FLOAT;
    }

    wxFAIL_MSG("unknown row");

    return wxEmptyString;
}

bool TaskManagerSysInfoTable::CanGetValueAs(int WXUNUSED(Row), int WXUNUSED(Col), const wxString& TypeName)
{
    if (TypeName == wxGRID_VALUE_STRING)
    {
        return true;
    }
    else if (TypeName == wxGRID_VALUE_FLOAT)
    {
        return true;
    }
    else
    {
        return false;
    }
}

double TaskManagerSysInfoTable::GetValueAsDouble(int Row, int WXUNUSED(Col))
{
    switch (Row)
    {
    case Row_PageSize:
        return SysInfo.PageSize;
    case Row_TotalPhys:
        return SysInfo.TotalPhys;
    case Row_UsedPhys:
        return SysInfo.UsedPhys;
    case Row_TotalVirtual:
        return SysInfo.TotalVirtual;
    case Row_UsedVirtual:
        return SysInfo.UsedVirtual;
    
    default:
        wxFAIL_MSG("unexpected row");
        return -1;
    }
}
