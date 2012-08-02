//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Dave Seah
//	Last Revision:	01/27/97
//
//	This blit routine is based on Jeremy's 16-bit blit, extended to 8-bit case.
//
//------------------------------------------------------------------------------


					__asm 
						{
								mov		esi, isrc								//	move the source pointer for the bitmap into esi
								mov		edi, idst								//	move the destination pointer for the bitmap into esi

								mov		ecx, len								//	move the length of the run into ecx
								nop														//	shr is not pairable in v pipe

								shr		ecx, 1									//	divide the length by two
								jnc		checkbit1								//	if there were no bits shifted out, then skip ahead

								mov		al, [esi]								//	otherwise, load a pixel from the source
								inc		esi											//	advance the source pointer

								mov		[edi], al								//	move the pixel to the destination
								inc		edi											//	advance the destination pointer

						checkbit1:
								shr		ecx, 1									//	divide by two again
								jnc		preloop									//	no bit? then get ready to write
								
								mov		ax, word ptr [esi]			//	load a word from source
								add		esi,2										//	advance the source pointer

								mov		word ptr [edi], ax			//	move the pixel to the destination
								add		edi,2										//	advance the destination pointer

						preloop:													//	jump label for an even # of pixels
								cmp		ecx, 0									//	make sure there are some pixels to blit
								je		done										//	if not, then we're done

						looper:														
								mov		eax, [esi]							//	move two pixels into eax from the source
								add		esi, 4									//	advance the source pointer by two pixels
								mov		[edi], eax							//	place two pixels at the destination
								add		edi, 4									//	advance the destination pointer
								dec		ecx											//	subtract from the (2) pixel count
								jnz		looper									//	if we're not at zero, jump back up
						done:															//	that's all folks
	
					}	
