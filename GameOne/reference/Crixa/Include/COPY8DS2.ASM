//------------------------------------------------------------------------------
//	Copyright ©1996-1997 qualia, inc. All rights reserved.
//
//	Written by:			Dave Seah
//	Last Revision:	01/25/97				FILD/FISTP move supposed to be fastest on
//																	Pentium (REP MOVSD faster on 486 and PPro)
//------------------------------------------------------------------------------
					__asm 
						{
								mov		ecx,len									// (uv) load the length
								mov		esi,isrc								// (uv) load the source pixels address (run)

								mov		edi,idst								// (uv) load the destination pixels address (screenmem)

								shr		ecx,1										// (u ) check for extra byte
								jnc		NoBit0									// ( v) none? then move on

								mov		al,[esi]								// (uv)	write the byte
								inc		esi											// (uv)

								mov		[edi],al								// (uv) 
								inc		edi											// (uv)

						NoBit0:
								shr		ecx,1										// (u ) check for extra word
								jnc		NoBit1									// ( v) none? then move on

								mov		ax,[esi]								// (uv) write the word
								add		esi,2										// (uv)

								mov		[edi],ax								// (uv) 
								add		edi,2										// (uv)

						NoBit1:														// (u ) check for extra dword
								shr		ecx,1										// ( v) none? then move on
								jnc		NoBit2

								mov		eax,[esi]								// (uv) write the dword
								add		esi,4

								mov		[edi],eax
								add		edi,4

						NoBit2:														
								shr		ecx,1										// (u ) check for qword
								jnc		NoBit3									// ( v) none? then move on

								mov		eax,[esi]								// write first dword
								mov		ebx,[esi+4]

								add		esi,8
								mov		[edi],eax

								mov		[edi+4],ebx
								add		edi,8
						NoBit3:														
								cmp		ecx,0										// anymore to write?
								je		DoEnd										// none? then quit

						Looper:
								fild	qword ptr [esi]					// load 16 bytes to fp stack
								fild	qword ptr [esi+8]
								fxch
								fistp	qword ptr [edi]					// write 16 bytes to fp stack
								fistp	qword ptr [edi+8]
								add		esi,16		
								add		edi,16
								dec		ecx
								jnz		Looper
						DoEnd:		
					}																																				//	end of __asm
