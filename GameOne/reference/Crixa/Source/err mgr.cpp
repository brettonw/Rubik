//------------------------------------------------------------------------------
//	Copyright �1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/30/97
//
//	02/22/97	DS		Added __cdecl keywords to matherr and newhandler to
//									support FastCall global calling.
//	02/22/97	DS		Added more memory reporting in NewHandler
//	03/20/97	DS		Added general exception handling and reporting.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "direct x.h"
#include "display.h"
#include "debugger.h"

//------------------------------------------------------------------------------
#define	MAKE_CASE(result)	case result: code = #result; break										//	define an error code string for reporting

//------------------------------------------------------------------------------
void	Error (cstr call, cstr file, int line)																		//	bail out on an error
{																																								//	begin
	cstr	code;																																		//	pointer for the error code string
	switch (GetLastError ())																											//	get the error code and index to the correct case
	{																																							//	begin
		//	general errors
		MAKE_CASE	(E_INVALIDARG);
		MAKE_CASE	(E_FAIL);
		MAKE_CASE	(E_OUTOFMEMORY);
		MAKE_CASE	(E_NOTIMPL);
		//	direct draw errors
		MAKE_CASE	(DDERR_ALREADYINITIALIZED);
		MAKE_CASE	(DDERR_CANNOTATTACHSURFACE);
		MAKE_CASE	(DDERR_CANNOTDETACHSURFACE);
		MAKE_CASE	(DDERR_CURRENTLYNOTAVAIL);
		MAKE_CASE	(DDERR_EXCEPTION);
		MAKE_CASE	(DDERR_HEIGHTALIGN);
		MAKE_CASE	(DDERR_INCOMPATIBLEPRIMARY);
		MAKE_CASE	(DDERR_INVALIDCAPS);
		MAKE_CASE	(DDERR_INVALIDCLIPLIST);
		MAKE_CASE	(DDERR_INVALIDMODE);
		MAKE_CASE	(DDERR_INVALIDOBJECT);
		MAKE_CASE	(DDERR_INVALIDPIXELFORMAT);
		MAKE_CASE	(DDERR_INVALIDRECT);
		MAKE_CASE	(DDERR_LOCKEDSURFACES);
		MAKE_CASE	(DDERR_NO3D);
		MAKE_CASE	(DDERR_NOALPHAHW);
		MAKE_CASE	(DDERR_NOCLIPLIST);
		MAKE_CASE	(DDERR_NOCOLORCONVHW);
		MAKE_CASE	(DDERR_NOCOOPERATIVELEVELSET);
		MAKE_CASE	(DDERR_NOCOLORKEY);
		MAKE_CASE	(DDERR_NOCOLORKEYHW);
		MAKE_CASE	(DDERR_NODIRECTDRAWSUPPORT);
		MAKE_CASE	(DDERR_NOEXCLUSIVEMODE);
		MAKE_CASE	(DDERR_NOFLIPHW);
		MAKE_CASE	(DDERR_NOGDI);
		MAKE_CASE	(DDERR_NOMIRRORHW);
		MAKE_CASE	(DDERR_NOTFOUND);
		MAKE_CASE	(DDERR_NOOVERLAYHW);
		MAKE_CASE	(DDERR_NORASTEROPHW);
		MAKE_CASE	(DDERR_NOROTATIONHW);
		MAKE_CASE	(DDERR_NOSTRETCHHW);
		MAKE_CASE	(DDERR_NOT4BITCOLOR);
		MAKE_CASE	(DDERR_NOT4BITCOLORINDEX);
		MAKE_CASE	(DDERR_NOT8BITCOLOR);
		MAKE_CASE	(DDERR_NOTEXTUREHW);
		MAKE_CASE	(DDERR_NOVSYNCHW);
		MAKE_CASE	(DDERR_NOZBUFFERHW);
		MAKE_CASE	(DDERR_NOZOVERLAYHW);
		MAKE_CASE	(DDERR_OUTOFCAPS);
		MAKE_CASE	(DDERR_OUTOFVIDEOMEMORY);
		MAKE_CASE	(DDERR_OVERLAYCANTCLIP);
		MAKE_CASE	(DDERR_OVERLAYCOLORKEYONLYONEACTIVE);
		MAKE_CASE	(DDERR_PALETTEBUSY);
		MAKE_CASE	(DDERR_COLORKEYNOTSET);
		MAKE_CASE	(DDERR_SURFACEALREADYATTACHED);
		MAKE_CASE	(DDERR_SURFACEALREADYDEPENDENT);
		MAKE_CASE	(DDERR_SURFACEBUSY);
		MAKE_CASE	(DDERR_CANTLOCKSURFACE);
		MAKE_CASE	(DDERR_SURFACEISOBSCURED);
		MAKE_CASE	(DDERR_SURFACELOST);
		MAKE_CASE	(DDERR_SURFACENOTATTACHED);
		MAKE_CASE	(DDERR_TOOBIGHEIGHT);
		MAKE_CASE	(DDERR_TOOBIGSIZE);
		MAKE_CASE	(DDERR_TOOBIGWIDTH);
		MAKE_CASE	(DDERR_UNSUPPORTEDFORMAT);
		MAKE_CASE	(DDERR_UNSUPPORTEDMASK);
		MAKE_CASE	(DDERR_VERTICALBLANKINPROGRESS);
		MAKE_CASE	(DDERR_WASSTILLDRAWING);
		MAKE_CASE	(DDERR_XALIGN);
		MAKE_CASE	(DDERR_INVALIDDIRECTDRAWGUID);
		MAKE_CASE	(DDERR_DIRECTDRAWALREADYCREATED);
		MAKE_CASE	(DDERR_NODIRECTDRAWHW);
		MAKE_CASE	(DDERR_PRIMARYSURFACEALREADYEXISTS);
		MAKE_CASE	(DDERR_NOEMULATION);
		MAKE_CASE	(DDERR_REGIONTOOSMALL);
		MAKE_CASE	(DDERR_CLIPPERISUSINGHWND);
		MAKE_CASE	(DDERR_NOCLIPPERATTACHED);
		MAKE_CASE	(DDERR_NOHWND);
		MAKE_CASE	(DDERR_HWNDSUBCLASSED);
		MAKE_CASE	(DDERR_HWNDALREADYSET);
		MAKE_CASE	(DDERR_NOPALETTEATTACHED);
		MAKE_CASE	(DDERR_NOPALETTEHW);
		MAKE_CASE	(DDERR_BLTFASTCANTCLIP);
		MAKE_CASE	(DDERR_NOBLTHW);
		MAKE_CASE	(DDERR_NODDROPSHW);
		MAKE_CASE	(DDERR_OVERLAYNOTVISIBLE);
		MAKE_CASE	(DDERR_NOOVERLAYDEST);
		MAKE_CASE	(DDERR_INVALIDPOSITION);
		MAKE_CASE	(DDERR_NOTAOVERLAYSURFACE);
		MAKE_CASE	(DDERR_EXCLUSIVEMODEALREADYSET);
		MAKE_CASE	(DDERR_NOTFLIPPABLE);
		MAKE_CASE	(DDERR_CANTDUPLICATE);
		MAKE_CASE	(DDERR_NOTLOCKED);
		MAKE_CASE	(DDERR_CANTCREATEDC);
		MAKE_CASE	(DDERR_NODC);
		MAKE_CASE	(DDERR_WRONGMODE);
		MAKE_CASE	(DDERR_IMPLICITLYCREATED);
		MAKE_CASE	(DDERR_NOTPALETTIZED);
		MAKE_CASE	(DDERR_UNSUPPORTEDMODE);
		MAKE_CASE	(DDERR_NOMIPMAPHW);
		MAKE_CASE	(DDERR_INVALIDSURFACETYPE);
		MAKE_CASE	(DDERR_DCALREADYCREATED);
		MAKE_CASE	(DDERR_CANTPAGELOCK);
		MAKE_CASE	(DDERR_CANTPAGEUNLOCK);
		MAKE_CASE	(DDERR_NOTPAGELOCKED);
		//	direct sound errors
		MAKE_CASE	(DSERR_ALLOCATED);
		MAKE_CASE	(DSERR_CONTROLUNAVAIL);
		MAKE_CASE	(DSERR_INVALIDCALL);
		MAKE_CASE	(DSERR_PRIOLEVELNEEDED);
		MAKE_CASE	(DSERR_BADFORMAT);
		MAKE_CASE	(DSERR_NODRIVER);
		MAKE_CASE	(DSERR_ALREADYINITIALIZED);
		MAKE_CASE	(DSERR_NOAGGREGATION);
		MAKE_CASE	(DSERR_BUFFERLOST);
		MAKE_CASE	(DSERR_OTHERAPPHASPRIO);
		MAKE_CASE	(DSERR_UNINITIALIZED);
		//	sockets errors
    MAKE_CASE (WSABASEERR);
    MAKE_CASE (WSAEINTR);
    MAKE_CASE (WSAEBADF);
    MAKE_CASE (WSAEACCES);
    MAKE_CASE (WSAEFAULT);
    MAKE_CASE (WSAEINVAL);
    MAKE_CASE (WSAEMFILE);
    MAKE_CASE (WSAEWOULDBLOCK);
    MAKE_CASE (WSAEINPROGRESS);
    MAKE_CASE (WSAEALREADY);
    MAKE_CASE (WSAENOTSOCK);
    MAKE_CASE (WSAEDESTADDRREQ);
    MAKE_CASE (WSAEMSGSIZE);
    MAKE_CASE (WSAEPROTOTYPE);
    MAKE_CASE (WSAENOPROTOOPT);
    MAKE_CASE (WSAEPROTONOSUPPORT);
    MAKE_CASE (WSAESOCKTNOSUPPORT);
    MAKE_CASE (WSAEOPNOTSUPP);
    MAKE_CASE (WSAEPFNOSUPPORT);
    MAKE_CASE (WSAEAFNOSUPPORT);
    MAKE_CASE (WSAEADDRINUSE);
    MAKE_CASE (WSAEADDRNOTAVAIL);
    MAKE_CASE (WSAENETDOWN);
    MAKE_CASE (WSAENETUNREACH);
    MAKE_CASE (WSAENETRESET);
    MAKE_CASE (WSAECONNABORTED);
    MAKE_CASE (WSAECONNRESET);
    MAKE_CASE (WSAENOBUFS);
    MAKE_CASE (WSAEISCONN);
    MAKE_CASE (WSAENOTCONN);
    MAKE_CASE (WSAESHUTDOWN);
    MAKE_CASE (WSAETOOMANYREFS);
    MAKE_CASE (WSAETIMEDOUT);
    MAKE_CASE (WSAECONNREFUSED);
    MAKE_CASE (WSAELOOP);
    MAKE_CASE (WSAENAMETOOLONG);
    MAKE_CASE (WSAEHOSTDOWN);
    MAKE_CASE (WSAEHOSTUNREACH);
    MAKE_CASE (WSAENOTEMPTY);
    MAKE_CASE (WSAEPROCLIM);
    MAKE_CASE (WSAEUSERS);
    MAKE_CASE (WSAEDQUOT);
    MAKE_CASE (WSAESTALE);
    MAKE_CASE (WSAEREMOTE);
    MAKE_CASE (WSAEDISCON);
    MAKE_CASE (WSASYSNOTREADY);
    MAKE_CASE (WSAVERNOTSUPPORTED);
    MAKE_CASE (WSANOTINITIALISED);
    MAKE_CASE (WSAHOST_NOT_FOUND);
    MAKE_CASE (WSATRY_AGAIN);
    MAKE_CASE (WSANO_RECOVERY);
    MAKE_CASE (WSANO_DATA);
		//	unknown error case
		default: code = "UNDEFINED ERROR"; break;																		//	default case
	}																																							//	end
	char	msg[4096];																															//	place to create error message
	sprintf (	msg, "  code: %s\n  call: %s\n  file: %s\n  line: %d",							\
						code, call, file, line);																						//	build the error message
	DEBUG_HARD ("\nDirectX Error:\n");
	DEBUG_HARD ("%s\n", msg);																											//	pump the message into the debugger
	if (gDisplay)																																	//	if we are in direct X
		gDisplay->Restore ();																												//	restore the system to a readable state
	MessageBox (GetActiveWindow (), msg, "Error", MB_OK);													//	put the message on the screen
	exit (EXIT_FAILURE);																													//	exit the program
}																																								//	end

