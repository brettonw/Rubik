//------------------------------------------------------------------------------
//	Copyright ©1996-97 qualia, inc. All rights reserved.
//
//	Written by:			Dave Seah
//	Last Revision:	01/27/97
//
//	This blit routine jumps through hoops to attain maximal read and write
//	alignment. Write alignment is favored over read alignment if there's no
//	choice.
//
//------------------------------------------------------------------------------


					__asm 
						{
								mov		ecx,len									// (uv) load the length
								mov		edx,ecx									// (uv) save another copy of len...

								mov		esi,isrc								// (uv) load the source pixels address (run)
								mov		edi,idst								// (uv) load the destination pixels address (screenmem)

								mov		bx, word ptr [esi+2]		// (uv) preload ebx in case we need it
								add		edx,edi									// (uv) compute the destination ending write address 

								cmp		ecx,4										// (u ) check for short runs < 4 bytes
								jae		WritePreQuad						// ( v) no short runs, so let's get going

					//DoSmallCopy:											// *note* going in, [esi] is dword aligned due to sprite format.
								mov		ax, word ptr [esi]			// (uv) get a whole word
								dec		ecx											// (uv) check count

								mov		byte ptr [edi],al				// (uv) write a byte
								je		DoEnd										// ( v) 3 clocks to write 1 byte (+4 if mispredicted branch)

								mov		byte ptr [edi+1],ah			// (uv) write another byte
								dec		ecx											// (uv) check count

								je		DoEnd										// ( v) 5 clocks to write 2 bytes (+4 if mispredicted branch)
								mov		byte ptr [edi+2],bl			// (uv) write last byte
 
								jmp		DoEnd										// ( v) 6 clocks to write 3 bytes (add misprediction penalties above)

						WritePreQuad:											// *note* ecx contains number of dwords
								mov		ebx,edi									// (uv) preserve

								and		ebx,03h									// (uv) compute jumpoffset1 (low 2 bits)
								jz		WriteQuads							// ( v) if low bits of edi are clear, then we're dword aligned

								cmp		ebx,02h									// (uv) check low bits against 2
								ja		WriteS1									// ( v) if addr 1 byte before, go write 1 byte

								je		WriteS2									// ( v) if addr 2 byte before, go write 2 bytes

					//WriteS3:													// *note* else write 3 bytes...offsets D, EF for aligned writes
								mov		al, byte ptr [esi]			// (uv) get byte to write
								inc		esi											// (uv) increment to next address

								mov		byte ptr [edi],al				// (uv) write the byte
								inc		edi											// (uv) increment to next address

								mov		ax, word ptr[esi]				// (uv) get next word to write
								add		esi,2										// (uv) increment to next address

								mov		word ptr [edi],ax				// (uv) write the word
								add		edi,2										// (uv) increment to next address

								sub		ecx,3										// (uv) decrement count
								jmp		WriteQuads							// ( v) get ready to blast

						WriteS2:
								mov		ax, word ptr [esi]			// (uv) get source byte
								add		esi,2										// (uv) increment source by 2

								mov		word ptr [edi],ax				// (uv) write a byte
								add		edi,2										// (uv) increment dest by 2

								sub		ecx,2										// (uv) decrement count
								jmp		WriteQuads							// ( v) get ready to blast

						WriteS1:
								mov		al, byte ptr [esi]			// (uv) get source byte
								inc		esi											// (uv) increment source

								mov		byte ptr [edi],al				// (uv) write a byte
								inc		edi											// (uv) increment dest

								dec		ecx											// (uv) decrement count

						WriteQuads:												// *note* ecx contains number of dwords
								shr		ecx,2										// (u ) adjust count for dwords

								REP		MOVSD										// (np) esi,edi, and ecx are setup! 12+ecx clocks
																							//			This is not as fast as FILD/FISTP pairs on Pentium, 
																							//			but the sprite run data alignment prevents it anyway.
																							//			Optimal on 486 and PentiumPro, though.

					//DoPostQuad:												// *note* edx contains ending address, computed at start
								sub		edx,edi									// (uv) esi is pointer in mem, edx is last address to write
								jz		DoEnd										// ( v) no more bytes, so we're done

								cmp		edx,02h									// (uv) edx should be between 0 and 3
								je		WriteE2									// ( v) write 2 bytes

								ja		WriteE3									// ( v) write 1 byte

					//WriteE1:													// *note* this isn't exactly a mirror of WriteS code
								mov		al, byte ptr [esi]			// (uv) 

								mov		byte ptr [edi],al				// (uv) AGI stall...oh well
								jmp		DoEnd										// ( v) but pairs with jump

						WriteE2:
								mov		ax,word ptr[esi]				// (uv)

								mov		word ptr[edi],ax				// (uv) AGI stall...oh well
								jmp		DoEnd										// ( v) but pairs with jump

						WriteE3:
								mov		ax,word ptr[esi]				// (uv) 
								add		esi,2										// (uv)

								mov		word ptr[edi],ax				// (uv) 
								add		edi,2										// (uv)

								mov		al,byte ptr[esi]				// (uv)
								mov		byte ptr[edi], al				// (uv)	AGI stall...oh well 
						DoEnd:		
					}	
