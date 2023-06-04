#pragma once
#include <wx/wxprec.h>
#include <wx/wx.h>

#include <wx/grid.h>
#include <wx/srchctrl.h>
#include <wx/sizer.h>
#include <wx/notebook.h>

#include "../SysInfoFacade.h"

class SysInfoTable : public wxGridTableBase
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
    SysInfoTable() = default;

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
    SysInfoFacade SysInfo;
};