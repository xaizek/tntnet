/* htmlfilter.cpp
   Copyright (C) 2003-2005 Tommi Maekitalo

This file is part of tntnet.

Tntnet is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Tntnet is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with tntnet; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330,
Boston, MA  02111-1307  USA
*/

#include "tnt/ecppc/htmlfilter.h"
#include <iostream>

namespace tnt
{
  namespace ecppc
  {

    ////////////////////////////////////////////////////////////////////////
    // htmlfilter
    //
    void htmlfilter::operator() (char ch)
    {
      switch (state)
      {
        case state_start:
          if (ch == '<')
            state = state_token;
          else if (std::isspace(ch))
            state = state_space0;
          out << ch;
          break;

        case state_token:
          if (std::isspace(ch))
          {
            out << ' ';
            state = state_tokenspace;
          }
          else
          {
            if (ch == '>')
              state = state_space0;
            out << ch;
          }
          break;

        case state_tokenspace:
          if (!std::isspace(ch))
          {
            if (ch == '>')
              state = state_space0;
            else
              state = state_token;
            out << ch;
          }
          break;

        case state_space0:
          if (std::isspace(ch))
          {
            out << ch;
            state = state_space;
          }
          // no break!!!

        case state_space:
          if (!std::isspace(ch))
          {
            if (ch == '<')
              // hier lassen wir spaces zwischen Tags weg
              state = state_token;
            else
              // wenn zwischen Tokens was anderes ist, als spaces,
              // dann verändern wir das nicht
              state = state_data;
            out << ch;
          }
          break;

        case state_data:
          if (std::isspace(ch))
            html += ch;
          else if (ch == '<')
          {
            // spaces vor '<' wegwerfen
            html.clear();
            state = state_token;
            out << ch;
          }
          else
          {
            if (!html.empty())
            {
              out << html;
              html.clear();
            }
            out << ch;
          }
          break;
      }
    }

    void htmlfilter::flush()
    {
      out << html;
      html.clear();
      out.flush();
    }
  }
}