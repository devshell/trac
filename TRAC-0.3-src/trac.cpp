///////////////////////////////////////////////////////////////////////////////
/*
*
*	Copyright (C) 2007  Roman Goldmann
*
*	 roman.goldmann@devshell.org
*	 http://www.devshell.org
*
*	This program is free software; you can redistribute it and/or
*	modify it under the terms of the GNU General Public License
*	as published by the Free Software Foundation; either version 2
*	of the License, or (at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program; if not, write to the Free Software
*	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*
*
*
*/
///////////////////////////////////////////////////////////////////////////////

#include "csvlib.h"
#include <windows.h>   // WinApi header

using namespace std;

/******************************************************************************
*					
*				START		Function Declarations
*
*				centerchar		-	takes text and centers it on screen using 
*									char type as input
*
*				centerstring	-	takes text and centers it on screen using 
*									string type as input
*
*				progress		-	takes what position starting at, what 
*									current position is, and what last 
*									position is in program
*
*				progressbar		-	takes data from progress function and 
*									outputs onto screen
*
*				ctxt			-	takes a number and sets output text colour
*									1 = green	2 = red		3 = yellow 
*									4 = blue	0 = white
*
*******************************************************************************/

void centerchar(char* s);
void centerstring(string s);
void progress(int start, int current, int total);
void progressbar(float current, float total);
void ctxt(int c);


/******************************************************************************
*					
*			END			Function Declarations
*
*******************************************************************************/


/*******************************************************************************
*
*			START		Struct Container
*			
*			This creates a structure with a name and column for the purpose
*			of comparison between the data in the excel file for row titles and
*			the fields in the taxr.layout file
*
********************************************************************************/
struct file
{
	string name;
	int col;
};

/*******************************************************************************
*
*			END			Struct Container
*
********************************************************************************/

