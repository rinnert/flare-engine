/*
Copyright © 2011-2012 Clint Bellanger
Copyright © 2012 Igor Paliychuk
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

#include "MenuConfirm.h"
#include "Settings.h"
#include "SharedResources.h"

using namespace std;


MenuConfirm::MenuConfirm(const string& _buttonMsg, const string& _boxMsg) : Menu() {
	confirmClicked = false;
	cancelClicked = false;
	hasConfirmButton = true;
	if (_buttonMsg == "") hasConfirmButton = false;
	// Text to display in confirmation box
	boxMsg = _boxMsg;

	if (hasConfirmButton) {
		buttonConfirm = new WidgetButton("images/menus/buttons/button_default.png");
		buttonConfirm->label = _buttonMsg;
		tablist.add(buttonConfirm);
	}

	buttonClose = new WidgetButton("images/menus/buttons/button_x.png");
	tablist.add(buttonClose);

	background.set_graphics(loadGraphicSurface("images/menus/confirm_bg.png"));
}

void MenuConfirm::update() {
	if (hasConfirmButton) {
		buttonConfirm->pos.x = window_area.x + window_area.w/2 - buttonConfirm->pos.w/2;
		buttonConfirm->pos.y = window_area.y + window_area.h/2;
		buttonConfirm->refresh();
		label.set(window_area.x + window_area.w/2, window_area.y + window_area.h - (buttonConfirm->pos.h * 2), JUSTIFY_CENTER, VALIGN_TOP, boxMsg, font->getColor("menu_normal"));
	}
	else {
		label.set(window_area.x + window_area.w/2, window_area.y + (window_area.h / 4), JUSTIFY_CENTER, VALIGN_TOP, boxMsg, font->getColor("menu_normal"));
	}

	buttonClose->pos.x = window_area.x + window_area.w;
	buttonClose->pos.y = window_area.y;
}

void MenuConfirm::logic() {
	if (visible) {
		tablist.logic();
		confirmClicked = false;
	}
	if (visible && hasConfirmButton) {
		if(buttonConfirm->checkClick()) {
			confirmClicked = true;
		}
		if(buttonClose->checkClick()) {
			visible = false;
			cancelClicked = true;
		}
	}
	else if (visible && !hasConfirmButton) {
		if(buttonClose->checkClick()) {
			visible = false;
			cancelClicked = true;
		}
	}
}

void MenuConfirm::render() {
	// background
	render_device->render(background);

	label.render();

	if (hasConfirmButton) buttonConfirm->render();
	buttonClose->render();
}

MenuConfirm::~MenuConfirm() {
	if (hasConfirmButton) delete buttonConfirm;
	delete buttonClose;
	background.clear_graphics();
}

