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

#ifndef _MG_BCK_GDI_H_
#define _MG_BCK_GDI_H_

#ifdef MGF_BACKEND_GDI

#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>
//#pragma comment (lib,"Gdiplus.lib")

#ifdef DrawText
#undef DrawText
#endif

#ifdef CreateFont
#undef CreateFont
#endif

#ifdef CreateWindow
#undef CreateWindow
#endif

#ifdef LoadImage
#undef LoadImage
#endif

namespace mgf
{
	class SystemWindowImpl;
	class Context;

	class BackendGDI : public Backend
	{
		friend class Context;

		mgf::SystemWindowImpl* m_window = 0;
		mgf::Context* m_context = 0;

		void* m_gpu = 0;
		Gdiplus::GdiplusStartupInput m_gdiplusStartupInput = 0;
		ULONG_PTR m_gdiplusToken = 0;

		//Gdiplus::Bitmap* m_gdiimage_defaultIcons = 0;
		mgTexture_s* m_defaultIcons = 0;
		Font* m_defaultFont = 0;

		mgRect m_clipRect;

		void _createBackbuffer(mgf::SystemWindowImpl* impl);

	public:
		BackendGDI();
		virtual ~BackendGDI();

		virtual void* GetVideoDriverAPI() override;

		virtual void InitWindow(mgf::SystemWindow*) override;
		virtual void SetActiveWindow(mgf::SystemWindow*) override;
		virtual void SetActiveContext(mgf::Context*) override;

		virtual void UpdateBackbuffer() override;

		virtual void BeginDraw() override;
		virtual void EndDraw() override;

		virtual mgTexture* CreateTexture(mgImage* img)  override;
		virtual void DestroyTexture(mgTexture*)  override;
		virtual void DrawRectangle(int reason,void* object,mgRect* rct,mgColor* color1,mgColor* color2,
			mgTexture* texture,  mgVec4* UVRegion ) override;
		virtual void DrawText(int reason,void* object,mgPoint* position,const wchar_t* text,int textLen,
			mgColor* color, mgFont* font) override;
		virtual void DrawLine( int reason, void* object, mgPoint* position, mgPoint* where,
			mgColor* color, int size) override;
		virtual mgRect SetClipRect(mgRect* r) override;
		virtual void GetTextSize(const wchar_t* text, mgFont* font, mgPoint* sz) override;
		virtual mgTexture_s* GetDefaultIcons() override;
		virtual Font* CreateFont(const wchar_t* file, int size, bool bold, bool italic) override;
		virtual void DestroyFont(Font*) override;
		virtual Font* GetDefaultFont() override;
	};

}

#endif

#endif