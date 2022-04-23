/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2004-2005 David Schultz <das@FreeBSD.ORG>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */

// Modified for migui 

#pragma once
#ifndef __FENV__
#define __FENV__

#include "CRT.h"
#include "stdint.h"

// 7.6:3
#ifdef __CRT_ARCH_x86
typedef struct {
	uint16_t	__control;
	uint16_t      __mxcsr_hi;
	uint16_t	__status;
	uint16_t      __mxcsr_lo;
	uint32_t	__tag;
	char		__other[16];
} fenv_t;
#else
typedef struct {
	struct {
		uint32_t	__control;
		uint32_t	__status;
		uint32_t	__tag;
		char		__other[16];
	} __x87;
	uint32_t		__mxcsr;
} fenv_t;
#endif /* __CRT_ARCH_x86 */

// 7.6:4
typedef	uint16_t	fexcept_t;

// 7.6:5
/* Exception flags */
#define	FE_INVALID	0x01
#define	FE_DENORMAL	0x02
#define	FE_DIVBYZERO	0x04
#define	FE_OVERFLOW	0x08
#define	FE_UNDERFLOW	0x10
#define	FE_INEXACT	0x20
#define	FE_ALL_EXCEPT	(FE_DIVBYZERO | FE_DENORMAL | FE_INEXACT | \
			 FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW)

/* Rounding modes */
#define	FE_TONEAREST	0x0000
#define	FE_DOWNWARD	0x0400
#define	FE_UPWARD	0x0800
#define	FE_TOWARDZERO	0x0c00
#define	_ROUND_MASK	(FE_TONEAREST | FE_DOWNWARD | \
			 FE_UPWARD | FE_TOWARDZERO)

extern const fenv_t	__CRT_fe_dfl_env;
#define	FE_DFL_ENV	(&__CRT_fe_dfl_env)

#ifdef __cplusplus
extern "C" {
#endif

	// 7.6.2.1
	// not supported
	int _C_DECL feclearexcept(int excepts);

	// 7.6.2.2
	// not supported
	int _C_DECL fegetexceptflag(fexcept_t* flagp, int excepts);

	// 7.6.2.3
	// not supported
	int _C_DECL feraiseexcept(int excepts);

	// 7.6.2.4
	// not supported
	int _C_DECL fesetexceptflag(const fexcept_t* flagp, int excepts);

	// 7.6.2.5
	// not supported
	int _C_DECL fetestexcept(int excepts);

	// 7.6.3.1
	// not supported
	int _C_DECL fegetround(void);

	// 7.6.3.2
	// not supported
	int _C_DECL fesetround(int round);

	// 7.6.4.1
	int _C_DECL fegetenv(fenv_t* envp);

	// 7.6.4.2
	// not supported
	int _C_DECL feholdexcept(fenv_t* envp);

	// 7.6.4.3
	// not supported
	int _C_DECL fesetenv(const fenv_t* envp);

	// 7.6.4.4
	// not supported
	int _C_DECL feupdateenv(const fenv_t* envp);

#ifdef __cplusplus
}
#endif

#endif