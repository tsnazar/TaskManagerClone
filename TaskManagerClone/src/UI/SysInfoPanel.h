#pragma once
#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/grid.h>

#include "SysInfoTable.h"

class SysInfoPanel : public wxPanel
{
public:
	SysInfoPanel(wxWindow* Parent);

	virtual ~SysInfoPanel() = default;

	void UpdateState();
private:
	wxGrid* SysGrid;
	SysInfoTable* SysTable;
};