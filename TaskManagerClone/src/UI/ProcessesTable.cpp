#include "ProcessesTable.h"
#include "../RabinKarpAlg.h"

#include <numeric>
// ----------------------------------------------------------------------------
// ProcessesTable
// ----------------------------------------------------------------------------

wxString ProcessesTable::GetTypeName(int WXUNUSED(Row), int Col)
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
        return wxString::Format("%s", wxGRID_VALUE_STRING);
    }

    wxFAIL_MSG("unknown column");

    return wxEmptyString;
}

void ProcessesTable::Update()
{
    ProcList.ScanProcesses();

    SortedOrder.clear();

    auto& Map = ProcList.GetMap();
    for (auto& Info : Map)
        SortedOrder.push_back(&Info.second);

    const int PrevRowsCount = DisplayIds.size();

    SortInternal();

    SearchInternal(this->SearchPromt);

    HandleResize(PrevRowsCount, DisplayIds.size());
}

void ProcessesTable::SortInternal()
{
    if (SortFunc)
        std::sort(SortedOrder.begin(), SortedOrder.end(), SortFunc);
}

void ProcessesTable::Search(const std::string& SearchPromt)
{
    this->SearchPromt = std::move(SearchPromt);

    const int PrevRowsCount = DisplayIds.size();

    SearchInternal(this->SearchPromt);

    HandleResize(PrevRowsCount, DisplayIds.size());
}

void ProcessesTable::HandleResize(int PrevRowsCount, int RowsCount)
{
    const int AddedRows = RowsCount - PrevRowsCount;
    const bool SizeChanged = AddedRows != 0;

    if (SizeChanged)
        GetView()->ProcessTableMessage(this, AddedRows > 0 ? wxGRIDTABLE_NOTIFY_ROWS_APPENDED : wxGRIDTABLE_NOTIFY_ROWS_DELETED,
            AddedRows > 0 ? AddedRows : RowsCount,
            AddedRows > 0 ? -1 : -AddedRows);
}

void ProcessesTable::SearchInternal(const std::string& SearchPromt)
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

int ProcessesTable::GetNumberRows()
{
    return DisplayIds.size();
}

int ProcessesTable::GetNumberCols()
{
    return Col_Max;
}

bool ProcessesTable::IsEmptyCell(int WXUNUSED(Row), int WXUNUSED(Col))
{
    return false;
}

wxString ProcessesTable::GetValue(int Row, int Col)
{
    if (Row > DisplayIds.size())
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
        return wxString::Format("%d", Info.CpuPercentage);

    case Col_MemoryUsage:
        return wxString::Format("%d", Info.MemorySize);

    case Col_Name:
        return wxString::Format("%s", Info.Name);
    }

    return wxEmptyString;
}

bool ProcessesTable::CanGetValueAs(int WXUNUSED(Row), int Col, const wxString& TypeName)
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

long ProcessesTable::GetValueAsLong(int Row, int Col)
{
    if (Row > DisplayIds.size())
        wxFAIL_MSG("unexpected row");

    const ProcessInfo& Info = *SortedOrder[DisplayIds[Row]];

    switch (Col)
    {

    case Col_Id:
        return Info.Id;

    default:
        wxFAIL_MSG("unexpected column");
        return -1;
    }
}

double ProcessesTable::GetValueAsDouble(int Row, int Col)
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

wxString ProcessesTable::GetColLabelValue(int Col)
{
    static const wxString headers[] = { "Id",
        "Name",
        "UserName",
        "Status",
        "CpuPercentage (%)",
        "MemorySize (MB)" };
    return headers[Col];
}
