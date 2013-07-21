/*
Copyright © 2012 Clint Bellanger
Copyright © 2012 davidriod
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
 * class WidgetCheckBox
 */

#include "CommonIncludes.h"
#include "SDL_gfxBlitFunc.h"
#include "SharedResources.h"
#include "WidgetCheckBox.h"
#include "Widget.h"

using namespace std;

WidgetCheckBox::WidgetCheckBox (const string &fname)
	: enabled(true),
	  checked(false),
	  pressed(false) {
	focusable = true;
	cb.set_graphics(loadGraphicSurface(fname, "Couldn't load image", true, false));

	pos.w = cb.sprite->w;
	pos.h = cb.sprite->h / 2;

  cb.set_clip(
      0,
      0,
      pos.w,
      pos.h
      );
	render_to_alpha = false;
}

void WidgetCheckBox::activate() {
	pressed = true;
}

WidgetCheckBox::~WidgetCheckBox () {
	cb.clear_graphics();
}

void WidgetCheckBox::Check () {
	checked = true;
  cb.set_clip(0,pos.h,pos.w,pos.h);
}

void WidgetCheckBox::unCheck () {
	checked = false;
  cb.set_clip(0,0,pos.w,pos.h);
}

void WidgetCheckBox::toggleCheck () {
	checked = !checked;
  cb.set_clip(0,(checked ? pos.h : 0),pos.w,pos.h);
}

bool WidgetCheckBox::checkClick() {
	return checkClick(inpt->mouse.x,inpt->mouse.y);
}

bool WidgetCheckBox::checkClick (int x, int y) {
	if (!enabled) return false;

	Point mouse(x,y);

	// main button already in use, new click not allowed
	if (inpt->lock[MAIN1]) return false;
	if (inpt->lock[ACCEPT]) return false;

	if (pressed && !inpt->lock[MAIN1] && !inpt->lock[ACCEPT]) { // this is a button release
		pressed = false;
		toggleCheck();
		return true;
	}

	if (inpt->pressing[MAIN1]) {
		if (isWithin(pos, mouse)) {
			pressed = true;
			inpt->lock[MAIN1] = true;
		}
	}
	return false;
}

bool WidgetCheckBox::isChecked () const {
	return checked;
}

void WidgetCheckBox::render (SDL_Surface *target) {

  if (NULL == target || screen == target) {
    cb.set_dest(pos);
    render_device->render(cb);
  } else {
    printf("WidgetCheckBox::render(): rendering to buffer...\n");

    SDL_Rect    src;
    src.x = 0;
    src.y = checked ? pos.h : 0;
    src.w = pos.w;
    src.h = pos.h;

    if (render_to_alpha)
      SDL_gfxBlitRGBA(cb.sprite, &src, target, &pos);
    else
      SDL_BlitSurface(cb.sprite, &src, target, &pos);

    if (in_focus) {
      Point topLeft;
      Point bottomRight;
      Uint32 color;

      topLeft.x = pos.x;
      topLeft.y = pos.y;
      bottomRight.x = pos.x + pos.w;
      bottomRight.y = pos.y + pos.h;
      color = SDL_MapRGB(target->format, 255,248,220);

      drawRectangle(target, topLeft, bottomRight, color);
    }
  }
}

