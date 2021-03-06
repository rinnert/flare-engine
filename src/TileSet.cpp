/*
Copyright © 2011-2012 Clint Bellanger
Copyright © 2012 Stefan Beller
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
 * class TileSet
 *
 * TileSet storage and file loading
 */

#include "TileSet.h"
#include "SharedResources.h"
#include "FileParser.h"
#include "UtilsParsing.h"
#include "Settings.h"

#include <cstdio>

using namespace std;

#ifdef WITH_OPENGL
Tile_Def::~Tile_Def() {
	glDeleteTextures(textures.size(),&(textures[0]));
}
#endif // WITH_OPENGL

TileSet::TileSet() {
	sprites = NULL;
	reset();
}

void TileSet::reset() {

	SDL_FreeSurface(sprites);

	alpha_background = true;
	trans_r = 255;
	trans_g = 0;
	trans_b = 255;

	sprites = NULL;
	tiles.clear();
	anim.clear();

	max_size_x = 0;
	max_size_y = 0;
}

void TileSet::loadGraphics(const std::string& filename) {
	if (sprites) {
		SDL_FreeSurface(sprites);
		tiles.clear();
	}

	if (!TEXTURE_QUALITY)
		sprites = loadGraphicSurface("images/tilesets/noalpha/" + filename, "Couldn't load image", false, true);

	if (!sprites)
		sprites = loadGraphicSurface("images/tilesets/" + filename);

	for (unsigned int t=0; t<tiles.size(); ++t) {
		Renderable& r = tiles[t].tile;
		r.sprite = sprites;
#ifdef WITH_OPENGL
		if (OPENGL) {
			SDL_Rect clip = r.src;
			r.texture = gl_resources->create_texture(sprites,&clip);
			r.gl_src[0] = r.gl_src[1] = 0.0f;
			r.gl_src[2] = r.gl_src[3] = 1.0f;
			tiles[t].textures.push_back(r.texture);

			if (t < anim.size()) {
				for (unsigned int f=1; f<anim[t].frames; ++f) {
					clip.x = anim[t].pos[f].x;
					clip.y = anim[t].pos[f].y;
					tiles[t].textures.push_back(gl_resources->create_texture(sprites,&clip));
				}
			}
		}
#endif // WITH_OPENGL
	}
}

void TileSet::load(const std::string& filename) {
	if (current_map == filename) return;

	reset();

	FileParser infile;
	string img;

	if (infile.open("tilesetdefs/" + filename)) {
		while (infile.next()) {
			if (infile.key == "tile") {

				infile.val = infile.val + ',';
				unsigned index = eatFirstInt(infile.val, ',');

				if (index >= tiles.size())
					tiles.resize(index + 1);

				tiles[index].tile.src.x = eatFirstInt(infile.val, ',');
				tiles[index].tile.src.y = eatFirstInt(infile.val, ',');
				tiles[index].tile.src.w = eatFirstInt(infile.val, ',');
				tiles[index].tile.src.h = eatFirstInt(infile.val, ',');
				tiles[index].offset.x = eatFirstInt(infile.val, ',');
				tiles[index].offset.y = eatFirstInt(infile.val, ',');
				max_size_x = std::max(max_size_x, (tiles[index].tile.src.w / TILE_W) + 1);
				max_size_y = std::max(max_size_y, (tiles[index].tile.src.h / TILE_H) + 1);
			}
			else if (infile.key == "img") {
				img = infile.val;
			}
			else if (infile.key == "transparency") {
				alpha_background = false;

				infile.val = infile.val + ',';
				trans_r = (Uint8)eatFirstInt(infile.val, ',');
				trans_g = (Uint8)eatFirstInt(infile.val, ',');
				trans_b = (Uint8)eatFirstInt(infile.val, ',');

			}
			else if (infile.key == "animation") {
				int frame = 0;
				unsigned TILE_ID = toInt(infile.nextValue());

				if (TILE_ID >= anim.size())
					anim.resize(TILE_ID + 1);

				string repeat_val = infile.nextValue();
				while (repeat_val != "") {
					anim[TILE_ID].frames++;
					anim[TILE_ID].pos.resize(frame + 1);
					anim[TILE_ID].frame_duration.resize(frame + 1);
					anim[TILE_ID].pos[frame].x = toInt(repeat_val);
					anim[TILE_ID].pos[frame].y = toInt(infile.nextValue());
					anim[TILE_ID].frame_duration[frame] = toInt(infile.nextValue());

					frame++;
					repeat_val = infile.nextValue();
				}
			}
		}
		infile.close();
		loadGraphics(img);
	}

	current_map = filename;
}

void TileSet::logic() {

	if (ANIMATED_TILES) {
		for (unsigned i = 0; i < anim.size() ; i++) {
			Tile_Anim &an = anim[i];
			if (!an.frames)
				continue;
			if (an.duration >= an.frame_duration[an.current_frame]) {
				tiles[i].tile.src.x = an.pos[an.current_frame].x;
				tiles[i].tile.src.y = an.pos[an.current_frame].y;
				an.duration = 0;
				an.current_frame = (an.current_frame + 1) % an.frames;
#ifdef WITH_OPENGL
				if (OPENGL) {
					tiles[i].tile.texture = tiles[i].textures[an.current_frame];
				}
#endif // WITH_OPENGL
			}
			an.duration++;
		}
	}
}

TileSet::~TileSet() {
	SDL_FreeSurface(sprites);
}
