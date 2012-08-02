//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Bretton Wade
//	Last Revision:	01/31/97
//
//	02/17/97	DS		Added "level.smg" to level files
//	02/19/97	DS		Added window debugging stuff
//	02/21/97	DS		Single-precision math setting in WinMain()
//	03/10/97	DS		Disabled Level File Conversion stuff
//	03/21/97	DS		Added general exception handling for all screen modes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifdef	_MSC_VER
#include "precompile.h"
#endif

//------------------------------------------------------------------------------
#include "play controller.h"
#include "wade.h"
#include "pi.h"
#include "direction.h"
#include "get parameters.h"
#include "display.h"
#include "tokenize.h"
#include "level file.h"
#include "disk file.h"
#include "format text.h"
#include "direct x.h"
#include "network.h"
#include "link node.h"
#include "dictionary node.h"
#include "debugger.h"
#include "vid debug.h"

//------------------------------------------------------------------------------
#define	CLASS_NAME	"Crixa"																											//	name of the window class and menu name

//------------------------------------------------------------------------------
static	Bool		gActive = FALSE;																								//	whether or not the application is active
static	MSG			gMessage;																												//	place to get a message
				HWND		gWindow;																												//	global handle to the window structure
static	RECT		oldClip;

//------------------------------------------------------------------------------
#if USE_COMMAND_LINE
//------------------------------------------------------------------------------
static	char		gCommand[256];																									//	command string
static	int			gIndex = 0;																											//	current index into the command string
static	Bool		gPlaying = FALSE;																								//	whether or not the game is playing

//------------------------------------------------------------------------------
void	DisplayCommandString (Bool carat = TRUE)																	//	draw the command string
{																																								//	begin
	HDC	hdc = gDisplay->GetDC ();																									//	get the drawing context
	SetTextColor (hdc, 0x00ffffff);																								//	set the text color to white
	SetBkColor (hdc, 0x00000000);																									//	set the background color to black
	char	command[300];																														//	a place to manipulate the command string
	if (carat)																																		//	if we should be drawing the carat
		sprintf (command, "> %s", gCommand);																				//	set up the command string with the carat
	else																																					//	otherwise
		strcpy (command, gCommand);																									//	just copy the command string
	TextOut (hdc, 10, 9, command, strlen (command));															//	draw the text
	gDisplay->ReleaseDC (hdc);																										//	release the drawing context
}																																								//	end

//------------------------------------------------------------------------------
void	EraseCommandString (void)																									//	erase the command string
{																																								//	begin
	HDC	hdc = gDisplay->GetDC ();																									//	get the drawing context
	HBRUSH	brush = CreateSolidBrush (0x00000000);																//	make a brush for solid black
	HGDIOBJ	oldBrush = SelectObject (hdc, brush);																	//	select the brush
	HPEN		pen = CreatePen (PS_SOLID, 0, 0x00ffffff);														//	make a pen for solid white
	HGDIOBJ	oldPen = SelectObject (hdc, pen);																			//	select the pen
	Rectangle (hdc, 0, 0, SCRN_HEIGHT, 35);																				//	draw the rectangle
	SelectObject (hdc, oldPen);																										//	restore the old pen
	DeleteObject (pen);																														//	free up the pen
	SelectObject (hdc, oldBrush);																									//	restore the old brush
	DeleteObject (brush);																													//	free up the brush
	gDisplay->ReleaseDC (hdc);																										//	release the drawing context
}																																								//	end

