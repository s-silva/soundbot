/**
 * The MIT License (MIT)
 * 
 * Copyright (c) 2013, Sandaruwan Silva <c-h [-a-t] users [-dot-] sf [-dot-] net>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
**/

#include <windows.h>
#include "sndfile.h"

LRESULT CALLBACK callback_window_main(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam); 

#define application_title L"Soundbot"

HINSTANCE application_instance = 0;
HWND hwnd_main = 0;
HDC  hdc_main = 0;
int band_n = 0;


void fft_float (
    unsigned  NumSamples,       /* must be a power of 2          */
    int       InverseTransform,	/* 0=forward FFT, 1=inverse FFT  */
    float    *RealIn,			/* array of input's real samples */
    float    *ImagIn,			/* array of input's imag samples */
    float    *RealOut,			/* array of output's reals       */
    float    *ImagOut);

/* main */


int wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int nCmdShow) 
{ 
	WNDCLASS            wc; 
	HWND                hwnd; 
	MSG                 msg; 
	
	application_instance = hInst;

	wc.style         = CS_HREDRAW | CS_VREDRAW; 
	wc.lpfnWndProc   = callback_window_main; 
	wc.cbClsExtra    = 0; 
	wc.cbWndExtra    = 0; 
	wc.hInstance     = hInst; 
	wc.hIcon         = LoadIcon(hInst,(LPCTSTR)1); 
	wc.hCursor       = LoadCursor(NULL,IDC_ARROW); 
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0xd9, 0xe3, 0xec)); 
	wc.lpszMenuName  = NULL; 
	wc.lpszClassName = application_title; 

	if(!RegisterClass(&wc)) return 0; 

	hwnd_main = CreateWindowEx(WS_EX_WINDOWEDGE | WS_EX_APPWINDOW, application_title, application_title, 
		WS_THICKFRAME | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_OVERLAPPED, 
		CW_USEDEFAULT, CW_USEDEFAULT, 1000, 500, NULL, NULL, hInst, NULL); 

	if (!hwnd_main) return 0; 

	ShowWindow(hwnd_main, nCmdShow); 
	UpdateWindow(hwnd_main); 

 
	while (GetMessage(&msg,NULL,0,0) > 0) 
	{ 
		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	} 

	return 0;
} 




void process(float *data, int count, int lcount, int ncount, int channels)
{
	int x = 50, i, j;
	float fft_r[4096], fft_i[4096];
	float monosamples[4096];
	static int lx =0, ly = 0;
	int nx = 0, ny = 0;
	/*
	x += lcount / channels;

	for(i=0; i<count; i+=channels, x++)
	{
		SetPixel(hdc_main, x, data[i] * 100 + 150, 0x0);
	}*/

	for(i=0, j=0; i<count; i+=channels, j++)
	{
		monosamples[j] = data[i];
	}

	
	fft_float (count / channels, false, monosamples, 0, fft_r, fft_i);
	

	/*x = ncount;

	for(i=0; i<count/4; i++)
	{
		int v = (int)(fft_r[(count / 4) - i] * 255.0f);
		if(v > 255) v = 255;
		else if(v < 0) v = 0;

		SetPixel(hdc_main, x, i, RGB(v, 0, 0));
	}*/

	x = ncount * 5;
	nx = x;
	ny = fft_r[band_n] * 0.2 + 200;

	MoveToEx(hdc_main, lx, ly, 0);
	LineTo(hdc_main, nx, ny);

	lx = nx;
	ly = ny;
	return;
}

void alert(const char *s)
{
	MessageBoxA(0, s, "Alert", 32);
}


LRESULT CALLBACK callback_window_main(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) 
{ 
	switch (msg) 
	{ 

		case WM_CREATE:
			hdc_main = GetDC(hwnd);
			break;

		case WM_PAINT:
		{
			SNDFILE  *snd;
			SF_INFO	 sfinfo;
			int      readcount;
			int      buffsize = 4096;
			float   *data;
			int      lcount = 0, ncount = 0;


			

			data = (float*) malloc(buffsize * sizeof(float));
			if(!data) break;


			snd = sf_open("D:/Production/Samples/Mine/Kicks/HE Kick OrgFull - F#.wav", SFM_READ, &sfinfo);

			while((readcount = sf_read_float(snd, data, buffsize)))
			{
				process(data, readcount, lcount, ncount, sfinfo.channels) ;
				lcount += readcount;
				ncount ++;
			};

			sf_close(snd);
			free(data);


		}
		break;

		/*
		case WM_PAINT: 
		{ 
			PAINTSTRUCT ps; 
			HDC hdc; 
			RECT r; 
			
			GetClientRect(hwnd, &r); 
			hdc = BeginPaint(hwnd, &ps);

			EndPaint(hwnd, &ps);
			break; 
		} */

		case WM_DESTROY: 
			PostQuitMessage(0); 
			break; 

		case WM_LBUTTONDOWN:
			
			break;

		case WM_LBUTTONUP:
			
			break;

		case WM_MOUSEMOVE:
			
			break;

		case WM_KEYDOWN:
			switch(wparam)
			{
				case VK_UP:
					{
						RECT r;
						GetClientRect(hwnd, &r);
						InvalidateRect(hwnd, &r, true);
					}
					band_n++;
					break;

				case VK_DOWN:
					{
						RECT r;
						GetClientRect(hwnd, &r);
						InvalidateRect(hwnd, &r, true);
					}
					band_n--;
					if(band_n < 0) band_n = 0;
					break;
			}
			break;

		default: 
			return DefWindowProc(hwnd, msg, wparam, lparam); 
	} 
	return 0; 
} 






















