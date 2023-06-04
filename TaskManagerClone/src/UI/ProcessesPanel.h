#pragma once
#include <wx/wxprec.h>
#include <wx/wx.h>

#include <wx/grid.h>
#include <wx/srchctrl.h>
#include <wx/sizer.h>
#include <wx/notebook.h>

#include "ProcessesTable.h"

class ProcessesPanel : public wxPanel
{
public:
	ProcessesPanel(wxWindow* Parent);

	virtual ~ProcessesPanel() = default;

	void UpdateState();

	void OnGridColSort(wxGridEvent& Ev);
	void OnSearch(wxCommandEvent& Ev);
	void OnSearchCancel(wxCommandEvent& Ev);

private:
	wxSearchCtrl* Search;

	wxGrid* ProcGrid;
	ProcessesTable* ProcTable;
};