//------------------------------------------------------------------------------
void	ParseCommandString (void)																									//	parse out the command string
{																																								//	begin
	cstr	*token;																																	//	token stream
	int		count = Tokenize (gCommand, token);																			//	convert the command to a token stream
	if (count)																																		//	if tokens were found
	{																																							//	begin
		if (strcmp (token[0], "SET") == 0)
		{
			cstr	success = 0;
			if (count == 3)
				success = gParmBase->Assign (token[1], token[2]);
			EraseCommandString ();
			if (success)
				sprintf (gCommand, "SET %s %s OK", token[1], token[2]);
			else
				strcpy (gCommand, "SET FAILED");
			DisplayCommandString (FALSE);
			Sleep (800);
		}
		else if (strcmp (token[0], "GET") == 0)
		{
			cstr	get = 0;
			EraseCommandString ();
			if (count == 2)
				get = gParmBase->GetString (token[1]);
			if (get)
				strcpy (gCommand, get);
			else
				strcpy (gCommand, "GET FAILED");
			DisplayCommandString (FALSE);
			Sleep (800);
		}
		else if (strcmp (token[0], "LOAD") == 0)
		{
			Bool	load = FALSE;
			cstr	get = 0;
			if (count == 2)
				load = gParmBase->Load (token[1]);
			if (load)
				sprintf (gCommand, "LOAD %s OK", token[1]);
			else
				strcpy (gCommand, "LOAD FAILED");
			EraseCommandString ();
			DisplayCommandString (FALSE);
			Sleep (800);
		}
		else if (strcmp (token[0], "DUMP") == 0)
		{
			if (count == 2)
			{
				gParmBase->Dump (token[1]);
				sprintf (gCommand, "DUMP %s OK", token[1]);
			}
			else
				strcpy (gCommand, "DUMP FAILED");
			EraseCommandString ();
			DisplayCommandString (FALSE);
			Sleep (800);
		}
		else if (strcmp (token[0], "SCREEN") == 0)
		{
			if (count == 2)
			{
				gDisplay->DumpToFile (token[1]);
				sprintf (gCommand, "SCREEN %s OK", token[1]);
			}
			else
				strcpy (gCommand, "SCREEN FAILED");
			EraseCommandString ();
			DisplayCommandString (FALSE);
			Sleep (800);
		}
		gIndex = 0;
		gCommand[0] = 0;
		EraseCommandString ();
		DisplayCommandString ();
		delete[] token;																															//	release the token stream when we are finished
	}																																							//	end
	else
		gPlaying = TRUE;
}																																								//	end

