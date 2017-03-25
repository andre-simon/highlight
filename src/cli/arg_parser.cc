/*  Arg_parser - A POSIX/GNU command line argument parser.
    Copyright (C) 2006, 2007, 2008 Antonio Diaz Diaz.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstring>
#include <string>
#include <vector>

#include "arg_parser.h"


bool Arg_parser::parse_long_option( const char * const opt, const char * const arg,
                                    const Option options[], int & argind ) throw()
  {
  unsigned int len;
  int index = -1;
  bool exact = false, ambig = false;

  for( len = 0; opt[len+2] && opt[len+2] != '='; ++len ) ;

  // Test all long options for either exact match or abbreviated matches.
  for( int i = 0; options[i].code != 0; ++i )
    if( options[i].name && !std::strncmp( options[i].name, &opt[2], len ) )
      {
      if( std::strlen( options[i].name ) == len )	// Exact match found
        { index = i; exact = true; break; }
      else if( index < 0 ) index = i;		// First nonexact match found
      else if( options[index].code != options[i].code ||
               options[index].has_arg != options[i].has_arg )
        ambig = true;			// Second or later nonexact match found
      }

  if( ambig && !exact )
    {
    _error = "option `"; _error += opt; _error += "' is ambiguous";
    return false;
    }

  if( index < 0 )		// nothing found
    {
    _error = "unrecognized option `"; _error += opt; _error += '\'';
    return false;
    }

  ++argind;
  data.push_back( Record( options[index].code ) );

  if( opt[len+2] )		// `--<long_option>=<argument>' syntax
    {
    if( options[index].has_arg == no )
      {
      _error = "option `--"; _error += options[index].name;
      _error += "' doesn't allow an argument";
      return false;
      }
    if( options[index].has_arg == yes && !opt[len+3] )
      {
      _error = "option `--"; _error += options[index].name;
      _error += "' requires an argument";
      return false;
      }
    data.back().argument = &opt[len+3];
    return true;
    }

  if( options[index].has_arg == yes )
    {
    if( !arg )
      {
      _error = "option `--"; _error += options[index].name;
      _error += "' requires an argument";
      return false;
      }
    ++argind; data.back().argument = arg;
    return true;
    }

  return true;
  }


bool Arg_parser::parse_short_option( const char * const opt, const char * const arg,
                                     const Option options[], int & argind ) throw()
  {
  int cind = 1;			// character index in opt

  while( cind > 0 )
    {
    int index = -1;
    const unsigned char code = opt[cind];

    if( code != 0 )
      for( int i = 0; options[i].code; ++i )
        if( code == options[i].code )
          { index = i; break; }

    if( index < 0 )
      {
      _error = "invalid option -- "; _error += code;
      return false;
      }

    data.push_back( Record( code ) );
    if( opt[++cind] == 0 ) { ++argind; cind = 0; }	// opt finished

    if( options[index].has_arg != no && cind > 0 && opt[cind] )
      {
      data.back().argument = &opt[cind]; ++argind; cind = 0;
      }
    else if( options[index].has_arg == yes )
      {
      if( !arg || !arg[0] )
        {
        _error = "option requires an argument -- "; _error += code;
        return false;
        }
      data.back().argument = arg; ++argind; cind = 0;
      }
    }
  return true;
  }


Arg_parser::Arg_parser( const int argc, const char * const argv[],
                        const Option options[], const bool in_order ) throw()
  {
  if( argc < 2 || !argv || !options ) return;

  std::vector< std::string > non_options;	// skipped non-options
  int argind = 1;				// index in argv

  while( argind < argc )
    {
    const unsigned char ch1 = argv[argind][0];
    const unsigned char ch2 = ( ch1 ? argv[argind][1] : 0 );

    if( ch1 == '-' && ch2 )		// we found an option
      {
      const char * const opt = argv[argind];
      const char * const arg = (argind + 1 < argc) ? argv[argind+1] : 0;
      if( ch2 == '-' )
        {
        if( !argv[argind][2] ) { ++argind; break; }	// we found "--"
        else if( !parse_long_option( opt, arg, options, argind ) ) break;
        }
      else if( !parse_short_option( opt, arg, options, argind ) ) break;
      }
    else
      {
      if( !in_order ) non_options.push_back( argv[argind++] );
      else { data.push_back( Record() ); data.back().argument = argv[argind++]; }
      }
    }
  if( _error.size() ) data.clear();
  else
    {
    for( unsigned int i = 0; i < non_options.size(); ++i )
      { data.push_back( Record() ); data.back().argument.swap( non_options[i] ); }
    while( argind < argc )
      { data.push_back( Record() ); data.back().argument = argv[argind++]; }
    }
  }


Arg_parser::Arg_parser( const char * const opt, const char * const arg,
                        const Option options[] ) throw()
  {
  if( !opt || !opt[0] || !options ) return;

  if( opt[0] == '-' && opt[1] )		// we found an option
    {
    int argind = 1;			// dummy
    if( opt[1] == '-' )
      { if( opt[2] ) parse_long_option( opt, arg, options, argind ); }
    else
      parse_short_option( opt, arg, options, argind );
    if( _error.size() ) data.clear();
    }
  else { data.push_back( Record() ); data.back().argument = opt; }
  }
