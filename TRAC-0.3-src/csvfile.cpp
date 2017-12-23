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
#include "csvfile.h"
using namespace std;



/////////////////////////////////////////////////////////////////////////
csvfile::csvfile(char file[], char logf[], char s)
{ 	
	sep = s;
	temp_ch_count = 0;
	xls_fin.open(file);
	log.open(logf, ios::trunc);
	if(!(xls_fin.is_open() && log.is_open()))
	{ 
		cout << "Error opening file or log,";
		cout << "\nmake sure the right file name has been entered";
		cout << "\notherwise, check correct path is used for the file";
		cout << "File = " << file << endl;
		cout << "Log = " << log << endl;
	}
}

/////////////////////////////////////////////////////////////////////////
csvfile::~csvfile()
{
	xls_fin.close();
	this->e_ch_buffer();
}

/////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////
void csvfile::e_ch_buffer()
{
	while(temp_ch_count)
	{
		delete [] temp_ch;
		temp_ch_count--;
	}
	temp_ch_count++;
}

/////////////////////////////////////////////////////////////////////////
int csvfile::m_atoi(char *s)
{
  int sign=0;
  int v=0;
  if (*s == '-') { s++; sign++; }
  for (;;)
  {
    int c=*s++ - '0';
    if (c < 0 || c > 9) break;
    v*=10;
    v+=c;
  }
  if (sign) return -(int) v;
  return (int)v;
}

/////////////////////////////////////////////////////////////////////////
string csvfile::get_str(int col, int row, bool csv)
{

log << "\n\n\n\n\n	NOW		IN		GET_STR\n\n";


// set private data
	pos_fr = 0;

	if(csv){ colcnt = 1; }
	else { colcnt = 0; }
	i = 0;
	pos = 1;

// clear file io stream and go to beginning of file
	xls_fin.clear();
	xls_fin.seekg(0);

// local data
	string col_data;
	string xls_data;

log << "col = " << col << endl;
log << "row = " << row << endl;

	for(pos; getline(xls_fin, xls_data); pos++)
	{
log << "xls_data.length() = " << xls_data.length() << endl;

		if(pos == row)
		{
log << "In if(pos==row) LOOP" << endl;

			if(col==0)
			{
log << "	col == 0" << endl;
				for(i = 0; (!(colcnt > col)) && (i < xls_data.length()); i++)
				{
					if((xls_data [i] == sep))
					{	 
						if(col_data.length() < 1) 
						{ 
							col_data.erase(0, col_data.length()); 
							col_data = "END"; 
						}
log << "		col_data start|" << col_data << "|end" << endl;
log << "		DONE - str() for col = 0" << endl;
log << "		col_data length() = " << col_data.length() << endl;
						return col_data; 
					}
					col_data += xls_data [i];
				}
			}


			// find row in question by looking for the comma #
			for(i; (colcnt != col) && (i < xls_data.length()); i++)
			{
log << "pos != 0, Now looking for column" << endl;
				if(xls_data [i] == sep)
				{ 
					pos_fr = i; 
					colcnt++;
log << "	column found = " << colcnt << endl;
log << "	pos_fr = " << pos_fr << endl;
log << "	i = " << i << endl;
				}
			}

// once in column, put string up to next comma in colstring
			for(i; !(colcnt > col) && (i < xls_data.length()); i++)
			{
log << "In column, " << colcnt << " copying data from cell" << endl;
				if(xls_data [i] == sep)
				{	 
					if(col_data.length() < 1) 
					{ 
						col_data.erase(0, col_data.length()); 
						col_data = "END"; 
					}

log << "	DONE - str() for col > 0" << endl;
log << "	col_data length() = " << col_data.length() << endl;
log << "	col_data = start-|" << col_data << "|-end" << endl << endl << endl;
					return col_data; 
				}
				col_data += xls_data [i];
			}
			if(i == xls_data.length()) return col_data;
		}

log << "xls_data length() before erase = " << xls_data.length() << endl;

		xls_data.erase(0, xls_data.length());

log << "xls_data length() after erase = " << xls_data.length() << endl;

	}

	cerr << "An error occured in cvsfile() class in member function get_str\n";
	return col_data;
}




