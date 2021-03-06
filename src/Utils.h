/*
Copyright © 2011-2012 Clint Bellanger
Copyright © 2012 Stefan Beller
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
 * Utils
 *
 * Various utility structures, enums, function
 */

#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#ifdef WITH_OPENGL
#include <SDL_opengl.h>
#endif // WITH_OPENGL

#include "CommonIncludes.h"

class Point {
public:
	int x,y;
	Point() : x(0), y(0) {}
	Point(int _x, int _y) : x(_x), y(_y) {}
};

class FPoint {
public:
	float x,y;
};

// message passing struct for various sprites rendered map inline
struct Renderable {
	SDL_Surface *sprite; // image to be used
	SDL_Rect local_frame;
	SDL_Rect src; // location on the sprite in pixel coordinates.
#ifdef WITH_OPENGL
	float gl_src[4]; // location on the sprite in texture coordinates.
	GLuint texture;
#endif // WITH_OPENGL


	Point map_pos;     // The map location on the floor between someone's feet
	Point offset;      // offset from map_pos to topleft corner of sprite
	uint64_t prio;     // 64-32 bit for map position, 31-16 for intertile position, 15-0 user dependent, such as Avatar.

	Renderable()
		: sprite(0)
		, local_frame(SDL_Rect())
		, src(SDL_Rect())
#ifdef WITH_OPENGL
		, texture(0)
#endif // WITH_OPENGL
		, map_pos()
		, offset()
		, prio(0) {
		;
	}

#ifdef WITH_OPENGL
	void set_graphics(SDL_Surface *s, SDL_Rect *texture_clip=0, GLuint t=0);
#else // WITH_OPENGL
	void set_graphics(SDL_Surface *s, SDL_Rect *texture_clip=0);
#endif // WITH_OPENGL
	void clear_graphics();
	void clear_texture();
	void set_clip(const SDL_Rect& clip);
	void set_clip(
		const int x,
		const int y,
		const int w,
		const int h
	);
	void set_dest(const SDL_Rect& dest) {
		map_pos.x = dest.x;
		map_pos.y = dest.y;
	}
	void set_dest(const Point& dest) {
		map_pos.x = dest.x;
		map_pos.y = dest.y;
	}
	void set_dest(int x, int y) {
		map_pos.x = x;
		map_pos.y = y;
	}
};

class Event_Component {
public:
	std::string type;
	std::string s;
	int x;
	int y;
	int z;
	int a;
	int b;

	Event_Component()
		: type("")
		, s("")
		, x(0)
		, y(0)
		, z(0)
		, a(0)
		, b(0)
	{}
};

// Utility Functions
int round(float f);
Point round(FPoint fp);
Point screen_to_map(int x, int y, int camx, int camy);
Point map_to_screen(int x, int y, int camx, int camy);
Point center_tile(Point p);
Point map_to_collision(Point p);
Point collision_to_map(Point p);
FPoint calcVector(Point pos, int direction, int dist);
float calcDist(Point p1, Point p2);
float calcTheta(int x1, int y1, int x2, int y2);
int calcDirection(int x0, int y0, int x1, int y1);
int calcDirection(const Point &src, const Point &dst);
bool isWithin(Point center, int radius, Point target);
bool isWithin(SDL_Rect r, Point target);

Uint32 readPixel(SDL_Surface *screen, int x, int y);
void drawPixel(SDL_Surface *screen, int x, int y, Uint32 color);
void drawLine(SDL_Surface *screen, int x0, int y0, int x1, int y1, Uint32 color);
void drawLine(SDL_Surface *screen, Point pos0, Point pos1, Uint32 color);
void drawRectangle(SDL_Surface *surface, Point pos0, Point pos1, Uint32 color);
bool checkPixel(Point px, SDL_Surface *surface);


/**
 * Creates a SDL_Surface.
 * The SDL_HWSURFACE or SDL_SWSURFACE flag is set according
 * to settings. The result is a surface which has the same format as the
 * screen surface.
 * Additionally the alpha flag is set, so transparent blits are possible.
 */
SDL_Surface* createAlphaSurface(int width, int height);

/**
 * Creates a SDL_Surface.
 * The SDL_HWSURFACE or SDL_SWSURFACE flag is set according
 * to settings. The result is a surface which has the same format as the
 * screen surface.
 * The bright pink (rgb 0xff00ff) is set as transparent color.
 */
SDL_Surface* createSurface(int width, int height);

SDL_Surface* scaleSurface(SDL_Surface *source, int width, int height);

/**
 * @brief loadGraphicSurface loads an image from a file.
 * @param filename
 *        The parameter filename is mandatory and specifies the image to be
 *        loaded. The filename will be located via the modmanager.
 * @param errormessage
 *        This is an optional parameter, which defines which error message
 *        should be displayed. If the errormessage is an empty string, no error
 *        message will be printed at all.
 * @param IfNotFoundExit
 *        If this optional boolean parameter is set to true, the program will
 *        shutdown sdl and quit, if the specified image is not found.
 * @param HavePinkColorKey
 *        This optional parameter specifies whether a color key with
 *        RGB(0xff, 0, 0xff) should be applied to the image.
 * @return
 *        Returns the SDL_Surface of the specified image or NULL if not
 *        successful
 */

SDL_Surface* loadGraphicSurface(std::string filename,
								std::string errormessage = "Couldn't load image",
								bool IfNotFoundExit = false,
								bool HavePinkColorKey = false);

std::string abbreviateKilo(int amount);

Renderable loadIcons();
#endif
