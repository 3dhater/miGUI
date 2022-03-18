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
#include "mgFunctions.h"

#include <assert.h>
#include <stdlib.h>
#include <wchar.h>

void miGUI_onUpdateTransform_rectangle(mgElement* e);
void miGUI_onUpdate_rectangle(mgElement* e);
void miGUI_onUpdateTransform_textinput(mgElement* e);
float lerp(float v0, float v1, float t);

struct lbData1
{
	const wchar_t* text;
};
struct lbData2
{
	const wchar_t* text;
	uint32_t flags;
};

int mgElementTable_textinput_onEndEdit(struct mgElement_s* e, int type)
{
	e->visible = 0;
	e->window->context->activeTextInput = 0;
	
	mgSetCursor_f(e->window->context, e->window->context->defaultCursors[mgCursorType_Arrow], mgCursorType_Arrow);

	mgElementTextInput* ti = e->implementation;
	mgElement* table = (mgElement*)e->userData;
	mgElementTable* tableImpl = (mgElementTable*)table->implementation;

	if (tableImpl->onCellTextInputEndEdit)
		return tableImpl->onCellTextInputEndEdit(table, type, ti->text, tableImpl->hoverRow, tableImpl->hoverRowIndex, tableImpl->hoverColIndex);

	return 1;
}

wchar_t mgElementTable_textinput_onCharEnter(struct mgElement_s* e, wchar_t c)
{
	mgElementTextInput* ti = e->implementation;
	mgElement* table = (mgElement*)e->userData;
	mgElementTable* tableImpl = (mgElementTable*)table->implementation;
	
	if (tableImpl->onCellTextInputCharEnter)
		return tableImpl->onCellTextInputCharEnter(table, c);

	return c;
}
void mgElementTable_textinput_onActivate(struct mgElement_s* e)
{
	mgElement* table = (mgElement*)e->userData;
	mgElementTable* tableImpl = (mgElementTable*)table->implementation;

	mgElementTextInput* ti = e->implementation;
	if (tableImpl->onCellTextInputActivate)
		mgTextInputSetText_f(ti, tableImpl->onCellTextInputActivate(table, tableImpl->hoverRow, tableImpl->hoverRowIndex, tableImpl->hoverColIndex));


	ti->isSelected = 1;
	ti->selectionStart = 0;
	ti->selectionEnd = ti->textLen;
	ti->textCursor = ti->textLen;
}

void
miGUI_onUpdateTransform_table(mgElement* e)
{
	miGUI_onUpdateTransform_rectangle(e);

	mgElementTable* impl = (mgElementTable*)e->implementation;
	impl->numOfLines = 0;
	int w = e->transformWorld.buildArea.bottom - e->transformWorld.buildArea.top;
	if (w > 0)
	{
		if (impl->rowHeight < 2)
			impl->rowHeight = 2;

		impl->numOfLines = (w / impl->rowHeight) + 4;
	}

	e->contentHeight = impl->numRows * impl->rowHeight;
}