//------------------------------------------------------------------------------
#else
#define	gPlaying	TRUE																													//	define this value as true for compiled options
#endif
//------------------------------------------------------------------------------
long FAR PASCAL	WinProc (HWND window, UINT message, WPARAM wpar, LPARAM lpar)		//	window proc for the application window
{																																								//	begin
	if (gPlaying)																																	//	if the game engine is running
	{																																							//	begin
		switch (message)																														//	check the message
		{																																						//	begin
			case WM_ACTIVATEAPP:																											//	activate/deactivate message
				gActive = wpar;																													//	save the activation value
				break;																																	//	end case
			case WM_SETCURSOR:																												//	set cursor message
				SetCursor (0);																													//	no, we don't want a cursor (time consuming)
				return TRUE;																														//	and don't let the defproc have it
			case WM_SYSCOMMAND:																												//	the system wants to do something
				if (wpar == SC_SCREENSAVE)																							//	if it is a screen saver
					return 0;																															//	tell it to buzz off
				break;																																	//	end case
			case WM_KEYDOWN:																													//	key pressed message
				if (wpar == VK_F4)																											//	if the key is the escape key
				{																																				//	begin
	#if USE_COMMAND_LINE
					gPlaying = FALSE;																											//	stop the game engine
					gCommand[0] = 0;																											//	truncate the command
					gIndex = 0;																														//	reset the index
					EraseCommandString ();																								//	erase the typing area
					DisplayCommandString ();																							//	display the empty string with carat
	#else
					PostMessage (window, WM_CLOSE, 0, 0);																	//	close the window
					Controller::Switch (QUIT_CONTROLLER_ID);															//	close the controller
					DEBUG_STR("\nWinProc:\n");
					DEBUG_STR("    User pressed F4 to Quit. Shutting Down...\n");
	#endif
				}																																				//	end
				return 0;																																//	end case
			case WM_DESTROY:																													//	destroy the window
				PostQuitMessage (0);																										//	quit
				break;																																	//	end case
		}																																						//	end
	}																																							//	end
	#if USE_COMMAND_LINE
	else																																					//	otherwise, we are in command mode
	{																																							//	begin
		switch (message)																														//	check the message
		{																																						//	begin
			case WM_ACTIVATEAPP:																											//	activate/deactivate message
				gActive = wpar;																													//	save the activation value
				break;																																	//	end case
			case WM_SETCURSOR:																												//	set cursor message
				SetCursor (0);																													//	no, we don't want a cursor (time consuming)
				EraseCommandString ();
				DisplayCommandString ();																								//	draw the command string as it currently exists
				return TRUE;																														//	and don't let the defproc have it
			case WM_SYSCOMMAND:																												//	the system wants to do something
				if (wpar == SC_SCREENSAVE)																							//	if it is a screen saver
					return 0;																															//	tell it to buzz off
				break;																																	//	end case
			case WM_CHAR:																															//	a translated key down message
				gCommand[gIndex++] = toupper (wpar);																		//	copy the upper case version of the pressed key
				gCommand[gIndex] = 0;																										//	null terminate the string
				DisplayCommandString ();																								//	draw the command string
				return 0;																																//	end case
			case WM_KEYDOWN:																													//	key pressed message
				switch (wpar)																														//	look at which key was pressed
				{																																				//	begin
					case VK_ESCAPE:																												//	the escape key
						PostMessage (window, WM_CLOSE, 0, 0);																//	close the window
						Controller::Switch (QUIT_CONTROLLER_ID);														//	close the controller
						break;																															//	end case
					case VK_RETURN:																												//	the return key
						ParseCommandString ();																							//	parse the current command string
						break;																															//	end case
					case VK_BACK:																													//	the backspace key
						gIndex--;																														//	back step in the command buffer
						if (gIndex < 0)																											//	if we backstep past the beginning of th ebuffer
							gIndex = 0;																												//	reset to the beginning
						gCommand[gIndex] = 0;																								//	null terminate the buffer
						EraseCommandString ();																							//	erase teh string area
						DisplayCommandString ();																						//	redraw the new command string
						break;																															//	end case
					default:																															//	for any other key
						TranslateMessage (&gMessage);																				//	convert it to an ascii code
						break;																															//	end case
				}																																				//	end
				return 0;																																//	end case
			case WM_QUERYNEWPALETTE:
						gDisplay->SetPalette();
						break;
			case WM_DESTROY:																													//	destroy the window
				PostQuitMessage (0);																										//	quit
				break;																																	//	end case
		}																																						//	end
	}																																							//	end
#endif
	//return 0;
	return DefWindowProc (window, message, wpar, lpar);														//	default window behavior
}																																								//	end

//------------------------------------------------------------------------------
static	void	ReportKeyNames (void)																							//	report the names of virtual key codes
{																																								//	begin
	for (uint i = 0; i < 256; i++)
	{
		char	buffer[256];
		uint	scan = MapVirtualKey (i, 0);
		GetKeyNameText ((scan << 16) | (1 << 25), buffer, 256);
		if (strlen (buffer))
			DEBUG_STR ("%03d (%s)\n", i, buffer);
	}
}																																								//	end

