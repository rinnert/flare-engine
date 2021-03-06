/*
Copyright © 2011-2012 kitano
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
 * class MenuExit
 */

#include "MenuExit.h"
#include "SharedResources.h"
#include "Settings.h"

MenuExit::MenuExit() : Menu() {

	exitClicked = false;

	buttonExit = new WidgetButton("images/menus/buttons/button_default.png");
	buttonExit->label = msg->get("Exit");

	buttonClose = new WidgetButton("images/menus/buttons/button_x.png");

	background.set_graphics(loadGraphicSurface("images/menus/confirm_bg.png"));
	tablist.add(buttonExit);
	tablist.add(buttonClose);
}

void MenuExit::update() {
	buttonExit->pos.x = VIEW_W_HALF - buttonExit->pos.w/2;
	buttonExit->pos.y = VIEW_H/2;
	buttonExit->refresh();

	buttonClose->pos.x = window_area.x + window_area.w;
	buttonClose->pos.y = window_area.y;

	label.set(window_area.x + window_area.w/2, window_area.y + window_area.h - (buttonExit->pos.h * 2), JUSTIFY_CENTER, VALIGN_TOP, msg->get("Save and exit to title?"), font->getColor("menu_normal"));
}

void MenuExit::logic() {
	if (visible) {
		if (NO_MOUSE) {
			tablist.logic();
		}

		if (buttonExit->checkClick()) {
			exitClicked = true;
		}
		if (buttonClose->checkClick()) {
			visible = false;
		}
	}
}

void MenuExit::render() {
	if (visible) {
		// background
		render_device->render(background);

		label.render();

		buttonExit->render();
		buttonClose->render();
	}
}

MenuExit::~MenuExit() {
	delete buttonExit;
	delete buttonClose;
	background.clear_graphics();
}

