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
#ifndef RENDERDEVICE_H
#define RENDERDEVICE_H

#include <iostream>

#include <SDL.h>

#include "Settings.h"
#include "Utils.h"


/** Provide abstract interface for FLARE engine rendering devices.
 *
 * Provide an abstract interface for renderning a Renderable to the screen.
 * Each rendering device implementation must fully implement the interface.
 * The idea is that the render() method replicates the behaviour of
 * SDL_BlitSurface() with different rendering backends, but bases on the 
 * information carried in a Renderable struct.
 *
 * As this is for the FLARE engine, implementations use the the engine's global
 * settings context.
 *
 * @class RenderDevice 
 * @author Kurt Rinnert 
 * @date 2013-07-06
 *
 */
class RenderDevice {

  public:

    /** Constuctor.
     */
    RenderDevice() 
      : is_initialized(false)
    {;}

    /** Destructor.
     */
    virtual ~RenderDevice() {;}

    /** Create context on startup.
     */
    virtual SDL_Surface *create_context(
        int width, 
        int height,
        bool full_screen
        ) = 0;

    /** Render a Renderable to the screen. 
     */ 
    virtual int render(Renderable& r) = 0; 

    /** Draw pixel to screen. 
     */ 
    virtual void draw_pixel(
        int x,
        int y,
        Uint32 color
        ) = 0;

    /** Blank the screen.
     */
    virtual void blank_screen() = 0;

    /** Commit the next frame to the display.
     */
    virtual void commit_frame() = 0;

    /** Destroy context on exit.
     */
    virtual void destroy_context() = 0; 

  protected:

    bool is_initialized; ///< true if a context was created once
};

#endif // RENDERDEVICE_H