/*-----------------------------------------------------------------------------

 FFT.c  -  Don Cross <dcross@intersrv.com>

 http://www.intersrv.com/~dcross/fft.html

 Contains definitions for doing Fourier transforms
 and inverse Fourier transforms.

 This module performs operations on arrays of 'float'.

 Revision history:
 
 1998 September 19 [Don Cross]
  Updated coding standards.
  Improved efficiency of trig calculations.

 Readme (by Don Cross):

 The file FFT.ZIP contains C source code for performing Discrete Fast Fourier
 Transforms (DFFTs) and inverse DFFTs.  This source code is public domain.
 Use at your own risk.  For more information, point your web browser at:

 http://www.intersrv.com/~dcross/fft.html

 Also, feel free to send questions/comments about this source code to me
  by e-mail at the address above.

     -----------------------------------------------------------------------
 
                          *** IMPORTANT NOTE ***

 There are two different ways to define the FFT (and inverse FFT) that
 result in having the same outputs, only the imaginary parts are
 negated.  In other words, the two different algorithms produce results
 that are complex conjugates of each other.
 
 The popular mathematical software tool MATLAB and my FFT source routines
 use opposite definitions.  However, it is easy to make my FFT source
 code compatible with MATLAB.  To do this, use your favorite text editor
 to change the following files:

     fourierd.c
     fourierf.c
 
 Look for the line of text that says:
 
     double angle_numerator = 2.0 * DDC_PI;
 
 And change it to:
 
     double angle_numerator = -2.0 * DDC_PI;
 
 That is, just make the angle numerator negative instead of positive.
 
 Be sure to do this in both fourierd.c AND fourierf.c so you have
 consistent routines for both single- and double-precision math.

 I am considering changing my code to be compatible with MATLAB, but
 first I would like to find out if there is some definitely "correct"
 or "preferred" definition within the mathematical community.  If you
 have any comments on this issue, please let me know at the following 
 email address, because I'm stumped and need help!
 
    dcross@intersrv.com
 
 Thanks!
 
    -----------------------------------------------------------------------
 
                         *** SMALL REQUESTS ****

 If you want to give away copies of this source code, that's fine, so long
 as you do the following:
 
 - Do not charge any money for this source code, except for possibly a
   reasonable fee to cover postage, disk duplication, etc.  I wrote this
   code and I want it to be free to EVERYONE!
 
 - Do not remove my name, e-mail address, or URL from any of the files in
   this collection.
 
 - Please keep this readme.txt file with the source and headers so that others
   can get in touch with me to ask questions and/or find my web page to read
   the online tutorial.
 
 - If you make any modifications to the source code, please add comments to
   it which include your name, e-mail address, web page URL (if any), and
   explain what you did to the code.
 
 - If you use this source code in an interesting program, please let me know.
   I promise will never try to get money from you, even if you use this code
   in a commercial program.  I just want to know what kind of clever and
   creative things people do with Fourier Transforms.
 
    -----------------------------------------------------------------------

 (formatted for fennec)
-----------------------------------------------------------------------------*/

#include <math.h>

#define  DDC_PI  (3.14159265358979323846)

int fft_local_IsPowerOfTwo ( unsigned x )
{
    if ( x < 2 )
        return 0;

    if ( x & (x-1) ) /* Thanks to 'byang' for this cute trick! */
        return 0;

    return 1;
}

unsigned fft_local_NumberOfBitsNeeded ( unsigned PowerOfTwo )
{
    unsigned i;

    if ( PowerOfTwo < 2 )
    {
		return 0;
    }

    for ( i=0; ; i++ )
    {
        if ( PowerOfTwo & (1 << i) )
            return i;
    }
}

