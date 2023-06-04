#pragma once
#include <wx/wxprec.h>
#include <wx/wx.h>

#include <wx/grid.h>
#include <wx/srchctrl.h>
#include <wx/sizer.h>
#include <wx/notebook.h>

#include "../ProcessList.h"

class ProcessesTable : public wxGridTableBase
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
    ProcessesTable() = default;

    void Update();

    template<bool SortOrderDescending>
    void Sort(int Col)
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
    };

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

private:
    ProcessList ProcList;

    std::vector<ProcessInfo*> SortedOrder;
    std::vector<int> DisplayIds;

    std::string SearchPromt;
    std::function<bool(const ProcessInfo*, const ProcessInfo*)> SortFunc;
};