//------------------------------------------------------------------------------
#define	NUM_SUBFILES	10
void	ConvertFiles (void)
{
	char	*filename[] = {
												"Level\\level.dev",
												"Level\\level.trg",
												"Level\\level.eks",
												"Level\\level.crx",
												"Level\\level.grd",
												"Level\\level.vis",
												"Level\\desc.txt",
												"Level\\mission.txt",
												"Level\\level.brf",
												"Level\\level.smg"
											};
	char	*subname[] =	{
												"Devices",
												"Triggers",
												"Eksekaren",
												"Crixa",
												"Grid",
												"World",
												"Descriptions",
												"Missions",
												"Brief",
												"StateMgr"
											};
	DiskFile	output ("Level01.dat");
	if (output.Open (DF_WRITE_ONLY))
	{
		SubFile	subfiles[NUM_SUBFILES];
		int			count = NUM_SUBFILES;
		output.Write (&count, sizeof (int));
		output.Write (subfiles, sizeof (SubFile), NUM_SUBFILES);
		for (int i = 0; i < NUM_SUBFILES; i++)
		{
			DiskFile	input (filename[i]);
			if (input.Open ())
			{
				int	length = input.Length ();
				char	*data = new char[length];
				input.Read (data, length);
				strcpy (subfiles[i].name, subname[i]);
				subfiles[i].start = output.Current ();
				output.Write (data, length);
				subfiles[i].end = output.Current ();
				delete[] data;
				input.Close ();
			}
		}
		output.Reset ();
		output.Write (&count, sizeof (int));
		output.Write (subfiles, sizeof (SubFile), NUM_SUBFILES);
		output.Close ();
	}
}

//------------------------------------------------------------------------------
static	void	Open (HINSTANCE instance, int show)																//	initialize the application
{																																								//	begin
//	ReportKeyNames ();
	//ConvertFiles ();
	SetCursor (0);																																//	we don't want a cursor
	DefinePI ();																																	//	define the mathematical relationship known as PI
	DefineDirections ();																													//	define the direction vectors for angles from 0 to 2 PI
	BuildTable_h2i ();																														//	build the lookup table for the hex to int conversion
	INode::PreAllocate (5000);
	LinkNode::PreAllocate (10000);
	PieceNode::PreAllocate (500);
	DNode::PreAllocate (500);
#if	GET_PARAMETERS_FROM_DATABASE																								//	if we are using the parameters database
	gParmBase = new ParmBase ("parameters.h");																		//	build the parameters database
#if WANT_COMMAND_LINE
	gCommand[0] = 0;																															//	terminate the command string
#endif
#endif																																					//	end if
	WNDCLASS	wClass;																															//	window class definition
	wClass.style = CS_HREDRAW | CS_VREDRAW;																				//	style flags
	wClass.lpfnWndProc = WinProc;																									//	set the window proc
	wClass.cbClsExtra = 0;																												//	no extra data
	wClass.cbWndExtra = 0;																												//	no extra data
	wClass.hInstance = instance;																									//	the application instance
	wClass.hIcon = LoadIcon (instance, IDI_APPLICATION);													//	generic application
	wClass.hCursor = LoadCursor (0, IDC_ARROW);																		//	normal cursor
	wClass.hbrBackground = 0;																											//	no background
	wClass.lpszMenuName = CLASS_NAME;																							//	class name
	wClass.lpszClassName = CLASS_NAME;																						//	class name
	RegisterClass (&wClass);																											//	register the class

#if CRIXA_DEBUG_HARMONY
	gWindow = CreateWindowEx (0, CLASS_NAME, CLASS_NAME, WS_POPUP,								//	create the window
														0, 0, SCRN_WIDTH, SCRN_HEIGHT,											//	create the window (cont'd)
														0, 0, instance, 0);																	//	create the window (cont'd)
#else
	gWindow = CreateWindowEx (WS_EX_TOPMOST, CLASS_NAME, CLASS_NAME, WS_POPUP,
														0, 0, GetSystemMetrics (SM_CXSCREEN), GetSystemMetrics (SM_CYSCREEN),
														0, 0, instance, 0);																	//	create the window
#endif

	GetClipCursor (&oldClip);																											//	get the current clip rectangle for later restoration
	ShowWindow (gWindow, show);																										//	show the window
	UpdateWindow (gWindow);																												//	update the window
	SetFocus (gWindow);																														//	set the window focus to my window
	DirectX::Open (gWindow);																											//	open the direct X environment
	Network::Open ();																															//	open the network subsystem
	gPlayController = new PlayController (gWindow);																//	allocate the play controller
	Controller::Switch (PLAY_CONTROLLER_ID);																			//	switch to the movie controller
}																																								//	end

