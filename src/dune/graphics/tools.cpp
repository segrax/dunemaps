/* OD2 - Dune II Clone
 *  
 * Copyright (C) 2009 Robert Crossfield		<robert.crossfield@strobs.com>
 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *  
 * 
 * $Id$
 * 
 */

#include "stdafx.h"
#include "tools.h"

int horizontalInvert( SDL_Surface *surface ) {
	return 1;
}

SDL_Surface *SDL_HorizontalInvert( SDL_Surface *image) {
	SDL_Surface *surface =  SDL_CreateRGBSurface(SDL_SWSURFACE, image->w, image->h, 8, 0, 0, 0, 0);
	
	SDL_FillRect( surface, 0, 0xFF);
	SDL_SetColorKey( surface, SDL_SRCCOLORKEY, 0);
	SDL_SetColors(surface, image->format->palette->colors, 0, image->format->palette->ncolors);

	byte *src = (byte*) image->pixels;
	byte *dst;

	word gap = (image->pitch - image->w);

	for(int y = 0; y < surface->h; y++) {

		dst  = ((byte*) surface->pixels) + surface->pitch + (( y * surface->pitch )  -gap) - 1;

		for(int x = 0; x < surface->w; x++) {
			
			*dst = *src;
			
			--dst;
			++src;

		}

		src += gap;
	}
			
	return surface;
}