void 
miGUI_onUpdate_table(mgElement* e)
{
	miGUI_onUpdate_rectangle(e);

	mgContext* c = e->window->context;
	mgElementTable* impl = (mgElementTable*)e->implementation;

	if (e->contentHeight > e->clientHeight)
	{
		if (mgPointInRect(&e->transformWorld.clipArea, &c->input->mousePosition))
		{
			if (c->input->mouseWheelDelta < 0.f)
			{
				impl->rowScrollValueTarget += impl->scrollSpeed;
				if (impl->rowScrollValueTarget > (e->contentHeight - e->clientHeight))
					impl->rowScrollValueTarget = (float)(e->contentHeight - e->clientHeight);
			}
			else if (c->input->mouseWheelDelta > 0.f)
			{
				impl->rowScrollValueTarget -= impl->scrollSpeed;

				if (impl->rowScrollValueTarget < 0.f)
					impl->rowScrollValueTarget = 0.f;
			}
		}

		impl->rowScrollValue = lerp(impl->rowScrollValue, impl->rowScrollValueTarget, 0.1f);
		impl->rowScrollValueWorld = impl->rowScrollValue;/*parent must be somewhere*/
	}

	if (impl->numRows && (impl->numRows > impl->numOfLines))
	{
		mgPoint pos;
		pos.x = e->transformWorld.buildArea.left;
		pos.y = e->transformWorld.buildArea.top;
		
		pos.y = pos.y + (impl->rowHeight * impl->firstRowIndexForDraw) - (int)impl->rowScrollValueWorld;

		int szY = e->transformWorld.buildArea.top - (int)impl->rowHeight;/*NEED TO MAKE SAME CHANGES FOR LISTBOX*/
		int nm = 1;/*NEED TO MAKE SAME CHANGES FOR LISTBOX*/
		if (pos.y < szY)
		{
			if (szY > (int)impl->rowHeight) /*NEED TO MAKE SAME CHANGES FOR LISTBOX*/
				nm = szY / pos.y;/*NEED TO MAKE SAME CHANGES FOR LISTBOX*/

			impl->firstRowIndexForDraw += nm;/*NEED TO MAKE SAME CHANGES FOR LISTBOX*/

			uint32_t lastIndex = impl->numRows - impl->numOfLines;
			if (impl->firstRowIndexForDraw > lastIndex)
				impl->firstRowIndexForDraw = lastIndex;
		}
		
		if (pos.y > szY)
		{

			/*it's still not good but better than nothing*/
			if (pos.y > (int)impl->rowHeight)/*NEED TO MAKE SAME CHANGES FOR LISTBOX*/
				nm = pos.y / szY;/*NEED TO MAKE SAME CHANGES FOR LISTBOX*/

			if (impl->firstRowIndexForDraw)
				impl->firstRowIndexForDraw -= nm;/*NEED TO MAKE SAME CHANGES FOR LISTBOX*/
		}
	}

	if (impl->hoverRow)
	{
	//	int issel = 0;
	//	
		
		int mouseBtn = 0;
		if (c->input->mouseButtonFlags1 & MG_MBFL_LMBDOWN)
			mouseBtn = 1;
		else if (c->input->mouseButtonFlags1 & MG_MBFL_RMBDOWN)
			mouseBtn = 2;
		else if (c->input->mouseButtonFlags1 & MG_MBFL_MMBDOWN)
			mouseBtn = 3;

		if (impl->onRowClick && mouseBtn)
			impl->onRowClick(e, impl->hoverRow, impl->hoverRowIndex, mouseBtn);

		if (impl->onCellClick && mouseBtn)
		{
			if (impl->onCellClick(e, impl->hoverRow, impl->hoverRowIndex, impl->hoverColIndex, mouseBtn))
			{
				mgElementTextInput* ti = (mgElementTextInput*)impl->textInput->implementation;
				impl->textInput->visible = 1;
				mgTextInputActivate_f(c, ti, 1, 0);
				mgRect r;
				r.left = impl->hoverCellClipRect.left - e->transformWorld.buildArea.left;
				r.right = r.left + (impl->hoverCellClipRect.right - impl->hoverCellClipRect.left);

				r.top = impl->hoverCellClipRect.top - e->transformWorld.buildArea.top;
				r.bottom = r.top + impl->rowHeight;

				r.top += (int)e->scrollValueWorld;
				r.bottom += (int)e->scrollValueWorld;
				impl->textInput->transformLocal.buildArea = r;
				impl->textInput->transformLocal.clipArea = r;
				impl->editRow = impl->hoverRow;

				miGUI_onUpdateTransform_textinput(impl->textInput);
			}

		}

			
			//issel = 1;

	//			if (impl->selectWithRMB && !issel)
	//			{
	//				if (c->input->mouseButtonFlags1 & MG_MBFL_RMBDOWN)
	//					issel = 1;
	//			}

	//			if (issel)
	//			{
	//				impl->clickedItems[impl->clickedItemsCurr] = impl->hoverRow;
	//				++impl->clickedItemsCurr;
	//				if (impl->clickedItemsCurr > 2)
	//					impl->clickedItemsCurr = 0;

	//				if (impl->isSelected)
	//				{
	//					uint32_t index = impl->hoverRow - (uint8_t*)impl->array;
	//					index /= impl->dataTypeSizeOf;

	//					if (index != impl->curSel)
	//					{
	//						if (impl->onSelect)
	//						{
	//							if (impl->onSelect(e, impl->hoverRow))
	//								impl->curSel = index;
	//						}
	//						else
	//						{
	//							impl->curSel = index;
	//						}
	//					}
	//					else if(!impl->noDeselect)
	//					{
	//						impl->isSelected = 0;
	//						impl->curSel = 0;
	//					}
	//				}
	//				else
	//				{
	//					if (impl->onSelect)
	//					{
	//						if (impl->onSelect(e, impl->hoverRow))
	//						{
	//							impl->isSelected = 1;
	//							impl->curSel = impl->hoverRow - (uint8_t*)impl->array;
	//							impl->curSel /= impl->dataTypeSizeOf;
	//						}
	//					}
	//					else
	//					{
	//						impl->isSelected = 1;
	//						impl->curSel = impl->hoverRow - (uint8_t*)impl->array;
	//						impl->curSel /= impl->dataTypeSizeOf;
	//					}
	//				}
	//			}

	//	if (edit)
	//	{
	//		mgElementTextInput* ti = (mgElementTextInput*)impl->textInput->implementation;
	//		impl->textInput->visible = 1;

	//		mgTextInputActivate_f(c, ti, 1, 0);
	//		mgRect r;
	//		r.left = 0;
	//		r.right = e->transformWorld.buildArea.right - e->transformWorld.buildArea.left;
	//		r.top = impl->hoverRowBuildRect.top - e->transformWorld.buildArea.top;
	//		r.bottom = r.top + impl->itemHeight;

	//		r.top += (int)e->scrollValueWorld;
	//		r.bottom += (int)e->scrollValueWorld;

	//		impl->textInput->transformLocal.buildArea = r;
	//		impl->textInput->transformLocal.clipArea = r;

	//		impl->editItem = impl->hoverRow;

	//		miGUI_onUpdateTransform_textinput(impl->textInput);

	//		/*
	//		* I need to select this item again...
	//		*/
	//		if (impl->multiselect)
	//		{
	//			struct lbData2* dptr = (struct lbData2*)impl->hoverRow;
	//			dptr->flags |= 0x1;
	//		}
	//		else
	//		{
	//			uint32_t index = impl->hoverRow - (uint8_t*)impl->array;
	//			index /= impl->dataTypeSizeOf;

	//			impl->isSelected = 1;
	//			impl->curSel = index;
	//		}
	//	}
	}
}

