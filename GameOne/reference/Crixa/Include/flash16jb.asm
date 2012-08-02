
				__asm																																		//	asm run blit
				{																																				//	begin asm blit
					mov	esi, isrc																													//	move the source pointer for the bitmap into eax
					mov edi, idst																													//	move the destination pointer for the bitmap into ebx
					mov ecx, len																													//	move the length of the run into ecx
					mov	eax, 0xFFFFFFFF
					shr ecx, 1																														//	divide the length by two, for dword length writes
					jnc noextraword																												//	if there were no bits shifted out, then skip ahead
					mov [edi], ax																													//	move the pixel to the destination
					add edi, 2																														//	advance the destination pointer
				noextraword:																														//	jump label for an even # of pixels
					cmp	ecx, 0																														//	make sure there are some pixels to blit
					je	done																															//	if not, then we're done
				looper:																																	//	loop label
					mov [edi], eax																												//	place two pixels at the destination
					add edi, 4																														//	advance the destination pointer
					dec	ecx																																//	subtract from the (2) pixel count
					jnz looper																														//	if we're not at zero, jump back up
				done:																																		//	that's all folks
				}																																				//	end asm blit
