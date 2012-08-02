				__asm																																		//	asm run blit
				{																																				//	begin asm blit
					mov		ecx, len																												//	move the length of the run into ecx
					mov		esi, isrc																												//	move the source into esi
					mov		edi, idst																												//	move the destination into edi

				loopah:																																	//	start with blue component
					mov		dx, [edi]																												//	get a copy of the destination pixel
					mov		bx, [esi]																												//	get the source pixel

					mov		ax, [edi]																												//	get the destination pixel
					and		dx, 0xFFE0																											//	mask out blue with 1111 1111 1110 0000

					and		bx, 0x001F																											//	get the blue component to add
					add		dx, 0x001F																											//	ax now contains "cmp1", the overflow check

					add		ax, bx																													//	add dest to source component

					cmp		ax, dx																													//	check for component overflow
					jle		dogreen																													//	if there's no overflow, then do next component

					mov		ax, dx																													//	otherwise, use cmp1, which is already clamped
				dogreen: 
					mov		dx, ax																													//	get a copy of the destination pixel
					mov		bx, [esi]																												//	get the source pixel

					and		dx, 0xF81F																											//	mask out green 1111 1000 0001 1111
					and		bx, 0x07E0																											//	get the green component to add

					add		dx, 0x07E0																											//	ax now contains "cmp2", the overflow check
					add		ax, bx																													//	add dest to source component

					cmp		ax, dx																													//	check for component overflow
					jle		dored																														//	if there's no overflow, then do next component

					mov		ax, dx																													//	otherwise, use cmp2, which is already clamped
				dored:
					mov		dx, ax																													//	get a copy of the destination pixel
					mov		bx, [esi]																												//	get the source pixel

					and		dx, 0x07FF																											//	mask out red	0000 0111 1111 1111
					and		bx, 0xF800																											//	get the red component to add

					add		dx, 0xF800																											//	ax now contains "cmp3", the overflow check
					add		ax, bx																													//	add dest to source component

					jnc		eloop																														//	if no overflow occured, we're done with this pixel
					mov		ax, dx																													//	otherwise, use cmp3, which is already clamped
				eloop:
					mov		[edi], ax																												//	write the pixel, finally
					add		esi, 2																													//	increment the source

					add		edi, 2																													//	increment the destination
					dec		ecx																															//	subtract from the (2) pixel count

					jnz		loopah																													//	if we're not at zero, jump back up
				}																																				//	end asm blit
