#pragma once
#include <wx/wxprec.h>
#include <wx/wx.h>

#include <wx/grid.h>
#include <wx/srchctrl.h>
#include <wx/sizer.h>

#include "ProcessList.h"

class TaskManagerApp : public wxApp
{
public:
    virtual bool OnInit() override;
public:
    ProcessList* ProcList;
};

class TaskManagerSysInfoTable : public wxGridTableBase
{
public:
    enum Columns
    {
        Col_Value,
        Col_Max
    };
    
    enum Rows
    {
        Row_PageSize,
        Row_TotalPhys,
        Row_UsedPhys,
        Row_TotalVirtual,
        Row_UsedVirtual,
        Row_Max
    };
public:
    TaskManagerSysInfoTable(ProcessList* ProcessList);

    void Update();

    virtual int GetNumberRows() override;
    virtual int GetNumberCols() override;
    virtual bool IsEmptyCell(int Row, int Col) override;
    virtual wxString GetValue(int Row, int Col) override;
    virtual void SetValue(int Row, int Col, const wxString& Value) override {};

    virtual wxString GetRowLabelValue(int Row) override;

    virtual wxString GetTypeName(int Row, int Col) override;
    virtual bool CanGetValueAs(int Row, int Col, const wxString& TypeName) override;

    virtual double GetValueAsDouble(int Row, int Col) override;

private:
    ProcessList* ProcList;

    GlobalSysInfo SysInfo;
};

class TaskManagerProcessTable : public wxGridTableBase
{
public:
    enum Columns
    {
        Col_Id,
        Col_Name,
        Col_UserName,
        Col_Status,
        Col_CpuPercentage,
        Col_MemoryUsage,
        Col_Max
    };
public:
    TaskManagerProcessTable(ProcessList* ProcessList);

    void Update();
    
    template<bool SortOrderDescending>
    void Sort(int Col);

    void Search(const std::string& SearchPromt);

    virtual int GetNumberRows() override;
    virtual int GetNumberCols() override;
    virtual bool IsEmptyCell(int Row, int Col) override;
    virtual wxString GetValue(int Row, int Col) override;
    virtual void SetValue(int Row, int Col, const wxString& Value) override {};

    virtual wxString GetColLabelValue(int Col) override;

    virtual wxString GetTypeName(int Row, int Col) override;
    virtual bool CanGetValueAs(int Row, int Col, const wxString& TypeName) override;

    virtual long GetValueAsLong(int Row, int Col) override;
    virtual double GetValueAsDouble(int Row, int Col) override;
private:
    void HandleResize(int PrevRowsCount, int RowsCount);

    void SortInternal();

    void SearchInternal(const std::string& SearchPromt);

public:
    ProcessList* ProcList;
    std::vector<ProcessInfo*> SortedOrder;
    std::vector<int> DisplayIds;

    std::string SearchPromt;
    std::function<bool(const ProcessInfo*, const ProcessInfo*)> SortFunc;
};

class TaskManagerFrame: public wxFrame
{
public:
    static const int Interval = 500;
public:
    TaskManagerFrame(ProcessList* ProcessList);

    void OnTimer(wxTimerEvent& Ev);
    void OnGridColSort(wxGridEvent& Ev);
    void OnSearch(wxCommandEvent& Ev);
    void OnSearchCancel(wxCommandEvent& Ev);

private:
    wxSearchCtrl* Search;
    
    wxGrid* ProcGrid;
    TaskManagerProcessTable* ProcTable;

    wxGrid* SysGrid;
    TaskManagerSysInfoTable* SysTable;

    ProcessList* ProcList;
};