int main()
{
	
	string temp_line(84, ' ');	//	this will be used by the program to know
								//	where the limits of the text-file are

	/***************************************************************************
	*
	*		START		Class Initialization
	*
	*	This section creates two csvfiles and sets the layout to
	*	'|' as the separator. The csv one is then initialized with
	*	the default separator which is a comma. Finally, two streams
	*	are created: one for the print output file and one for the
	*	error log for the main() function
	*
	****************************************************************************/

	csvfile layout("taxr.layout", "logs\\taxr_layout.log", '|');
	csvfile csv("taxr.csv", "logs\\taxr_csv.log");

	ofstream fout("taxr.print", ios::trunc);

	ofstream log("logs\\taxr.log", ios::trunc);

	/***************************************************************************
	*
	*		END		Class Initialization
	*
	****************************************************************************/

	int total=0;	
	int start=0;
	int strpos=0;

	int col=0;
	int row=1;
	int lout=0;

	bool say = false;

	string tmpstr;
	int tmpstrsize = 0;

//	LICENSE
	ctxt(3);
	cout << endl;
	centerchar("LICENSE");
	cout << endl << endl << endl;
	centerchar("TRAC (Tax Receipt Aggregation Console), Copyright (C) 2007 Roman Goldmann");
	cout << endl << endl << endl;
	centerchar("TRAC comes with ABSOLUTELY NO WARRANTY; for details");
	cout << endl;
	centerchar("read the README.  This is free software, and you are welcome");
	cout << endl;
	centerchar("to redistribute it under certain conditions; read the README.txt");
	cout << endl;
	centerchar("file that came with this software in the root directory for details.");
	cout << endl;
	cout << endl;
	cout << endl;
	for(int ch=0; ch < 80; ch++) cout << "-";
	cout << endl << endl;
	ctxt(0);

//  END LICENSE

//	INSTRUCTIONS
	ctxt(2);
	cout << "\n\n";
	centerchar("To quit simply close this window");
	cout << "\n\n";
	ctxt(0);

//	END INSTRUCTIONS


/*	
*	Get column data -- what data is where
*	This pice of code creates an pointer to an array of structures that is of a size that is set
*	by getting the number of columns from the csv file using the get_num_of_col method
*	which returns an integer that is then used for the creation of the pointer to an
*	array of structures
*/

	int arsize = csv.get_num_of_col(1);
	file* lfile = new file[arsize+1];


//-----------------------------------------------------------------------------------------------
log << "0.1 - arsize = " << arsize << endl;
//-----------------------------------------------------------------------------------------------

/************************************************************************************
*	Here we have a for loop that uses the input array size that we got above to
*	go through each column
************************************************************************************/
	for(int cl=1; cl <= arsize; cl++)
	{


//-----------------------------------------------------------------------------------------------
log << "0.2 - csv.get_str(cl,1) = " << csv.get_str(cl, 1) << endl;
//-----------------------------------------------------------------------------------------------


/************************************************************************************
*	Here we take the data from the columns such as which column number it is
*	and what is the string stored in the column. It then takes this data and
*	stores it in the structure we created at the beginning.
************************************************************************************/
		lfile[cl].name = csv.get_str(cl,1);
		lfile[cl].col = cl;
	}

		

// user configuration input.
	ctxt(1);
	cout << "Enter total receipts to print\n:::> ";
	cin >> total;
	
	cout << "\n\nEnter which row to start at\n:::> ";
	cin >> start;
	ctxt(0);
	int csv_row = start;


//-----------------------------------------------------------------------------------------------
log << "0 - tmpstr size = " << tmpstrsize << endl;
log << "0 - tmpstr length = " << tmpstr.length() << endl;
//-----------------------------------------------------------------------------------------------

/************************************************************************************
*	Here we create another for loop which uses the data we obtained from the
*	user and stored in csv_rows and total to go through the csv file row-by-row
*	and column-by-column. It then compares the data in each column in each row
*	to the data contained in the structure we created and if there is a match
*	the data in that column in the csv file is replaced with the placeholder
*	in the taxr.layout file and prints the output to the taxr.print file.
************************************************************************************/
	for(csv_row; csv_row < total + start; csv_row++)
	{


//-----------------------------------------------------------------------------------------------
log << "\n\n\n\n\n\n\n\n\n\n\n		START		" << endl;
log << "csv_row = " << csv_row << endl;
log << "total + csv_row = " << total + csv_row << endl;
//-----------------------------------------------------------------------------------------------


if (csv_row < (csv_row + total)){ say = true; }


//-----------------------------------------------------------------------------------------------
log << "csv_row < total + csv_row? = " << say << endl << endl;
//-----------------------------------------------------------------------------------------------


		col=0;
		row=1;
		lout=0;

//	Here we start the progress bar so we can show the user how much time is left
//	until the program is finished.
		progress(start, csv_row, total);


//	Here we create a for loop and have it continue until the end of the file is reached
//	which occurs when the get_str function returns a string equal to "STOP". This for loop
//	moves the program through the rows one-by-one.
		for(row; layout.get_str(1, row, false) != "STOP"; row++)
		{


//-----------------------------------------------------------------------------------------------
log << "1 - row++" << endl;
//-----------------------------------------------------------------------------------------------

//	Here we create a for loop that does the same as above except to move us through the
//	columns. The loop terminates when it reaches a "||" in the taxr.layout file, which
//	tells the loop that it has reached the end of that row in the taxr.layout file
//	(reached an empty cell forces the method to output a string equal to "END".
			for(col=0; layout.get_str(col, row, false) != "END"; col++)
			{


//-----------------------------------------------------------------------------------------------
log << "2 - col++" << endl;
log << "2.1 - layout.get_str(col, row, false) = start-|" << layout.get_str(col, row, false) << "|-end" << endl;
log << "col = " << col << endl;
log << "row = " << row << endl;
//-----------------------------------------------------------------------------------------------

//	Another for loop, this time it compares each string in the taxr.layout file to that of
//	each string we stored in the structure we created at the beginning. Upon finding a match
//	the program replaces that string in the taxr.layout file with that of the string in the csv
//	file and outputs to the taxr.print file.
				for(lout=0; layout.get_str(col, row, false) != lfile[lout].name; lout++)
				{


//-----------------------------------------------------------------------------------------------
log << "lout = " << lout << endl;
//-----------------------------------------------------------------------------------------------

//	while we have not gone through all the data stored in the structure the loop will continue
					if(lout >= arsize){break;}
				}
//	if a match is found the for loop ends and the value contained in the "lout" variable
//	is used to replace the data in the taxr.layout file and output to the taxr.print file
				if(layout.get_str(col, row, false) == lfile[lout].name)
				{


//-----------------------------------------------------------------------------------------------
log << "3 - match found - replacing" << endl;
log << "0 - tmpstr size = " << tmpstrsize << endl;
log << "0 - tmpstr length = " << tmpstr.length() << endl;
//-----------------------------------------------------------------------------------------------


					strpos=0;


//-----------------------------------------------------------------------------------------------
log << "3.1 - tmpstr before erase = start-|" << tmpstr << "|-end" << endl;
//-----------------------------------------------------------------------------------------------

//	check to see if our temporary string is empty and if it is not, empty it.
//	note that I would have used tmpstr.clear() but for some reason by compiler
//	(Visual Studio 6) would not recognize that method as being part of the string class.
//	Otherwise, I would have used clear().
					if(!tmpstr.empty()){ tmpstr.erase(0, tmpstr.length()); }


//-----------------------------------------------------------------------------------------------
log << "3.2 - tmpstr after erase = start-|" << tmpstr << "|-end" << endl;
log << "0 - tmpstr size = " << tmpstrsize << endl;
log << "0 - tmpstr length = " << tmpstr.length() << endl;
//-----------------------------------------------------------------------------------------------

//	Set the data to tmpstr in the location it is found in the taxr.layout file.
					tmpstr = layout.get_lcell(col, row, strpos, false);


//-----------------------------------------------------------------------------------------------
log << "5.3 - tmpstr = start-|" << tmpstr << "|-end" << endl;
log << "3.3 - About to replace" << endl;
log << "3.4 - lout = " << lout << endl;
log << "3.5 - strpos = " << strpos << endl;
log << "3.6 - tmpstr.length()-1 = " << tmpstr.length()-1 << endl;
log << "3.7 - strpos = " << strpos << endl;
log << "3.8 - csv_row = " << csv_row << endl;
log << "3.9 - lfile[lout].col = " << lfile[lout].col << endl;
log << "3.10 - csv.get_str(lfile[lout].col, csv_row) = " << csv.get_str(lfile[lout].col, csv_row) << endl;
log << "4.1 - temp_line =" << endl << "start|" << temp_line << "|end" << endl;
//-----------------------------------------------------------------------------------------------

//	We now set that data to the temp_line and continue on to the next item in the
//	taxr.layout file until we reach the end of that row.
					temp_line.replace(strpos, tmpstr.length(), csv.get_str(lfile[lout].col, csv_row));


//-----------------------------------------------------------------------------------------------
log << "4.2 - temp_line =" << endl << "start|" << temp_line << "|end" << endl;
//-----------------------------------------------------------------------------------------------

//	To ensure the temp_line does not get longer than the area that is printable 
//	in the taxr.layout file the program erases the end of the temp_line until it
//	is back to its maximum size.
					while(temp_line.length() > 84)
					{
						temp_line.erase(temp_line.length()-1, 1);
					}


//-----------------------------------------------------------------------------------------------
log << "4.3 - temp_line length() =" << temp_line.length() << endl;
log << "0 - tmpstr size = " << tmpstrsize << endl;
log << "0 - tmpstr length = " << tmpstr.length() << endl;
//-----------------------------------------------------------------------------------------------


				}
//	If the column number we are in is equal to zero we simply copy the data
//	from the taxr.layout file and put it in the temp_line same as above.
				else if(!col)
				{


//-----------------------------------------------------------------------------------------------
log << "5 - match NOT found - replacing with spaces from layout file" << endl;
//-----------------------------------------------------------------------------------------------


					strpos=0;


//-----------------------------------------------------------------------------------------------
log << "5.1 - tmpstr before erase = start-|" << tmpstr << "|-end" << endl;
log << "0 - tmpstr size = " << tmpstrsize << endl;
log << "0 - tmpstr length = " << tmpstr.length() << endl;
//-----------------------------------------------------------------------------------------------


					if(!tmpstr.empty()){ tmpstr.erase(0, tmpstr.length()-1); }


//-----------------------------------------------------------------------------------------------
log << "5.2 - tmpstr after erase = start-|" << tmpstr << "|-end" << endl;
//-----------------------------------------------------------------------------------------------


					tmpstr = layout.get_lcell(col, row, strpos, false);


//-----------------------------------------------------------------------------------------------
log << "5.3 - tmpstr = start-|" << tmpstr << "|-end" << endl;
log << "0 - tmpstr size = " << tmpstrsize << endl;
log << "0 - tmpstr length = " << tmpstr.length() << endl;
//-----------------------------------------------------------------------------------------------


					temp_line.replace(strpos, tmpstr.length(), tmpstr);
					

//-----------------------------------------------------------------------------------------------
log << "6 - temp_line =" << endl << "start|" << temp_line << "|end" << endl;
//-----------------------------------------------------------------------------------------------


					while(temp_line.length() > 84)
					{
						temp_line.erase(temp_line.length()-1, 1);
					}


//-----------------------------------------------------------------------------------------------
log << "6.1 - temp_line length() =" << temp_line.length() << endl;
log << "0 - tmpstr size = " << tmpstrsize << endl;
log << "0 - tmpstr length = " << tmpstr.length() << endl;
//-----------------------------------------------------------------------------------------------



				}

				
			}


//-----------------------------------------------------------------------------------------------
log << "7.1 - PUTTING IN FILE " << endl;
//-----------------------------------------------------------------------------------------------

//	The program has exited the column for loop as it has reached the end of the row.
//	the program now outputs the contents in temp_line into the file taxr.print.
			fout << temp_line << endl;


//-----------------------------------------------------------------------------------------------
log << "7.2 - temp_line after FOUT =" << endl << "start-|" << temp_line << "|-end" << endl;
//-----------------------------------------------------------------------------------------------

//	Finally the program checks to see if temp_line is empty and if it is not it
//	clears the contents of it for the next loop. See above about why I used the
//	erase() method of the string class rather than the much simpler clear() method.
			if(!temp_line.empty()){ temp_line.erase(0, temp_line.length()-1); temp_line.resize(84, ' '); }


//-----------------------------------------------------------------------------------------------
log << "7.3 - temp_line after erase =" << endl << "start-|" << temp_line << "|-end" << endl;
//-----------------------------------------------------------------------------------------------


			col=0;


//-----------------------------------------------------------------------------------------------
log << " - - - - E N D  OF  ROW - - - -" << endl;
//-----------------------------------------------------------------------------------------------


		}
	}

//-----------------------------------------------------------------------------------------------
log << "\n\n			DONE		DONE		DONE";	
//-----------------------------------------------------------------------------------------------

//	Free the memory used by the pointer to the array of structures we created earlier.
	delete [] lfile;
	return 0;
}




