﻿/*
  Copyright (C) 2022 Basov Artyom
  The authors can be contacted at <artembasov@outlook.com>
  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
	 notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
	 notice, this list of conditions and the following disclaimer in
	 the documentation and/or other materials provided with the
	 distribution.
  3. The names of the authors may not be used to endorse or promote
	 products derived from this software without specific prior
	 written permission.
  THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS
  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
  IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
  IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "miGUI.h"

#include "framework/mgf.h"
#include "framework/Window.h"
#include "framework/Rectangle.h"
#include "framework/Text.h"
#include "framework/Button.h"
#include "framework/TextInput.h"
#include "framework/ListBox.h"
#include "framework/Table.h"
#include "framework/FontImpl.h"

using namespace mgf;

Window::Window()
{
}

Window::~Window()
{
	auto elements = m_elements.data();
	for (size_t i = 0, sz = m_elements.size(); i < sz; ++i)
	{
		delete(elements[i]);
	}

	if(m_window)
		mgDestroyWindow(m_window);
}

void Window::SetTitle(const wchar_t* t, Font* optionalFont)
{
	m_title.assign(t);
	if (optionalFont)
	{
		FontImpl* f = (FontImpl*)optionalFont;
		m_window->titlebarFont = f->m_font;
	}
	mgSetWindowTitle(m_window, m_title.data());
}

void Window::SetVisible(bool v)
{
	m_isVisible = v;
	mgShowWindow(m_window, v ? 1 : 0);
}

bool Window::IsVisible()
{
	return m_isVisible;
}

void Window::SetWithCloseButton(bool v)
{
	if (v)
	{
		m_window->flags |= mgWindowFlag_closeButton;
	}
	else
	{
		if (m_window->flags & mgWindowFlag_closeButton)
			m_window->flags ^= mgWindowFlag_closeButton;
	}
}

void Window::SetWithCollapseButton(bool v)
{
	if (v)
	{
		m_window->flags |= mgWindowFlag_collapseButton;
	}
	else
	{
		if (m_window->flags & mgWindowFlag_collapseButton)
			m_window->flags ^= mgWindowFlag_collapseButton;
	}
}

void Window::SetWithTitlebar(bool v)
{
	if (v)
	{
		m_window->flags |= mgWindowFlag_withTitlebar;
	}
	else
	{
		if (m_window->flags & mgWindowFlag_withTitlebar)
			m_window->flags ^= mgWindowFlag_withTitlebar;
	}
}

void Window::SetCanMove(bool v)
{
	if (v)
	{
		m_window->flags |= mgWindowFlag_canMove;
	}
	else
	{
		if (m_window->flags & mgWindowFlag_canMove)
			m_window->flags ^= mgWindowFlag_canMove;
	}
}

void Window::UpdateRect()
{
	m_window->rect.left = m_window->position.x;
	m_window->rect.top = m_window->position.y;
	m_window->rect.right = m_window->rect.left + m_window->size.x;
	m_window->rect.bottom = m_window->rect.top + m_window->size.y;
}

void Window::SetSize(int x, int y)
{
	m_window->size.x = x;
	m_window->size.y = y;
	UpdateRect();
}

void Window::SetPosition(int x, int y)
{
	m_window->position.x = x;
	m_window->position.y = y;
	UpdateRect();
}

void Window::SetDrawBG(bool v)
{
	if (v)
	{
		m_window->flags |= mgWindowFlag_drawBG;
	}
	else
	{
		if (m_window->flags & mgWindowFlag_drawBG)
			m_window->flags ^= mgWindowFlag_drawBG;
	}
}

void Window::SetCanDock(bool v)
{
	if (v)
	{
		m_window->flags |= mgWindowFlag_canDock;
	}
	else
	{
		if (m_window->flags & mgWindowFlag_canDock)
			m_window->flags ^= mgWindowFlag_canDock;
	}
}

void Window::SetCanResize(bool v)
{
	if (v)
	{
		m_window->flags |= mgWindowFlag_canResize;
	}
	else
	{
		if (m_window->flags & mgWindowFlag_canResize)
			m_window->flags ^= mgWindowFlag_canResize;
	}
}

void Window::SetCanToTop(bool v)
{
	if (v)
	{
		if (m_window->flags & mgWindowFlag_noToTop)
			m_window->flags ^= mgWindowFlag_noToTop;
	}
	else
	{
		m_window->flags |= mgWindowFlag_noToTop;
	}
}

void Window::SetID(int i)
{
	m_window->id = i;
}

int Window::GetID()
{
	return m_window->id;
}

void Window::SetUserData(void* d)
{
	m_window->userData = d;
}

void* Window::GetUserData()
{
	return m_window->userData;
}

//void Window::DeleteElement(Element* e)
//{
//	assert(e);
//	auto elements = m_elements.data();
//	for (size_t i = 0, sz = m_elements.size(); i < sz; ++i)
//	{
//		if (elements[i] != e)
//			continue;
//
//		m_elements.erase(m_elements.begin() + i);
//		break;
//	}
//	delete e;
//}
//
//mgf::Rectangle* Window::AddRectangle()
//{
//	Rectangle* e = new Rectangle(this);
//	m_elements.emplace_back(e);
//	return e;
//}
//
//Text* Window::AddText(int x, int y, const wchar_t* text, Font* f)
//{
//	Text* e = new Text(this, text, f);
//	m_elements.emplace_back(e);
//	e->SetPosition(x, y);
//	return e;
//}
//
//Button* Window::AddButton()
//{
//	Button* e = new Button(this);
//	m_elements.emplace_back(e);
//	return e;
//}
//
//TextInput* Window::AddTextInput(Font* f)
//{
//	TextInput* e = new TextInput(this, f);
//	m_elements.emplace_back(e);
//	return e;
//}
//
//ListBox* Window::AddListBox(Font* f)
//{
//	ListBox* e = new ListBox(this, f);
//	m_elements.emplace_back(e);
//	return e;
//}
//
//Table* Window::AddTable(uint32_t colNum, Font* f)
//{
//	Table* e = new Table(this, colNum, f);
//	m_elements.emplace_back(e);
//	return e;
//}

void Window::UseMenu(bool v, Font* f)
{
	m_menuFont = f;

	if (m_menu)
		m_menu->font = ((FontImpl*)f)->m_font;

	if (v)
		m_window->menu = m_menu;
	else
		m_window->menu = 0;
}

void Window::RebuildMenu()
{
	bool isUse = m_window->menu != nullptr;
	

	if (m_menu)
		mgDestroyMenu(m_menu);
	
	mgMenuItemInfo* mii = new mgMenuItemInfo[m_menuItems.size()];

	for (uint32_t i = 0; i < m_menuItems.size(); ++i)
	{
		mii[i].popup = 0;
		mii[i].text = m_menuItems[i].m_title.c_str();
	}

	m_menu = mgCreateMenu(m_window->context, mii, m_menuItems.size(), ((FontImpl*)m_menuFont)->m_font);

	delete[] mii;

	UseMenu(true, m_menuFont);
}

void Window::DeleteMenu()
{
	UseMenu(false, m_menuFont);
	if (m_menu)
		mgDestroyMenu(m_menu);
	m_menu = 0;
	m_menuItems.clear();
}

void Window::BeginMenu(const wchar_t* title)
{
	m_menu_currItem.m_title = title;
}

void Window::EndMenu()
{
	m_menuItems.push_back(m_menu_currItem);
}

void Window::Draw()
{
	mgDrawWindow(m_window);
}

void Window::SetUserStyle(mgStyle_s* s)
{
	m_window->userStyle = s;
}

void Window::SetNoMenuBG(bool v)
{
	if (v)
	{
		m_window->flags |= mgWindowFlag_noMenuBG;
	}
	else
	{
		if (m_window->flags & mgWindowFlag_noMenuBG)
			m_window->flags ^= mgWindowFlag_noMenuBG;
	}
}

mgRect* Window::GetMenuRect()
{
	return &m_window->menuRect;
}