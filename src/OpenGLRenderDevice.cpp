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

#include <SDL_opengl.h>

#include "SharedResources.h"
#include "SDL_gfxBlitFunc.h"
#include "Utils.h"

#include "OpenGLRenderDevice.h"

using namespace std;

OpenGLRenderDevice::OpenGLRenderDevice() 
  : bound_texture(0) {
  cout << "Using Render Device: OpenGLRenderDevice" << endl;
  printf("GL_RENDERER   = %s\n", (char*)glGetString(GL_RENDERER));
  printf("GL_VERSION    = %s\n", (char*)glGetString(GL_VERSION));
  printf("GL_VENDOR     = %s\n", (char*)glGetString(GL_VENDOR));
  printf("GL_EXTENSIONS = %s\n", (char*)glGetString(GL_EXTENSIONS));
}

SDL_Surface *OpenGLRenderDevice::create_context(
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
  flags |= SDL_HWSURFACE | SDL_HWACCEL;
  flags |= SDL_GL_DOUBLEBUFFER | SDL_OPENGL;

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

  // Configure OpenGL if all went well.
  if (NULL != view) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
  }

  screen = view;
  is_initialized = true;
  bound_texture = 0;

  return view;
}

int OpenGLRenderDevice::render(Renderable& r) {
  if (NULL == r.sprite) return -1;

  GLuint texture = r.texture;

  // If the Renderable has no texture, create a temporary one.
  // NOTE: this is *very* costly.  Ideally this should never happen.
  if (0 == texture) { 
    cerr << "OpenGLRenderDevice: creating temporary texture. This is bad." << endl;
    texture = gl_resources->create_texture(r.sprite,&(r.src)); 
  }

  // Because switching texture context can be expensive, only bind the texture
  // if it's not currently bound.
  // TODO: Can this responsibility be moved to client (front-end) code?  If so,
  // it could help performance.
  if (bound_texture != texture) { 
    glBindTexture(GL_TEXTURE_2D, texture); 
    bound_texture = texture;
  }

  tx0 = ty0 = 0.0f;
  tx1 = ty1 = 1.0f;

  // The following texture coordinate computations assume the sprite and
  // texture dimensions are identical.  The front-end code is responsible for
  // ensuring that condition.
  if (r.src.w != r.sprite->w) {
    tx0 = (float)r.src.x/r.sprite->w;
    tx1 = tx0 + (float)r.src.w/r.sprite->w;
  }
  if (r.src.h != r.sprite->h) {
    ty0 = (float)r.src.y/r.sprite->h;
    ty1 = ty0 + (float)r.src.h/r.sprite->h;
  }

  x0 = (float)(r.map_pos.x+r.offset.x);
  y0 = (float)(r.map_pos.x+r.offset.y);
  x1 = x0 + r.src.w;
  y1 = y0 + r.src.h;

  // All coordinate are determined; render now.
  glBegin(GL_QUADS);
  glTexCoord2f(tx0, ty0); glVertex2f(x0, y0);
  glTexCoord2f(tx1, ty0); glVertex2f(x1, y0);
  glTexCoord2f(tx1, ty1); glVertex2f(x1, y1);
  glTexCoord2f(tx0, ty1); glVertex2f(x0, y1);
  glEnd();

  // If the texture is temporary, get rid of it.
  if (0 == r.texture) { 
    glDeleteTextures(1, &texture); 
    bound_texture = 0;
  }

  return 0;
}

void OpenGLRenderDevice::draw_pixel(
    int x,
    int y,
    Uint32 color
    ) {
  Uint8 r,g,b;
  float gl_r,gl_g,gl_b;

  SDL_GetRGB(color,screen->format,&r,&g,&b);
  gl_r = (float)r/255.0f;
  gl_g = (float)g/255.0f;
  gl_b = (float)b/255.0f;


  glDisable(GL_TEXTURE_2D);
  glBegin(GL_POINTS);
  glColor3f(gl_r,gl_g,gl_b);
  glVertex2f(x,y);
  glEnd();
  glEnable(GL_TEXTURE_2D);

  return;
}

void OpenGLRenderDevice::blank_screen() {
  glClear(GL_COLOR_BUFFER_BIT);
  glFinish();
  return;
}

void OpenGLRenderDevice::commit_frame() {
  glFinish();
  SDL_GL_SwapBuffers();
  return;
}

void OpenGLRenderDevice::destroy_context() {
  // Nothing to be done; SDL_Quit() will handle it all
  // for this render device.
  return;
}

#endif // WITH_OPENGL