void centerchar(char* s)
{
  int l=strlen(s);
  int pos=(int)((80-l)/2);
  for(int i=0;i<pos;i++)
   cout<<" ";

  cout<<s;
}

void centerstring(string s)
{
  int l = s.length();
  int pos=(int)((80-l)/2);
  for(int i = 0; i < pos; i++) cout << " ";

  cout << s;
}


void progress(int start, int current, int total)
{
	system("cls");

	cout << "\n\n\n";
	ctxt(1);
	centerstring("P R O G R E S S");
	ctxt(0);
	cout << "\n\n\n\n\n\n";

	int cur = current - start;
	int tot = total;

	progressbar((float)cur, (float)tot);
	cout << endl << "\n\n\n\n\n\n\n\n";

}


void progressbar(float current, float total)
{
	string l(1, '[');
	string b(1, '|');
	string r(1, ']');

	float tmp = ((current+1) / (total)) * 48;
	int pos = (int)tmp;

	string progressbar(50, ' ');
	progressbar.replace(0, l.length(), l);


	for(int i = 1; i <= (int)pos; i++) progressbar.replace(i, b.length(), b);
					
	while(progressbar.length() > 50)
	{
		progressbar.erase(progressbar.length()-1, 1);
	}

	progressbar.replace(progressbar.length()-1, r.length(), r);

	while(progressbar.length() > 50)
	{
		progressbar.erase(progressbar.length()-1, 1);
	}
	ctxt(1);
	centerchar("Percent Complete: ");
	ctxt(3);
	cout << ((current + 1) / (total)) * 100 << "%" << endl << endl;
	
	ctxt(4);
	centerstring(progressbar);
	ctxt(0);

	if((((current + 1) / (total)) * 100) == 100)
	{
		cout << "\n\n\n\n";
		ctxt(2);
		centerchar("D O N E");
		ctxt(0);
	}
}
void ctxt(int c)
{
	HANDLE hConsole;
	int k = 15;

	switch(c)
	{
	case 1:	//	green
		k = 10;
		break;
	case 2:	//	red
		k = 12;
		break;
	case 3:	//	yellow
		k = 14;
		break;
	case 4:	//	blue
		k = 11;
		break;
	default:	//	white
		int k = 15;
		break;
	}


	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, k);
}