unsigned fft_local_ReverseBits ( unsigned index, unsigned NumBits )
{
    unsigned i, rev;

    for ( i=rev=0; i < NumBits; i++ )
    {
        rev = (rev << 1) | (index & 1);
        index >>= 1;
    }

    return rev;
}

/*
**   The following function returns an "abstract frequency" of a
**   given index into a buffer with a given number of frequency samples.
**   Multiply return value by sampling rate to get frequency expressed in Hz.
*/

double fft_local_Index_to_frequency ( unsigned NumSamples, unsigned Index )
{
    if ( Index >= NumSamples )
        return 0.0;
    else if ( Index <= NumSamples/2 )
        return (double)Index / (double)NumSamples;

    return -(double)(NumSamples-Index) / (double)NumSamples;
}

/*
**   fft() computes the Fourier transform or inverse transform
**   of the complex inputs to produce the complex outputs.
**   The number of samples must be a power of two to do the
**   recursive decomposition of the FFT algorithm.
**   See Chapter 12 of "Numerical Recipes in FORTRAN" by
**   Press, Teukolsky, Vetterling, and Flannery,
**   Cambridge University Press.
**
**   Notes:  If you pass ImaginaryIn = NULL, this function will "pretend"
**           that it is an array of all zeroes.  This is convenient for
**           transforming digital samples of real number data without
**           wasting memory.
*/

void fft_float (
    unsigned  NumSamples,       /* must be a power of 2          */
    int       InverseTransform,	/* 0=forward FFT, 1=inverse FFT  */
    float    *RealIn,			/* array of input's real samples */
    float    *ImagIn,			/* array of input's imag samples */
    float    *RealOut,			/* array of output's reals       */
    float    *ImagOut )			/* array of output's imaginaries */
{
    unsigned NumBits;    /* Number of bits needed to store indices */
    unsigned i, j, k, n;
    unsigned BlockSize, BlockEnd;

    double angle_numerator = 2.0 * DDC_PI;
    double tr, ti;     /* temp real, temp imaginary */

    if ( !fft_local_IsPowerOfTwo(NumSamples) )
    {
       return;
    }

    if ( InverseTransform )
        angle_numerator = -angle_numerator;

    /*CHECKPOINTER ( RealIn );
      CHECKPOINTER ( RealOut );
      CHECKPOINTER ( ImagOut ); */

    NumBits = fft_local_NumberOfBitsNeeded ( NumSamples );

    /*
    **   Do simultaneous data copy and bit-reversal ordering into outputs...
    */

    for ( i=0; i < NumSamples; i++ )
    {
        j = fft_local_ReverseBits ( i, NumBits );
        RealOut[j] = RealIn[i];
        ImagOut[j] = (ImagIn == 0) ? 0.0f : ImagIn[i];
    }

    /*
    **   Do the FFT itself...
    */

    BlockEnd = 1;
    for ( BlockSize = 2; BlockSize <= NumSamples; BlockSize <<= 1 )
    {
        double delta_angle = angle_numerator / (double)BlockSize;
        double sm2 = sin ( -2 * delta_angle );
        double sm1 = sin ( -delta_angle );
        double cm2 = cos ( -2 * delta_angle );
        double cm1 = cos ( -delta_angle );
        double w = 2 * cm1;
        double ar[3], ai[3];

        for ( i=0; i < NumSamples; i += BlockSize )
        {
            ar[2] = cm2;
            ar[1] = cm1;

            ai[2] = sm2;
            ai[1] = sm1;

            for ( j=i, n=0; n < BlockEnd; j++, n++ )
            {
                ar[0] = w*ar[1] - ar[2];
                ar[2] = ar[1];
                ar[1] = ar[0];

                ai[0] = w*ai[1] - ai[2];
                ai[2] = ai[1];
                ai[1] = ai[0];

                k = j + BlockEnd;
                tr = ar[0]*RealOut[k] - ai[0]*ImagOut[k];
                ti = ar[0]*ImagOut[k] + ai[0]*RealOut[k];

                RealOut[k] = RealOut[j] - (float)tr;
                ImagOut[k] = ImagOut[j] - (float)ti;

                RealOut[j] += (float)tr;
                ImagOut[j] += (float)ti;
            }
        }

        BlockEnd = BlockSize;
    }

    /*
    **   Need to normalize if inverse transform...
    */

    if ( InverseTransform )
    {
        float denom = (float)NumSamples;

        for ( i=0; i < NumSamples; i++ )
        {
            RealOut[i] /= denom;
            ImagOut[i] /= denom;
        }
    }
}

/* end of the file */