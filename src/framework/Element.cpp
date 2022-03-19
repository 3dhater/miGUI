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
#include "framework/Element.h"

using namespace mgf;

void Element_onMouseEnter(struct mgElement_s* e) { 
	Element* el = (Element*)e->userData; 
	if (el->onMouseEnter)
		el->onMouseEnter(el);
}
void Element_onMouseLeave(struct mgElement_s* e) {
	Element* el = (Element*)e->userData;
	if (el->onMouseLeave)
		el->onMouseLeave(el);
}
void Element_onClickLMB(struct mgElement_s* e) {
	Element* el = (Element*)e->userData;
	if (el->onClickLMB)
		el->onClickLMB(el);
}
void Element_onClickRMB(struct mgElement_s* e) {
	Element* el = (Element*)e->userData;
	if (el->onClickRMB)
		el->onClickRMB(el);
}
void Element_onClickMMB(struct mgElement_s* e) {
	Element* el = (Element*)e->userData;
	if (el->onClickMMB)
		el->onClickMMB(el);
}
void Element_onClickX1MB(struct mgElement_s* e) {
	Element* el = (Element*)e->userData;
	if (el->onClickX1MB)
		el->onClickX1MB(el);
}
void Element_onClickX2MB(struct mgElement_s* e) {
	Element* el = (Element*)e->userData;
	if (el->onClickX2MB)
		el->onClickX2MB(el);
}
void Element_onReleaseLMB(struct mgElement_s* e) {
	Element* el = (Element*)e->userData;
	if (el->onReleaseLMB)
		el->onReleaseLMB(el);
}
void Element_onReleaseRMB(struct mgElement_s* e) {
	Element* el = (Element*)e->userData;
	if (el->onReleaseRMB)
		el->onReleaseRMB(el);
}
void Element_onReleaseMMB(struct mgElement_s* e) {
	Element* el = (Element*)e->userData;
	if (el->onReleaseMMB)
		el->onReleaseMMB(el);
}
void Element_onReleaseX1MB(struct mgElement_s* e) {
	Element* el = (Element*)e->userData;
	if (el->onReleaseX1MB)
		el->onReleaseX1MB(el);
}
void Element_onReleaseX2MB(struct mgElement_s* e) {
	Element* el = (Element*)e->userData;
	if (el->onReleaseX2MB)
		el->onReleaseX2MB(el);
}


Element::Element()
{
}

Element::~Element()
{
}

void Element::PostInit()
{
	m_element->onMouseEnter = Element_onMouseEnter;
	m_element->onMouseLeave = Element_onMouseLeave;
	m_element->onClickLMB = Element_onClickLMB;
	m_element->onClickRMB = Element_onClickRMB;
	m_element->onClickMMB = Element_onClickMMB;
	m_element->onClickX1MB = Element_onClickX1MB;
	m_element->onClickX2MB = Element_onClickX2MB;
	m_element->onReleaseLMB = Element_onReleaseLMB;
	m_element->onReleaseRMB = Element_onReleaseRMB;
	m_element->onReleaseMMB = Element_onReleaseMMB;
	m_element->onReleaseX1MB = Element_onReleaseX1MB;
	m_element->onReleaseX2MB = Element_onReleaseX2MB;
	m_element->userData = this;
}

void Element::SetUserData(void* d)
{
	m_userData = d;
}

void* Element::GetUserData()
{
	return m_userData;
}

void Element::SetVisible(bool v)
{
	m_element->visible = v ? 1 : 0;
}

bool Element::IsVisible()
{
	return (bool)m_element->visible;
}

void Element::SetID(int id)
{
	m_element->id = id;
}

int Element::GetID()
{
	return m_element->id;
}

void Element::SetRect(mgRect* r)
{
	assert(m_element);
	m_element->transformLocal.buildArea = *r;
	m_element->transformLocal.clipArea = *r;
	m_element->creationRect = *r;
	m_element->transformWorld = m_element->transformLocal;
}

void Element::SetRect(int left, int top, int right, int bottom)
{
	assert(m_element);
	m_element->transformLocal.buildArea.left = left;
	m_element->transformLocal.buildArea.top = top;
	m_element->transformLocal.buildArea.right = right;
	m_element->transformLocal.buildArea.bottom = bottom;
	m_element->transformLocal.clipArea.left = left;
	m_element->transformLocal.clipArea.top = top;
	m_element->transformLocal.clipArea.right = right;
	m_element->transformLocal.clipArea.bottom = bottom;
	m_element->creationRect.left = left;
	m_element->creationRect.top = top;
	m_element->creationRect.right = right;
	m_element->creationRect.bottom = bottom;
	m_element->transformWorld = m_element->transformLocal;
}

void Element::SetRectLeft(int v)
{
	m_element->transformLocal.buildArea.left = v;
	m_element->transformLocal.clipArea.left = v;
	m_element->creationRect.left = v;
	m_element->transformWorld = m_element->transformLocal;
}

void Element::SetRectTop(int v)
{
	m_element->transformLocal.buildArea.top = v;
	m_element->transformLocal.clipArea.top = v;
	m_element->creationRect.top = v;
	m_element->transformWorld = m_element->transformLocal;
}

void Element::SetRectRight(int v)
{
	m_element->transformLocal.buildArea.right = v;
	m_element->transformLocal.clipArea.right = v;
	m_element->creationRect.right = v;
	m_element->transformWorld = m_element->transformLocal;
}

void Element::SetRectBottom(int v)
{
	m_element->transformLocal.buildArea.bottom = v;
	m_element->transformLocal.clipArea.bottom = v;
	m_element->creationRect.bottom = v;
	m_element->transformWorld = m_element->transformLocal;
}

ElementAlignment Element::GetAlignment()
{
	return m_alignment;
}

void Element::SetAlignment(ElementAlignment a)
{
	m_alignment = a;
	switch (a)
	{
	case mgf::ElementAlignment::LeftTop:
		m_element->align = mgAlignment_leftTop;
		break;
	case mgf::ElementAlignment::Top:
		m_element->align = mgAlignment_top;
		break;
	case mgf::ElementAlignment::RightTop:
		m_element->align = mgAlignment_rightTop;
		break;
	case mgf::ElementAlignment::Right:
		m_element->align = mgAlignment_right;
		break;
	case mgf::ElementAlignment::RightBottom:
		m_element->align = mgAlignment_rightBottom;
		break;
	case mgf::ElementAlignment::Bottom:
		m_element->align = mgAlignment_bottom;
		break;
	case mgf::ElementAlignment::LeftBottom:
		m_element->align = mgAlignment_leftBottom;
		break;
	case mgf::ElementAlignment::Left:
		m_element->align = mgAlignment_left;
		break;
	case mgf::ElementAlignment::Center:
		m_element->align = mgAlignment_center;
		break;
	}
}

void Element::SetUserStyle(mgStyle_s* s)
{
	m_element->userStyle = s;
}

void Element::SetDrawBG(bool v)
{
	m_element->drawBG = v ? 1 : 0;
}

bool Element::IsDrawBG()
{
	return m_element->drawBG;
}

void Element::SetEnabled(bool v)
{
	m_element->enabled = v ? 1 : 0;
}

bool Element::IsEnabled()
{
	return m_element->enabled;
}

mgElement_s* Element::GetElement()
{
	return m_element;
}
