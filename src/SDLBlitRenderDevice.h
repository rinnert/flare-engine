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

#pragma once
#ifndef SDLBLITRENDERDEVICE_H
#define SDLBLITRENDERDEVICE_H

#include "RenderDevice.h"

/** Provide rendering device using SDL_BlitSurface backend.
 *
 * Provide an SDL_BlitSurface implementation for renderning a Renderable to
 * the screen.  Simply dispatches rendering to SDL_BlitSurface().
 *
 * As this is for the FLARE engine, the implementation use the the engine's
 * global settings context, which is included by the interface.
 *
 * @class SDLBlitRenderDevice 
 * @see RenderDevice
 * @author Kurt Rinnert 
 * @date 2013-07-06
 *
 */
class SDLBlitRenderDevice : public RenderDevice {

  public:

    /** Initialize base class and report rendering device in use.
     */
    SDLBlitRenderDevice();

    /** Create context on startup.
     */
    virtual SDL_Surface *create_context(
        int width, 
        int height,
        bool full_screen
        );

    /** Render surface to screen. 
     */ 
    virtual int render(Renderable& r); 

    /** Draw pixel to screen. 
     */ 
    virtual void draw_pixel_to_screen(
        int x,
        int y,
        Uint32 color
        );

    /** Blank the screen.
     */
    virtual void blank_screen();

    /** Commit the next frame to dispay.
     */
    virtual void commit_frame();

    /** Destroy context on exit.
     */
    virtual void destroy_context(); 

};

#endif // SDLBLITRENDERDEVICE_H