//------------------------------------------------------------------------------
int		__cdecl	_matherr (_exception *except)																			//	report a math error
{																																								//	begin
	cstr	error;																																	//	place to store an error string
	switch (except->type)																													//	check the error type
	{																																							//	begin
		case _DOMAIN:																																//	error
			error = "DOMAIN";																													//	set the descriptive string
			break;																																		//	end case
		case _SING:																																	//	error
			error = "SING";																														//	set the descriptive string
			break;																																		//	end case
		case _OVERFLOW:																															//	error
			error = "OVERFLOW";																												//	set the descriptive string
			break;																																		//	end case
		case _PLOSS:																																//	error
			error = "PLOSS";																													//	set the descriptive string
			break;																																		//	end case
		case _TLOSS:																																//	error
			error = "TLOSS";																													//	set the descriptive string
			break;																																		//	end case
		case _UNDERFLOW:																														//	error
			error = "UNDERFLOW";																											//	set the descriptive string
			break;																																		//	end case
	}																																							//	end
	DEBUG_STR ("Math error: %s (%s)\n", except->name, error);											//	report the error to disk
	DEBUG_STR ("\tArg 1: %f\n", except->arg1);																		//	report the error to disk
	DEBUG_STR ("\tArg 2: %f\n", except->arg1);																		//	report the error to disk
	DEBUG_STR ("\tRet:   %f\n", except->retval);																	//	report the error to disk
	return 0;																																			//	let the system handle the error
}																																								//	end

