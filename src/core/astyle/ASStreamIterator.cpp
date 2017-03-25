
#include "astyle/ASStreamIterator.h"

namespace astyle
{


ASStreamIterator::ASStreamIterator(istream *in)
{
	inStream = in;
	buffer.reserve(200);
	eolWindows = eolLinux = eolMacOld = 0;
	peekStart = 0;
	prevLineDeleted = false;
	checkForEmptyLine = false;
}


ASStreamIterator::~ASStreamIterator()
{
}

// save the last input line after input has reached EOF

void ASStreamIterator::saveLastInputLine()
{
	assert(inStream->eof());
	prevBuffer = buffer;
}

/**
 * read the input stream, delete any end of line characters,
 *     and build a string that contains the input line.
 *
 * @return        string containing the next input line minus any end of line characters
 */
//template<typename T>
string ASStreamIterator::nextLine(bool emptyLineWasDeleted)
{
	// verify that the current position is correct
	assert (peekStart == 0);

	// a deleted line may be replaced if break-blocks is requested
	// this sets up the compare to check for a replaced empty line
	if (prevLineDeleted)
	{
		prevLineDeleted = false;
		checkForEmptyLine = true;
	}
	if (!emptyLineWasDeleted)
		prevBuffer = buffer;
	else
		prevLineDeleted = true;

	// read the next record
	buffer.clear();
	char ch;
	inStream->get(ch);

	while (!inStream->eof() && ch != '\n' && ch != '\r')
	{
		buffer.append(1, ch);
		inStream->get(ch);
	}

	if (inStream->eof())
	{
		return buffer;
	}

	int peekCh = inStream->peek();

	// find input end-of-line characters
	if (!inStream->eof())
	{
		if (ch == '\r')         // CR+LF is windows otherwise Mac OS 9
		{
			if (peekCh == '\n')
			{
				inStream->get(ch);
				eolWindows++;
			}
			else
				eolMacOld++;
		}
		else                    // LF is Linux, allow for improbable LF/CR
		{
			if (peekCh == '\r')
			{
				inStream->get(ch);
				eolWindows++;
			}
			else
				eolLinux++;
		}
	}
	else
	{
		inStream->clear();
	}

	// set output end of line characters
	if (eolWindows >= eolLinux)
	{
		if (eolWindows >= eolMacOld)
			strcpy(outputEOL, "\r\n");  // Windows (CR+LF)
		else
			strcpy(outputEOL, "\r");    // MacOld (CR)
	}
	else if (eolLinux >= eolMacOld)
		strcpy(outputEOL, "\n");    // Linux (LF)
	else
		strcpy(outputEOL, "\r");    // MacOld (CR)

	return buffer;
}

// save the current position and get the next line
// this can be called for multiple reads
// when finished peeking you MUST call peekReset()
// call this function from ASFormatter ONLY
//template<typename T>
string ASStreamIterator::peekNextLine()
{
	assert (hasMoreLines());
	string nextLine;
	char ch;

	if (peekStart == 0)
		peekStart = inStream->tellg();

	// read the next record
	inStream->get(ch);
	while (!inStream->eof() && ch != '\n' && ch != '\r')
	{
		nextLine.append(1, ch);
		inStream->get(ch);
	}

	if (inStream->eof())
	{
		return nextLine;
	}

	int peekCh = inStream->peek();

	// remove end-of-line characters
	if (!inStream->eof())
	{
		if ((peekCh == '\n' || peekCh == '\r') && peekCh != ch)  /////////////  changed  //////////
			inStream->get(ch);
	}

	return nextLine;
}

// reset current position and EOF for peekNextLine()
//template<typename T>
void ASStreamIterator::peekReset()
{
	assert(peekStart != 0);
	inStream->clear();
	inStream->seekg(peekStart);
	peekStart = 0;
}

}
