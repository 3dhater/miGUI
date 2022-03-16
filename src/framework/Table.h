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

#pragma once
#ifndef _MGF_TABLE_H_
#define _MGF_TABLE_H_

#include "Rectangle.h"

#include <string>

/*
* Например я собираю информацию о всех файлах на жёстком диске.
* Информация где-то хранится, и будет правильным использовать то же самое место и в таблице,
* то есть не создавать отдельные массивы.
* Ещё, для множества данных правильнее использовать linked list а не массив.
struct FileInfo // info for row
{
	string filename;
	u32 fileSize;
	string fileType;
	u32 date;
	MyStruct* data;
};

я храню всю информацию в списке
list<FileInfo> allFiles;
каждый FileInfo лежит отдельно от другого, не по порядку.

я хочу использовать allFiles в таблице.
Таблица работает по индексам (напр. начинать рисовать линию с такого идекса), значит всё равно необходим массив.

Например, в allFiles добавляю новую запись. Далее необходимо создать массив указателей, и каждый раз при
добавлении или удалении записи надо будет пересоздавать этот массив.
Значит, таблица будет работать с массивом указателей.

FileInfo** arrFileInfo = malloc(количество * sizeof(FileInfo*));
далее заполнение arrFileInfo

потом передаю таблице
table->rows = arrFileInfo;

При рисовании будет вызываться колбэк в который будет передан указатель на текущий FileInfo*, и 
 значение индекс - столбец. 
*/

namespace mgf
{
	enum TableFlag
	{
		TableFlag_select = 0x1,
	};

	class Table : public Element
	{
		mgElementTable_s* m_elementTable = 0;

		// size must be equal `column number`
		std::vector<uint32_t> m_colSizes;
		
		/*std::vector<mgTableRow_s*> m_rows;*/
	public:
		Table(Window* w, uint32_t colNum, Font*);
		virtual ~Table();

		void SetCurSel(uint32_t);
		uint32_t GetCurSel();
		
		void SetSelectWithRMB(bool);
		void SetDrawItemBG(bool);

	//	uint32_t GetArraySize();
		
	//	mgTableRow_s* AddRow();

		void SetFont(Font*);

		// 0 - no limit;
		void SetTextInputCharLimit(uint32_t);
		
		void SetData(void** arr, uint32_t arrSz);
		
		wchar_t(*onTextInputCharEnter)(Table*, wchar_t) = 0;
		/*
		* i:
		*	1 - Enter
		*   2 - click somewhere
		*   3 - Escape
		* str: new text
		* editItem: first byte for edited item
		*/
		int(*onTextInputEndEdit)(Table*, int i, const wchar_t* str, uint8_t* editItem) = 0;

		/*return 1 if need to select*/
		int(*onSelect)(Table*, uint8_t* item);

		/*
	* Return 0 - no draw
	*        1 - this is wchar_t* (you must set `text` and `textlen`)
	*
	*/
		int(*onDrawRow)(Table*, void* row, uint32_t col, wchar_t** text, uint32_t* textlen);
	};
}

#endif