//------------------------------------------------------------------------------
long __stdcall ExceptionHandler (struct _EXCEPTION_POINTERS *ExInfo)						//	exception handler for terminal errors
{
	long returncode = EXCEPTION_CONTINUE_SEARCH;																	//	Let default ExceptionFilter handle dialog
	DEBUG_HARD ("\nException Handler: *** Terminal Exception Caught ***\n");	

	DWORD code = ExInfo->ExceptionRecord->ExceptionCode;													//	Grab the exception type
	PVOID addr = ExInfo->ExceptionRecord->ExceptionAddress;												//	Grab the address

	DEBUG_HARD ("    Exception code 0x%0X occured at Address 0x%0X\n", code, addr);

	DEBUG_HARD ("    Exception Type: ");
	switch (code) {
		case EXCEPTION_SINGLE_STEP:
			DEBUG_HARD ("Debugger Single-Step\n");
			break;
		case EXCEPTION_BREAKPOINT:
			DEBUG_HARD ("Debugger Breakpoint\n");
			break;
		case EXCEPTION_ACCESS_VIOLATION:
			DEBUG_HARD ("Access Violation\n");
			break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			DEBUG_HARD ("Array Bound Exceeded\n");
			break;
		case EXCEPTION_DATATYPE_MISALIGNMENT:
			DEBUG_HARD ("Datatype misalignment\n");
			break;
	  case EXCEPTION_FLT_DENORMAL_OPERAND:
			DEBUG_HARD ("FLT too small to represent\n");
			break;
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			DEBUG_HARD ("FLT divide by zero\n");
			break;
		case EXCEPTION_FLT_INEXACT_RESULT:
			DEBUG_HARD ("FLT inexact result\n");
			break;
		case EXCEPTION_FLT_INVALID_OPERATION:
			DEBUG_HARD ("FLT invalid operation\n");
			break;
		case EXCEPTION_FLT_OVERFLOW:
			DEBUG_HARD ("FLT overflow\n");
			break;
	  case EXCEPTION_FLT_STACK_CHECK:
			DEBUG_HARD ("FLT stack blown \n");
			break;
		case EXCEPTION_FLT_UNDERFLOW:
			DEBUG_HARD ("FLT underflow\n");
			break;
		case EXCEPTION_ILLEGAL_INSTRUCTION:
			DEBUG_HARD ("Illegal instruction\n");
			break;
		case EXCEPTION_IN_PAGE_ERROR:
			DEBUG_HARD ("Page Exception (running over network?)\n");
			break;
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			DEBUG_HARD ("Integer divide by zero\n");
			break;
		case EXCEPTION_INT_OVERFLOW:
			DEBUG_HARD ("Integer overflow\n");
			break;
		case EXCEPTION_INVALID_DISPOSITION:
			DEBUG_HARD ("Invalid Disposition. Very weird.\n");
			break;
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			DEBUG_HARD ("Can't continue execution after terminal exception.\n");
			break;
		case EXCEPTION_PRIV_INSTRUCTION:
			DEBUG_HARD ("Can't execute Privilleged Instruction in current machine mode\n");
			break;
		case EXCEPTION_STACK_OVERFLOW:
			DEBUG_HARD ("Stack overflow\n");
			break;
		default:
			DEBUG_HARD ("Unrecognized Code\n");
	} 

	DEBUG_HARD ("\nException Handler: *** Restoring Debug services ***\n");
	DEBUG_HARD ("    Closing Network and DirectX.\n");

	if (gDisplay) delete gDisplay;
	Network::Close ();
	DirectX::Close ();
	if (!CloseWindow (gWindow))
	{
		DEBUG_HARD ("    CloseWindow() failed. Trying DestroyWindow()...\n");
		if (!DestroyWindow (gWindow)) 
			DEBUG_HARD ("    DestroyWindow() failed. System probably needs reboot\n");
	}
	else {
		DEBUG_HARD ("    Crixa Window hidden...\n");
		DEBUG_HARD ("    Debug Services Restored!\n");
	}
	ClipCursor (&oldClip);

	DEBUG_HARD ("\n*** Exiting Exception Handler ***\n");
	DEBUG_HARD   ("***   Good luck, Commander!   ***\n");

	return returncode;
}