void 
miGUI_onDraw_table(mgElement* e)
{
	mgStyle* style = e->userStyle;
	if (!style)
		style = e->window->context->activeStyle;

	mgContext* ctx = e->window->context;

	mgElementTable* impl = (mgElementTable*)e->implementation;
	e->window->context->gpu->setClipRect(&e->transformWorld.clipArea);
	if (e->drawBG)
	{
		e->window->context->gpu->drawRectangle(mgDrawRectangleReason_tableBG, impl,
			&e->transformWorld.buildArea,
			&style->tableBG,
			&style->tableBG, 0, 0);
	}


	if (impl->rows && impl->font)
	{
		uint32_t index = impl->firstRowIndexForDraw;

		mgPoint pos;
		pos.x = e->transformWorld.buildArea.left;
		pos.y = e->transformWorld.buildArea.top - (int)impl->rowScrollValueWorld;
		pos.y = pos.y + (impl->rowHeight * index);

		//void* rowBegin = impl->rows[0];

		impl->hoverRow = 0;
		impl->hoverRowIndex = 0;
		impl->hoverColIndex = 0xFFFFFFFF;
		for (uint32_t i = 0; i < impl->numOfLines; ++i)
		{
			void* rowCurr = impl->rows[index];// rowBegin + (index * sizeof(void*));
		
			mgRect r;
			r.left = pos.x;
			r.top = pos.y;
			r.right = e->transformWorld.clipArea.right;
			r.bottom = r.top + impl->rowHeight;

			mgRect rClip = r;
			if (rClip.top < e->parent->transformWorld.clipArea.top)
				rClip.top = e->parent->transformWorld.clipArea.top;

			if (r.bottom < e->transformWorld.clipArea.top)
				goto end;

			if (r.top > e->transformWorld.clipArea.bottom)
				goto end;

			e->window->context->gpu->setClipRect(&e->transformWorld.clipArea);
			if (impl->drawItemBG)
			{
				mgColor* ibgc = &style->tableRowBG2;
				int rea = mgDrawRectangleReason_tableRowBG2;

				if (index)
				{
					if ((index % 2) != 0)
					{
						rea = mgDrawRectangleReason_tableRowBG1;
						ibgc = &style->tableRowBG1;
					}
				}

				e->window->context->gpu->drawRectangle(rea,
					impl,
					&r,
					ibgc,
					ibgc,
					0, 0);
			}


			if (mgPointInRect(&rClip, &ctx->input->mousePosition) && !impl->hoverRow && e->cursorInRect)
			{
				impl->hoverRowIndex = index;
				impl->hoverRow = rowCurr;
				impl->hoverRowClipRect = rClip;
				impl->hoverRowBuildRect = r;

				e->window->context->gpu->drawRectangle(mgDrawRectangleReason_tableHoverItemBG,
					impl,
					&r,
					&style->tableRowHoverBG,
					&style->tableRowHoverBG, 0, 0);
			}

/*

			if (impl->multiselect)
			{
				struct lbData2* dptr = (struct lbData2*)u8ptr_curr;
				str = dptr->text;
				if (dptr->flags & 0x1)
				{
					e->window->context->gpu->drawRectangle(mgDrawRectangleReason_listHoverItemBG,
						impl,
						&r,
						&style->listItemSelectedBG,
						&style->listItemSelectedBG, 0, 0);
				}
			}
			else
			{
				struct lbData1* dptr = (struct lbData1*)u8ptr_curr;
				str = dptr->text;

				if (impl->isSelected && (impl->curSel == index))
				{
					e->window->context->gpu->drawRectangle(mgDrawRectangleReason_listHoverItemBG,
						impl,
						&r,
						&style->listItemSelectedBG,
						&style->listItemSelectedBG, 0, 0);
				}
			}

			if(impl->hoverRow == u8ptr_curr)
				impl->hoverItemText = str;

		*/
			if (impl->onDrawRow)
			{
				mgPoint pos2 = pos;

				int isRowSelected = 0;
				if (impl->onIsRowSelected)
					isRowSelected = impl->onIsRowSelected(e, rowCurr);

				if (isRowSelected)
				{
					e->window->context->gpu->drawRectangle(
						mgDrawRectangleReason_tableRowSelectedBG,
						impl,
						&r,
						&style->tableRowSelectedBG,
						&style->tableRowSelectedBG,
						0, 0);
				}

				wchar_t* str = 0;
				uint32_t strLen = 0;
				for (uint32_t i2 = 0; i2 < impl->numCols; ++i2)
				{
					str = 0;
					strLen = 0;

					pos2.x += 2;

					mgRect rctCol;
					rctCol.left = 0;
					rctCol.top = 0;
					rctCol.right = 0;
					rctCol.bottom = 0;

					if (impl->onDrawRow(e, rowCurr, i2, &str, &strLen))
					{
						rctCol.left = pos2.x;
						rctCol.top = pos2.y;
						rctCol.right = rctCol.left + impl->colsSizes[i2];
						rctCol.bottom = rctCol.top + impl->rowHeight;

						if (rctCol.top < e->transformWorld.clipArea.top)
							rctCol.top = e->transformWorld.clipArea.top;
						if (rctCol.bottom > e->transformWorld.clipArea.bottom)
							rctCol.bottom = e->transformWorld.clipArea.bottom;

						e->window->context->gpu->setClipRect(&rctCol);

						if (impl->hoverRow == rowCurr)
						{
							if (mgPointInRect(&rctCol, &ctx->input->mousePosition))
							{
								impl->hoverColIndex = i2;
								impl->hoverCellClipRect = rctCol;
							}
						}

						int drwBg = 0;
						
						if (str)
							drwBg = 1;

						if (drwBg)
						{
							e->window->context->gpu->drawRectangle(
								mgDrawRectangleReason_tableCellBG,
								impl,
								&rctCol,
								&style->tableCellBG,
								&style->tableCellBG,
								0, 0);
						}

						if (str)
						{
							e->window->context->gpu->drawText(mgDrawTextReason_table,
								impl,
								&pos2,
								str,
								strLen,
								&style->tableCellText,
								impl->font);
						}
					}

					pos2.x = pos2.x + (rctCol.right - rctCol.left);
				}
			}

		end:;
			pos.y = pos.y + impl->rowHeight;

			++index;
			if (index >= impl->numRows)
				break;
		}
	}
}

