///////////////////////////////////////////////////////////////////////////////
/*

Copyright (C) 2007  Roman Goldmann

  roman.goldmann@devshell.org
  http://www.devshell.org

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.



*/
///////////////////////////////////////////////////////////////////////////////

// static members in classes
#ifndef _CSVFILE_H_
#define _CSVFILE_H_

#include "headers.h"

using namespace std;


class csvfile 
{
private:

//int
	int len;
	int pos_fr;
	int colcnt;
	int i;
	int n;
	int pos;
	int temp_ch_count;			// for memory management, deletes all temp_ch created

// char
	char sep;

// char pointers
	char* temp_ch;				// used in get_char() as temporary storage for strcpy()

// string
	string in_from_xls;			// used in get_str() as input from file from getline()
	string colstring;			// used as output for colstring

// file io stream
	fstream xls_fin;
	ofstream log;

// private methods
//	const char* get_cchar(int col=1, int row=1);


public:
	csvfile(char file[], char logf[], char s = ',');
	~csvfile();
	
	void e_ch_buffer();	
	int m_atoi(char* s);

	string get_lcell(int col, int row, int& pstart, bool csv=true);
	string get_str(int col=1, int row=1, bool csv=true);
	int get_num_of_col(int row=1);
	char* get_char(int col=1, int row=1);
	int get_int(int col=1, int row=1);

};

#endif