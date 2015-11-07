#include "my_printing.h"



MyHtmlEasyPrinting::MyHtmlEasyPrinting(const wxString & name, wxWindow * parent) : wxHtmlEasyPrinting(name, parent)
{
	m_ParentWindow = parent;
}

MyHtmlEasyPrinting::~MyHtmlEasyPrinting()
{
}