//------------------------------------------------------------------------------
void	DebugMemoryConfig (void)																									//	function reports memory statistics
{
	MEMORYSTATUS memstat;
	memstat.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&memstat);
	
	int memTotal	= (memstat.dwTotalPhys >> 20) + 1;															// round up all values
	int memAvail	= (memstat.dwAvailPhys >> 20) + 1;
	int virtTotal = (memstat.dwTotalVirtual >> 20);																// these should be unrounded
	int virtAvail = (memstat.dwAvailVirtual >> 20);
	int	pageTotal = (memstat.dwTotalPageFile >> 20);
	int	pageAvail	= (memstat.dwAvailPageFile >> 20);

	DEBUG_STR ("    %d/%d MB physical memory available\n",memAvail,memTotal);			// report physical memory
	DEBUG_STR ("    %d/%d MB paged memory available\n",pageAvail,pageTotal);			// report virtual memory
	DEBUG_STR ("    %d/%d MB virtual memory available\n",virtAvail,virtTotal);		// report virtual memory

}

//------------------------------------------------------------------------------
int	__cdecl		NewHandler (size_t size)																					//	function to call when we are out of memory
{																																								//	begin
	int allocFail = size >> 20;																										//	size of failed megabytage request
	DEBUG_STR ("\nNewHandler: %d MB unavailable\n", allocFail);											//	report the size of the allocation
	DebugMemoryConfig();																													//	report the memory sizes
	/*
	SetLastError (E_OUTOFMEMORY);																									//	set the error code
	Error ("NewHandler", __FILE__, __LINE__);																			//	report the out of memory condition and bail out
	*/
	return 0;																																			//	don't retry the allocation
}																																								//	end

//------------------------------------------------------------------------------
