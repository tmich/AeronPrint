#include "OrderListCtrl.h"

wxBEGIN_EVENT_TABLE(OrderListCtrl, wxListCtrl)
	/*EVT_LIST_ITEM_ACTIVATED(LIST_CTRL, OrderListCtrl::OnActivated)
	EVT_LIST_BEGIN_DRAG(LIST_CTRL, OrderListCtrl::OnBeginDrag)
	EVT_LIST_BEGIN_RDRAG(LIST_CTRL, MyListCtrl::OnBeginRDrag)
	EVT_LIST_BEGIN_LABEL_EDIT(LIST_CTRL, MyListCtrl::OnBeginLabelEdit)
	EVT_LIST_END_LABEL_EDIT(LIST_CTRL, MyListCtrl::OnEndLabelEdit)
	EVT_LIST_DELETE_ITEM(LIST_CTRL, MyListCtrl::OnDeleteItem)
	EVT_LIST_DELETE_ALL_ITEMS(LIST_CTRL, MyListCtrl::OnDeleteAllItems)
	EVT_LIST_ITEM_SELECTED(LIST_CTRL, MyListCtrl::OnSelected)
	EVT_LIST_ITEM_DESELECTED(LIST_CTRL, MyListCtrl::OnDeselected)
	EVT_LIST_KEY_DOWN(LIST_CTRL, MyListCtrl::OnListKeyDown)
	EVT_LIST_ITEM_ACTIVATED(LIST_CTRL, MyListCtrl::OnActivated)
	EVT_LIST_ITEM_FOCUSED(LIST_CTRL, MyListCtrl::OnFocused)

	EVT_LIST_COL_CLICK(LIST_CTRL, MyListCtrl::OnColClick)
	EVT_LIST_COL_RIGHT_CLICK(LIST_CTRL, MyListCtrl::OnColRightClick)
	EVT_LIST_COL_BEGIN_DRAG(LIST_CTRL, MyListCtrl::OnColBeginDrag)
	EVT_LIST_COL_DRAGGING(LIST_CTRL, MyListCtrl::OnColDragging)
	EVT_LIST_COL_END_DRAG(LIST_CTRL, MyListCtrl::OnColEndDrag)

	EVT_LIST_CACHE_HINT(LIST_CTRL, MyListCtrl::OnCacheHint)

	#if USE_CONTEXT_MENU
	EVT_CONTEXT_MENU(MyListCtrl::OnContextMenu)
	#endif
	EVT_CHAR(MyListCtrl::OnChar)

	EVT_RIGHT_DOWN(MyListCtrl::OnRightClick)*/
wxEND_EVENT_TABLE()

//void OrderListCtrl::OnActivated(wxListEvent& event)
//{
//	//event.GetItem().SetBackgroundColour(wxColour(*wxWHITE));
//	int idx = event.GetIndex();
//	SetItemBackgroundColour(idx, wxColour(*wxWHITE));
//	wxMessageBox(event.GetText());
//	event.Veto();
//}

std::vector<int> OrderListCtrl::GetSelectedItems()
{
	int current = -1;
	std::vector<int> items;
	
	while (items.size() < GetSelectedItemCount())
	{
		current = GetNextItem(current, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		items.push_back(current);
	}

	return items;
}
