/*
Copyright © 2011-2012 Clint Bellanger
Copyright © 2013 Henrik Andersson
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

/**
SharedResources

"Global" sort of system resources that are used by most game classes.
Only one instance of these classes are needed by the engine.
Generic objects only. Game-specific objects don't belong here.
Created and destroyed by main.cpp
**/

#pragma once
#ifndef SHARED_RESOURCES_H
#define SHARED_RESOURCES_H

#include "CommonIncludes.h"
#include "AnimationManager.h"
#include "CombatText.h"
#include "FontEngine.h"
#include "ImageManager.h"
#include "InputState.h"
#include "MessageEngine.h"
#include "ModManager.h"
#include "SoundManager.h"
#include "RenderDevice.h"
#ifdef WITH_OPENGL
#include "OpenGLUtils.h"
#endif // WITH_OPENGL

extern SDL_Surface *screen;
extern SDL_Joystick *joy;

extern AnimationManager *anim;
extern CombatText *comb;
extern FontEngine *font;
extern ImageManager *imag;
extern InputState *inpt;
extern MessageEngine *msg;
extern ModManager *mods;
extern SoundManager *snd;

extern SDL_Surface *icon_atlas;
#ifdef WITH_OPENGL
extern GLuint icon_texture_atlas;
#endif // WITH_OPENGL

extern RenderDevice *render_device;

#ifdef WITH_OPENGL
extern OpenGLResourceManager *gl_resources;
#endif // WITH_OPENGL
#endif
