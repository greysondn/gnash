// 
//   Copyright (C) 2005, 2006 Free Software Foundation, Inc.
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

// Linking Gnash statically or dynamically with other modules is making a
// combined work based on Gnash. Thus, the terms and conditions of the GNU
// General Public License cover the whole combination.
//
// As a special exception, the copyright holders of Gnash give you
// permission to combine Gnash with free software programs or libraries
// that are released under the GNU LGPL and with code included in any
// release of Talkback distributed by the Mozilla Foundation. You may
// copy and distribute such a system following the terms of the GNU GPL
// for all but the LGPL-covered parts and Talkback, and following the
// LGPL for the LGPL-covered parts.
//
// Note that people who make modified versions of Gnash are not obligated
// to grant this special exception for their modified versions; it is their
// choice whether to do so. The GNU General Public License gives permission
// to release a modified version without this exception; this exception
// also makes it possible to release a modified version which carries
// forward this exception.
// 
//
//

// Code for the text tags.


#ifndef GNASH_PARSER_TEXT_CHARACTER_DEF_H
#define GNASH_PARSER_TEXT_CHARACTER_DEF_H

#include "character_def.h" // for inheritance
#include "text.h" // for text_glyph_record
#include "textformat.h" // maybe we should include it here
#include "styles.h" 
#include "rect.h" // for composition

namespace gnash {

class movie_definition;

/// Text character 
//
/// This is either read from SWF stream 
/// or (hopefully) created with scripting
///
class text_character_def : public character_def
{
public:
	movie_definition*	m_root_def;
	rect	m_rect;
	matrix	m_matrix;
	std::vector<text_glyph_record>	m_text_glyph_records;

	text_character_def(movie_definition* root_def)
		:
		m_root_def(root_def)
	{
		assert(m_root_def);
	}

	void read(stream* in, int tag_type, movie_definition* m);

	/// Draw the string.
	void display(character* inst);
	
	const rect&	get_bound() const {
    // TODO: There is a m_matrix field in the definition(!) that's currently 
    // ignored. Don't know if it needs to be transformed... 
    return m_rect; 
  }
	
};


} // namespace gnash

#endif // GNASH_PARSER_TEXT_CHARACTER_DEF_H
