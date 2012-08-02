
				__asm																																		//	asm run blit
				{																																				//	begin asm blit
					mov	eax, isrc																													//	move the source pointer for the bitmap into eax
					mov ebx, idst																													//	move the destination pointer for the bitmap into ebx
					mov ecx, len																													//	move the length of the run into ecx
					mov esi, eax																													//	move the source into esi
					mov edi, ebx																													//	move the destination into edi
					shr ecx, 1																														//	divide the length by two, for dword length writes
					jnc noextraword																												//	if there were no bits shifted out, then skip ahead
					mov ax, [esi]																													//	otherwise, load a pixel from the source
					add esi, 2																														//	advance the source pointer
					mov [edi], ax																													//	move the pixel to the destination
					add edi, 2																														//	advance the destination pointer
				noextraword:																														//	jump label for an even # of pixels
					cmp	ecx, 0																														//	make sure there are some pixels to blit
					je	done																															//	if not, then we're done
				looper:																																	//	loop label
					mov eax, [esi]																												//	move two pixels into eax from the source
					add esi, 4																														//	advance the source pointer by two pixels
					mov [edi], eax																												//	place two pixels at the destination
					add edi, 4																														//	advance the destination pointer
					dec	ecx																																//	subtract from the (2) pixel count
					jnz looper																														//	if we're not at zero, jump back up
				done:																																		//	that's all folks
				}																																				//	end asm blit