////////////////////////////////////////////////////////////////////
string csvfile::get_lcell(int col, int row, int& pstart, bool csv)
{

log << "\n\n\n\n\n	NOW		IN		GET_LCELL\n\n";

// set private data
	pos_fr = 0;
	bool pst = false;
	if(csv){ colcnt = 1; }
	else { colcnt = 0; }
	i = 0;
	pos = 1;

// clear file io stream and go to beginning of file
	xls_fin.clear();
	xls_fin.seekg(0);

// local data
	string col_data;
	string xls_data;
	string eol = "END";

log << "col = " << col << endl;
log << "row = " << row << endl;

	for(pos; getline(xls_fin, xls_data); pos++)
	{

log << "xls_data.length() = " << xls_data.length() << endl;

		if(pos == row)
		{
log << "In if(pos==row) LOOP" << endl;

			if(col==0)
			{
log << "	col == 0" << endl;
				for(i; (!(colcnt > col)) && (i <= xls_data.length()); i++)
				{
					if(xls_data [i] == sep)
					{	 
						if(col_data.length() < 1) 
						{ 
							col_data.erase(0, col_data.length()); 
							col_data = "END"; 
						}
log << "		col_data start|" << col_data << "|end" << endl;
log << "		DONE - str() for col = 0" << endl;
log << "		col_data length() = " << col_data.length() << endl;
						return col_data; 
					}
					col_data += xls_data [i];
				}
			}


			// find row in question by looking for the comma #
			for(i; (colcnt != col) && (i <= xls_data.length()); i++)
			{
log << "pos != 0, Now looking for column" << endl;
				if(xls_data [i] == sep)
				{ 
					pos_fr = i; 
					colcnt++;
log << "	column found = " << colcnt << endl;
log << "	pos_fr = " << pos_fr << endl;
log << "	i = " << i << endl;
				}
			}


// once in column, put string up to next comma in colstring			
			for(i; (!(colcnt > col)) && (i <= xls_data.length()); i++)
			{
// once in column, set pstart to its starting position
				if(!pst){ pstart = i; pst = true; }
log << "pstart (txtpos) = " << i << endl;
log << "In column, " << colcnt << " copying data from cell" << endl;
				if(xls_data [i] == sep)
				{	 
					if(col_data.length() < 1) 
						{ 

log << "col_data.length() < 1 = TRUE" << endl;

							col_data.erase(0, col_data.length()); 
							col_data = "END"; 
						}
log << "		col_data start|" << col_data << "|end" << endl;
log << "		DONE - str() for col = 0" << endl;
log << "		col_data length() = " << col_data.length() << endl;
					return col_data; 
				}
				col_data += xls_data [i];
			}
			if(i == xls_data.length()) return col_data;
		}

log << "xls_data length() before erase = " << xls_data.length() << endl;

		xls_data.erase(0, xls_data.length());

log << "xls_data length() after erase = " << xls_data.length() << endl;
	}

	cerr << "An error occured in cvsfile class in member function get_lcell()\n";
	return col_data;
}

/////////////////////////////////////////////////////////////////////////
int csvfile::get_num_of_col(int row)
{

log << "\n\n\n\n\n	NOW		IN		GET_NUM_OF_COL\n\n";


// set private data
	pos_fr = 0;

	colcnt = 1;
	i = 0;
	pos = 1;

// clear file io stream and go to beginning of file
	xls_fin.clear();
	xls_fin.seekg(0);

// local data
	string xls_data;

log << "row = " << row << endl;

	for(pos; getline(xls_fin, xls_data); pos++)
	{
log << "xls_data.length() = " << xls_data.length() << endl;

		if(pos == row)
		{
log << "In if(pos==row) LOOP" << endl;


			// find row in question by looking for the comma #
			for(i; i < xls_data.length(); i++)
			{
log << "pos != 0, Now looking for column" << endl;
				if(xls_data [i] == sep)
				{  
					colcnt++;
log << "	column found = " << colcnt << endl;
log << "	pos_fr = " << pos_fr << endl;
log << "	i = " << i << endl;
				}
			}
			
			return colcnt;
		}
	}


	cerr << "An error occured in cvsfile() class in member function get_num_of_col\n";
	return colcnt;
}


////////////////////////////////////////////////////////////////////
char* csvfile::get_char(int col, int row)
{
	this->e_ch_buffer();
	colstring = this->get_str(col,row);
	len = colstring.size();
	temp_ch = new char [len + 1];
	temp_ch[len] = NULL;
	strncpy(temp_ch, colstring.c_str(), colstring.size());

	return temp_ch;
}

////////////////////////////////////////////////////////////////////
int csvfile::get_int(int col, int row)
{
	this->e_ch_buffer();
	temp_ch = new char [len + 1];
	temp_ch = this->get_char(col,row);
	return this->m_atoi(temp_ch);
}



