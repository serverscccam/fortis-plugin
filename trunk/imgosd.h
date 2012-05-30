
#ifndef _IMGOSD_H_
#define _IMGOSD_H_

typedef struct {
	byte	r;
	byte	g;
	byte	b;
	byte _padding;
} S_Palentry;

typedef struct {
	int		width;		/* image width in pixels*/
	int		height;		/* image height in pixels*/
	int		planes;		/* # image planes*/
	int		bpp;		/* bits per pixel (1, 4 or 8)*/
	int		pitch;		/* bytes per line*/
	int		bytesperpixel;	/* bytes per pixel*/
	int		compression;	/* compression algorithm*/
	int		palsize;			/* palette size*/
	long		transcolor;		/* transparent color or -1 if none*/
	S_Palentry	*palette;	/* palette*/
	byte 		*imagebits;	/* image bits (dword right aligned)*/
} S_ImageHeader;

typedef struct {
	S_ImageHeader *imgHd;
	byte *comprBits;
	dword comprSize;
} S_OsdImageHdr;

#endif	// _IMGOSD_H_

