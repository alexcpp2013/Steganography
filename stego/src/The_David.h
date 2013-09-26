#if !defined(AFX_The_David_TOOLS_H__F52C6F65_BDD6_11D7_B2A0_000244648998__INCLUDED_)
#define AFX_The_David_TOOLS_H__F52C6F65_BDD6_11D7_B2A0_000244648998__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"


#endif // !defined(AFX_The_David_TOOLS_H__F52C6F65_BDD6_11D7_B2A0_000244648998__INCLUDED_)

typedef struct {
	char id_riff[4];
    long len_riff;

	char id_chunk[4];
	char fmt[4];
	long len_chunk;

	short int type;
	short int channels;
	long freq;
	long bytes;
	short int align;
	short int bits;

    char waste[18];

} TitleWave;

