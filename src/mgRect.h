﻿/*
  Copyright (C) 2021 Basov Artyom
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
#ifndef _MG_RECT_H_
#define _MG_RECT_H_

typedef struct mgRect_s {
	int left;
	int top;
	int right;
	int bottom;

#if defined(__cplusplus)
	mgRect_s() 
		:
		left(0), 
		top(0), 
		right(0), 
		bottom(0) 
	{}

	mgRect_s(int Left, int Top, int Right, int Bottom) 
		:
		left(Left), 
		top(Top), 
		right(Right), 
		bottom(Bottom) 
	{}
#endif

} mgRect;

inline
void mgRectSet(mgRect* r, int Left, int Top, int Right, int Bottom)
{
	r->left = Left;
	r->top = Top;
	r->right = Right;
	r->bottom = Bottom;
}

inline
void mgRectSetPoint(mgRect* r, mgPoint* p1, mgPoint* p2)
{
	r->left = p1->x;
	r->top = p1->y;
	r->right = p2->x;
	r->bottom = p2->y;
}

inline
unsigned int mgPointInRect(mgRect* r, mgPoint* p)
{
	if (p->x >= r->left) { if (p->x <= r->right) { if (p->y >= r->top) { if (p->y <= r->bottom) { return 1; } } } }
	return 0;
}

#endif