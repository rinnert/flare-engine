/*
Copyright © 2013 Kurt Rinnert

This file is part of FLARE.

FLARE is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

FLARE is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
FLARE.  If not, see http://www.gnu.org/licenses/
*/

#include<iostream>

#include<stdio.h>
#include<stdlib.h>

#include "SharedResources.h"

#include "SDLBlitRenderDevice.h"

using namespace std;

SDLBlitRenderDevice::SDLBlitRenderDevice()
{
  cout << "Using Render Device: SDLBlitRenderDevice" << endl;
}

SDL_Surface *SDLBlitRenderDevice::create_context(
    int width, 
    int height,
    bool full_screen
    ) {
  // Is there anything to do? If not, simply return the 
  // current context.
  if (width == VIEW_W && 
      height == VIEW_H && 
      full_screen == FULLSCREEN &&
      is_initialized
      ) {
    return screen;
  }

  // Check all mods support the requested resolution.
	if (MIN_VIEW_W > width && MIN_VIEW_H > height) {
		fprintf (
        stderr, 
        "A mod is requiring a minimum resolution of %dx%d\n", 
        MIN_VIEW_W, 
        MIN_VIEW_H
        );
		if (width < MIN_VIEW_W) width = MIN_VIEW_W;
		if (height < MIN_VIEW_H) height = MIN_VIEW_H;
	}

  // Make sure we free the resources if this is not
  // the first request for a context.
  if (is_initialized) {
    SDL_FreeSurface(screen);
  }

  Uint32 flags = 0;
  if (full_screen) { flags |= SDL_FULLSCREEN; }
  if (DOUBLEBUF) { flags |= SDL_DOUBLEBUF; }
  if (HWSURFACE) { flags |= SDL_HWSURFACE | SDL_HWACCEL; }
  else { flags |= SDL_SWSURFACE; }

  SDL_Surface *view = SDL_SetVideoMode (width, height, 0, flags);
  if (view == NULL) {

    fprintf (stderr, "Error during SDL_SetVideoMode: %s\n", SDL_GetError());

    if (!is_initialized) 
    { 
      // If this is the first attempt and it failed we are not
      // getting anywhere.
      SDL_Quit();
      exit(1);
    } else { 
      // We had a working context before, revert to it. 
      //
      // TODO 2013-07-08: while this method is prepared to handle errors when
      // trying to toggle full screen mode, so far no client code actually uses
      // this. How to handle this in clients is a design decision to be made by
      // the community.
      flags &= (~SDL_FULLSCREEN);
      if (FULLSCREEN) { flags |= SDL_FULLSCREEN; }
      view = SDL_SetVideoMode (VIEW_W, VIEW_H, 0, flags); 
    }
  } else if (is_initialized) {
    // We succeeded in setting a new video mode.
		VIEW_W = width;
		VIEW_W_HALF = width/2;
		VIEW_H = height;
		VIEW_H_HALF = height/2;
    FULLSCREEN = full_screen;
  }

  screen = view;
  is_initialized = true;

  return view;
}

int SDLBlitRenderDevice::render(Renderable& r) {
  SDL_Rect dest;

  dest.x = r.map_pos.x + r.offset.x; 
  dest.y = r.map_pos.y + r.offset.y;

  return SDL_BlitSurface(r.sprite, &r.src, screen, &dest);
}


void SDLBlitRenderDevice::draw_pixel_to_screen(
    int x,
    int y,
    Uint32 color
    ) {
	int bpp = screen->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

	switch(bpp) {
	case 1:
		*p = color;
		break;

	case 2:
		*(Uint16 *)p = color;
		break;

	case 3:
#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			p[0] = (color >> 16) & 0xff;
			p[1] = (color >> 8) & 0xff;
			p[2] = color & 0xff;
#else
			p[0] = color & 0xff;
			p[1] = (color >> 8) & 0xff;
			p[2] = (color >> 16) & 0xff;
#endif
		break;

	case 4:
		*(Uint32 *)p = color;
		break;
	}

  return;
}

void SDLBlitRenderDevice::blank_screen() {
  SDL_FillRect(screen, NULL, 0);
  return;
}

void SDLBlitRenderDevice::commit_frame() {
  SDL_Flip(screen);
  return;
}

void SDLBlitRenderDevice::destroy_context() {
  // Nothing to be done; SDL_Quit() will handle it all
  // for this render device.
  return;
}

