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

#ifdef WITH_OPENGL

#pragma once
#ifndef OPENGLOPENGLUTILS_H
#define OPENGLOPENGLUTILS_H

#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>

/** Provide utilities for OpenGL rendering.
 *
 * @author Kurt Rinnert 
 * @date 2013-07-17
 *
 */

/** Provide handling of OpenGL textures.
 *
 * Provide functions for renderning an SLD_surface to an
 * OpenGL texture and a wrappers to handle textures in Renderables. 
 *
 * As this is for the FLARE engine, the implementation use the the engine's
 * global settings context, which is included by the interface.
 *
 * @class OpenGLResourceManager 
 * @see Renderable
 * @author Kurt Rinnert 
 * @date 2013-07-17
 *
 */
class OpenGLResourceManager {

  public:

    /** Initialize.
     */
    OpenGLResourceManager();

    /** Create a texture from an SDL surface.
     */
    GLuint create_texture(
        SDL_Surface *surface=0,
        SDL_Rect *clip=0,
        float priority=0.5f
        ); 

    /** Update a texture of a Renderable.
     *
     *  Create a texture from the SDL_Surface and clipping rectangle
     *  stored in the Renderable. Store the result in the Rendarable's
     *  texture field. If the texture field was not 0, free the old
     *  texture before storing the new one.
     */
    void update_texture(Renderable& r);

    /** Free the texture in a Renderable.
     *
     *  Free the texture stored in a Renderable and set
     *  the Renderable's texture field to 0.
     */
    void free_texture(Renderable& r);

  private:

    void render_to_image_buffer(
        char *image, 
        int x,
        int y,
        int width, 
        int height,
        int image_width, 
        int bpp
        );

  private:

    std::vector<char> image_buffer;
    char *image_buffer_pixels;
};

#endif // OPENGLOPENGLUTILS_H
#endif // WITH_OPENGL
