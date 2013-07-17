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

#include<iostream>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "SharedResources.h"
#include "SDL_gfxBlitFunc.h"
#include "Utils.h"

#include "OpenGLUtils.h"

using namespace std;

OpenGLResourceManager::OpenGLResourceManager()
  : image_buffer(160*120*4)
  , image_buffer_pixels(&(image_buffer[0])) { ; }

void OpenGLResourceManager::render_to_image_buffer(
    char *image,
    int x,
    int y,
    int width,
    int height,
    int image_width,
    int bpp
    ) {
  unsigned int required_size = width*height*bpp;

  if (image_buffer.size() < required_size) {
    image_buffer.resize(required_size);
    image_buffer_pixels = &(image_buffer[0]);
    printf(
        "OpenGLResourceManager: resisizing clipping buffer to %dx%dx%d.\n",
        width,
        height,
        bpp
        );
  }

  char *src = NULL;
  char *dest = image_buffer_pixels;

  for (int row=0; row<height; ++row) {
    src = image + (row+y)*image_width*bpp + x*bpp;
    memcpy(dest+(row*width*bpp),src,width*bpp);
  }

  return;
}

GLuint OpenGLResourceManager::create_texture(
    SDL_Surface *surface,
    SDL_Rect *clip
    ) {
  if ( 0 == surface ) return 0;

  GLuint texture; 
  GLenum texture_format;

  // Determine correct color encoding for texture.
  if (4 == surface->format->BytesPerPixel) {  // contains an alpha channel
    if (0x000000ff == surface->format->Rmask) { texture_format = GL_RGBA; } 
    else { texture_format = GL_BGRA; }
  } else {  // no alpha channel
    if (0x000000ff == surface->format->Rmask) { texture_format = GL_RGB; } 
    else { texture_format = GL_BGR; }
  } 
   
  // If the source rectangle is not specified (NULL) the whole source surface
  // shall be rendered.  Otherwise the origin and dimensions are taken from the
  // clip rectangle.
  SDL_LockSurface(surface);
  char* pixels = (char*)surface->pixels;
  Uint16 surface_w = (Uint16)surface->w;
  Uint16 surface_h = (Uint16)surface->h;
  if (NULL != clip) {
    if (surface_w != clip->w) {
      render_to_image_buffer(
          pixels,
          clip->x,
          clip->y,
          clip->w,
          clip->h,
          surface->w,
          surface->format->BytesPerPixel
          );
      pixels = image_buffer_pixels;
    } else {
      pixels = pixels + clip->w*surface->format->BytesPerPixel*clip->y;
    }

    surface_w = clip->w;
    surface_h = clip->h;
  }

  // The source and destination coodinates and clipping are all set.
  // Create the texture.
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(
      GL_TEXTURE_2D, 
      0, 
      surface->format->BytesPerPixel, 
      surface_w, 
      surface_h, 
      0,
      texture_format, 
      GL_UNSIGNED_BYTE, 
      (void*)pixels
      );
  SDL_UnlockSurface(surface);

  return texture;
}

void OpenGLResourceManager::update_texture(Renderable& r) {
  if (0 != r.texture) glDeleteTextures(1, &(r.texture));
  r.texture = create_texture(r.sprite,&(r.src));
  return;
}

void OpenGLResourceManager::free_texture(Renderable& r) {
  glDeleteTextures(1, &(r.texture));
  r.texture = 0;
  return;
}

#endif // WITH_OPENGL