//------------------------------------------------------------------------------
static	void	Loop (void)																												//	the main application loop
{																																								//	begin
	Bool	done = FALSE;																														//	the done flag
	while (!done)																																	//	loop until we are done
		if (PeekMessage (&gMessage, 0, 0, 0, PM_NOREMOVE))													//	if there is a message in the queue
			if (GetMessage (&gMessage, gWindow, 0, 0) == TRUE)												//	if the message is not a quit message
				DispatchMessage (&gMessage);																						//	send the message to the appropriate window
			else																																			//	otherwise we want to quit
				done = TRUE;																														//	set the finished flag
		else if (gActive && gPlaying)																								//	otherwise there is no message in the queue
			Controller::GetControl ();																								//	if the application is active, run the current controller
		else																																				//	otherwise, the application is not active
			WaitMessage ();																														//	just wait for a message
}																																								//	end

//------------------------------------------------------------------------------
static	void	Close (void)																											//	close the application
{																																								//	begin
	delete gPlayController;																												//	clean up the play controller when done
	DEBUG_STR("\nWinMain: Exiting Application\n");
	Network::Close ();																														//	close the network subsystem
	DirectX::Close ();																														//	close up the direct X environment
	ClipCursor (&oldClip);																												//	restore the old cursor clipping rectangle
	char	copyright[256];																													//	a string for a copyright notice
	strcpy (copyright, WADE);																											//	just use the string to embed it in the code
}																																								//	end

//------------------------------------------------------------------------------
int PASCAL	WinMain (HINSTANCE inst, HINSTANCE prev, LPSTR cmdLine, int show)		//	windows main
{																																								//	begin

#define FPU_EXCEPTIONS 1
#define FPU_SINGLE_PRECISION 1

	DEBUG_ON();																																		// turn on debug logging (just one place)
	DEBUG_STR ("\nWinMain:\n");																									

	_set_new_handler (NewHandler);																								// report out of memory conditions (err mgr.cpp)
	DEBUG_STR ("    New Memory Handler Installed\n");

	SetUnhandledExceptionFilter(ExceptionHandler);																// set up a handler to get full-screen debug mode
	DEBUG_STR ("    Exception Handler Installed for Full-Screen Debug\n");

	#if FPU_EXCEPTIONS
  int cw = _controlfp( 0,0 );																										// Get the default control word.
  cw &=~(EM_ZERODIVIDE|EM_DENORMAL|EM_INVALID);																	// Set the exception masks OFF, turn exceptions on.
  _controlfp (cw, MCW_EM);																											// Set the control word.
	DEBUG_STR ("    FP ZeroDivide, DeNormal, Invalid Exceptions are enabled\n");
	#endif

	#if FPU_SINGLE_PRECISION
	_controlfp (_PC_24, MCW_PC);
	DEBUG_STR ("    FP Single Precision enabled\n");
	#endif

	Open (inst, show);																														//	open the application
	Loop ();																																			//	do the main loop
	Close ();																																			//	close the application

	DEBUG_OFF();

	return TRUE;																																	//	return success
}																																								//	end

//------------------------------------------------------------------------------