void
miGUI_onRebuild_table(mgElement* e) 
{
}

MG_API
mgElement* MG_C_DECL
mgCreateTable_f(struct mgWindow_s* w, 
	mgPoint* position, 
	mgPoint* size, 
	void** rows, 
	uint32_t rowsSz, 
	uint32_t colNum, 
	mgFont* f)
{
	assert(w);
	assert(position);
	assert(size);

	mgElement* newElement = calloc(1, sizeof(mgElement));
	newElement->type = MG_TYPE_LIST;

	newElement->transformLocal.buildArea.left = position->x;
	newElement->transformLocal.buildArea.top = position->y;
	newElement->transformLocal.buildArea.right = position->x + size->x;
	newElement->transformLocal.buildArea.bottom = position->y + size->y;
	newElement->transformLocal.clipArea = newElement->transformLocal.buildArea;
	newElement->transformLocal.sz = *size;
	newElement->creationRect = newElement->transformLocal.buildArea;

	newElement->window = w;

	mgElementDefaultInit(newElement);
	newElement->onDraw = miGUI_onDraw_table;
	newElement->onUpdate = miGUI_onUpdate_table;
	newElement->onUpdateTransform = miGUI_onUpdateTransform_table;
	newElement->onRebuild = miGUI_onRebuild_table;

	newElement->implementation = calloc(1, sizeof(mgElementTable));
	mgElementTable* impl = (mgElementTable*)newElement->implementation;
	impl->rows = rows;
	impl->numRows = rowsSz;
	impl->numCols = colNum;
	impl->font = f;
	impl->rowHeight = 16;
	impl->drawItemBG = 1;
	impl->scrollSpeed = 10.f;
	size->x = 20;
	size->y = 20;
	impl->textInput = mgCreateTextInput_f(w, position, size, f);
	impl->textInput->userData = newElement;
	impl->textInput->visible = 0;
	((mgElementTextInput*)impl->textInput->implementation)->onActivate = mgElementTable_textinput_onActivate;
	((mgElementTextInput*)impl->textInput->implementation)->onEndEdit = mgElementTable_textinput_onEndEdit;
	((mgElementTextInput*)impl->textInput->implementation)->onCharEnter = mgElementTable_textinput_onCharEnter;
	mgSetParent_f(impl->textInput, newElement);

	mgSetParent_f(newElement, 0);
	return newElement;
}
