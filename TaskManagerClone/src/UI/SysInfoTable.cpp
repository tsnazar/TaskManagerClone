#include "SysInfoTable.h"

// ----------------------------------------------------------------------------
// SysInfoTable
// ----------------------------------------------------------------------------

void SysInfoTable::Update()
{
    SysInfo.ScanSysInfo();
}

int SysInfoTable::GetNumberRows()
{
    return Row_Max;
}

int SysInfoTable::GetNumberCols()
{
    return Col_Max;
}

bool SysInfoTable::IsEmptyCell(int Row, int Col)
{
    return false;
}

wxString SysInfoTable::GetValue(int Row, int WXUNUSED(Col))
{
    auto SysInfoData = SysInfo.GetSysInfo();
    switch (Row)
    {
    case Row_PageSize:
        return wxString::Format("%d", SysInfoData.PageSize);
    case Row_TotalPhys:
        return wxString::Format("%d", SysInfoData.TotalPhys);
    case Row_UsedPhys:
        return wxString::Format("%d", SysInfoData.UsedPhys);
    case Row_TotalVirtual:
        return wxString::Format("%d", SysInfoData.TotalVirtual);
    case Row_UsedVirtual:
        return wxString::Format("%d", SysInfoData.UsedVirtual);
    }

    return wxEmptyString;
}

wxString SysInfoTable::GetRowLabelValue(int Row)
{
    static const wxString headers[] = { "PageSize (bytes)",
         "TotalPhys (MB)",
         "UsedPhys (MB)",
         "TotalVirtual (MB)",
         "UsedVirtual (MB)"
    };
    return headers[Row];
}

wxString SysInfoTable::GetTypeName(int Row, int WXUNUSED(Col))
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

bool SysInfoTable::CanGetValueAs(int WXUNUSED(Row), int WXUNUSED(Col), const wxString& TypeName)
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

double SysInfoTable::GetValueAsDouble(int Row, int WXUNUSED(Col))
{
    auto SysInfoData = SysInfo.GetSysInfo();
    switch (Row)
    {
    case Row_PageSize:
        return SysInfoData.PageSize;
    case Row_TotalPhys:
        return SysInfoData.TotalPhys;
    case Row_UsedPhys:
        return SysInfoData.UsedPhys;
    case Row_TotalVirtual:
        return SysInfoData.TotalVirtual;
    case Row_UsedVirtual:
        return SysInfoData.UsedVirtual;

    default:
        wxFAIL_MSG("unexpected row");
        return -1;
    }
}