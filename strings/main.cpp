// String searching lab exercise.
// Adam Sampson <a.sampson@abertay.ac.uk>

#include <cassert>
#include <iostream>
#include <list>
#include <string>

#include "utils.h"

using std::cout;
using std::endl;
using std::list;
using std::string;

/** Return first position of pat in text, or -1 if not found. */
Position find_bruteforce(const string& pat, const string& text)
{
	Position pat_len = pat.size();
	Position text_len = text.size();

	// Advance along the text.
	for (Position i = 0; i < text_len - pat_len; ++i)
	{
		show_context(text, i);

		Position j;

		// Advance along the pattern.
		for (j = 0; j < pat_len; j++)
		{
			show_context(pat, j);

			// Check each letter of the pattern against the text.
			if (text[i + j] != pat[j])
			{
				break; // Doesn't match here.
			}
		}

		// This is why the Position j variable is outside the condition.
		// If it were inside the for loops condition, the j variable would die
		// at the end of the for loop code block.
		if (j == pat_len)
		{
			return i; // Matched here.
		}
	}

	return -1; // Not found.
}

/** Return first position of pat in text, or -1 if not found. */
Position find_skipping(const string& pat, const string& text)
{
	Position pat_len = pat.size();
	Position text_len = text.size();

	bool in_pattern[256];

	// Populate the above bool array with deafult values 'false'
	for (int i = 0; i < 256; ++i)
	{
		in_pattern[i] = false;
	}

	// Loop over every char that is in the 'pat' string passed to this function
	// For every char we find we set the index value of that in the bool array
	// to true, this allows us to have a value associated with each letter as per
	// its ASCII table value.
	for (char c : pat)
	{
		in_pattern[int(c)] = true;
	}

	// Advance along the text.
	for (Position i = 0; i < text_len - pat_len; ++i)
	{
		show_context(text, i);

		/* Get the char at the last pos in the pattern, cast that to its int (ASCII)
		 * corrosponding value, subscript the in_pattern array with that value
		 * which remember is the ASCII value of the last letter in the pattern
		 * If the condition returns true then we have found a match, as the element would
		 * have been set true during the second for loop above where we set to true
		 * only the elements of the corrosponding ASCII values in the pattern.
		 */
		if (!in_pattern[int(text[(i + pat_len) - 1])])
		{
			i += pat_len - 1;	// Skip forward the length of the word.
			continue;
		}

		Position j;

		// Advance along the pattern.
		for (j = 0; j < pat_len; j++)
		{
			show_context(pat, j);

			// Check each letter of the pattern against the text.
			if (text[i + j] != pat[j])
			{
				break; // Doesn't match here.
			}
		}

		// This is why the Position j variable is outside the condition.
		// If it were inside the for loops condition, the j variable would die
		// at the end of the for loop code block.
		if (j == pat_len)
		{
			return i; // Matched here.
		}
	}

	return -1; // Not found.
}

/** Return first position of pat in text, or -1 if not found. */
Position find_boyer_moore(const string& pat, const string& text)
{
	Position pat_len = pat.size();
	Position text_len = text.size();

	// Create a int array, default values = 0;
	int skip[256];

	// Populate the above int array with deafult value 'pat_len'
	for (int i = 0; i < 256; ++i)
	{
		skip[i] = pat_len;
	}

	/*
	* Cast the value returned from the pattern at index 'i' to its ASCII equivalent
	* Use that value to subscript the skip array, setting the value at that index position
	* to pattern length - the value of 'i'. i.e it's index position.
	* This way we have a system which allows us a way of being able to know how many
	* letters we can skip. i.e. how many chars from the end of the pattern.
	* 
	*				[0][1] [2] [3] [4] [5]
	* e.g. Pattern: "D	u	n	d	e	e"
	* int(pat[0]) = 68 - The ASCII value of capital  'D'
	* int(pat[1]) = 117 - The ASCII value of capital 'u'
	* int(pat[2]) = 110 - The ASCII value of capital 'n'
	* int(pat[3]) = 100 - The ASCII value of capital 'd'
	* int(pat[4]) = 101 - The ASCII value of capital 'e'
	* int(pat[5]) = 101 - The ASCII value of capital 'e'

	* skip[68] = (pat_len(6) - 1) - i(0)	->	skip[68] = (6 - 1) - 0	->	skip[68] = 5
	* skip[117] = (pat_len(6) - 1) - i(1)	->	skip[117] = (6 - 1) - 1	->	skip[117] = 4
	* skip[110] = (pat_len(6) - 1) - i(2)	->	skip[110] = (6 - 1) - 2	->	skip[110] = 3
	* skip[100] = (pat_len(6) - 1) - i(3)	->	skip[100] = (6 - 1) - 3	->	skip[100] = 2
	* skip[101] = (pat_len(6) - 1) - i(4)	->	skip[101] = (6 - 1) - 4	->	skip[101] = 1 ???
	* skip[101] = (pat_len(6) - 1) - i(5)	->	skip[101] = (6 - 1) - 5	->	skip[101] = 0 ???
	* 
	* Need this clarified for what happens in the case of repeated letters?
	* It doesn't matter that the value at the index is overwritten by a new amount which
	* is representative of its position relative to the end of the pattern counting backwards.
	* As this will be the new amount we can skip.
	*/
	for (Position i = 0; i < pat_len; ++i)
	{
		skip[int(pat[i])] = (pat_len - 1) - i;
	}

	// Advance along the text.
	for (Position i = 0; i < text_len - pat_len; ++i)
	{
		show_context(text, i);

		/*
		 * Get the char at the position in the text from the index value + pat_len - 1
		 * Subscript the skip array with that value
		 * Set that to the variable 's'
		 * 
		 *				[0] [1] [2] [3] [4] [5] [6] [7] [8] [9] [10] [11] [12] [13]	[14] [15]......
		 * e.g. Text	"U	 n	 i	 v	 e	 r	 s	 i	 t	 y		   o	f		  D	   u   n  d	 e	 e"
		 * 
		 * text[i + pat_len - 1]	->	text[0 + (6 - 1)]	-> text[5] = 'r'	-> ASCII 'r' = 114
		 * s = skip[114]	= 6 - DEFAULT VALUE
		 * text[i + pat_len - 1]	->	text[
		 */
		int s = skip[int(text[i + pat_len - 1])];

		/*
		 * If s != 0, this means that we have found a letter that is contained in the pattern somewhere.
		 * OR the DEFAULT value of 6. We then skip by this amount.
		 * Enter the code block and advance the loop index 'i' by and amount (s - 1).
		 * Skip over the rest of this iteration, proceeding to the iteration where we check everything again.
		 * 
		 * If s == 0 this means we have found the letter that corrosponds to the last letter of the pattern
		 * Which means we cannot skip any but must instead check over all the letters in the pattern against
		 * the text as in the normal skip algo function.
		 */
		if (s != 0)
		{
			i += (s - 1);	// Skip forwards by an amount maximum pat_len - 1
			continue;
		}

		Position j;

		// Advance along the pattern.
		for (j = 0; j < pat_len; j++)
		{
			show_context(pat, j);

			// Check each letter of the pattern against the text.
			if (text[i + j] != pat[j])
			{
				break; // Doesn't match here.
			}
		}

		// This is why the Position j variable is outside the condition.
		// If it were inside the for loops condition, the j variable would die
		// at the end of the for loop code block.
		if (j == pat_len)
		{
			return i; // Matched here.
		}
	}

	return -1; // Not found.
}

/** Return first position of pat in text, or -1 if not found. */
list<Position> find_boyer_moore_multi(const string& pat, const string& text)
{
	Position pat_len = pat.size();
	Position text_len = text.size();
	list<Position> matchedPositions;

	// Create a int array, default values = 0;
	int skip[256];

	// Populate the above int array with deafult value 'pat_len'
	for (int i = 0; i < 256; ++i)
	{
		skip[i] = pat_len;
	}

	/*
	* Cast the value returned from the pattern at index 'i' to its ASCII equivalent
	* Use that value to subscript the skip array, setting the value at that index position
	* to pattern length - the value of 'i'. i.e it's index position.
	* This way we have a system which allows us a way of being able to know how many
	* letters we can skip. i.e. how many chars from the end of the pattern.
	*
	*				[0][1] [2] [3] [4] [5]
	* e.g. Pattern: "D	u	n	d	e	e"
	* int(pat[0]) = 68 - The ASCII value of capital  'D'
	* int(pat[1]) = 117 - The ASCII value of capital 'u'
	* int(pat[2]) = 110 - The ASCII value of capital 'n'
	* int(pat[3]) = 100 - The ASCII value of capital 'd'
	* int(pat[4]) = 101 - The ASCII value of capital 'e'
	* int(pat[5]) = 101 - The ASCII value of capital 'e'

	* skip[68] = (pat_len(6) - 1) - i(0)	->	skip[68] = (6 - 1) - 0	->	skip[68] = 5
	* skip[117] = (pat_len(6) - 1) - i(1)	->	skip[117] = (6 - 1) - 1	->	skip[117] = 4
	* skip[110] = (pat_len(6) - 1) - i(2)	->	skip[110] = (6 - 1) - 2	->	skip[110] = 3
	* skip[100] = (pat_len(6) - 1) - i(3)	->	skip[100] = (6 - 1) - 3	->	skip[100] = 2
	* skip[101] = (pat_len(6) - 1) - i(4)	->	skip[101] = (6 - 1) - 4	->	skip[101] = 1 ???
	* skip[101] = (pat_len(6) - 1) - i(5)	->	skip[101] = (6 - 1) - 5	->	skip[101] = 0 ???
	*
	* Need this clarified for what happens in the case of repeated letters?
	* It doesn't matter that the value at the index is overwritten by a new amount which
	* is representative of its position relative to the end of the pattern counting backwards.
	* As this will be the new amount we can skip.
	*/
	for (Position i = 0; i < pat_len; ++i)
	{
		skip[int(pat[i])] = (pat_len - 1) - i;
	}

	// Advance along the text.
	for (Position i = 0; i < text_len - pat_len; ++i)
	{
		show_context(text, i);

		/*
		 * Get the char at the position in the text from the index value + pat_len - 1
		 * Subscript the skip array with that value
		 * Set that to the variable 's'
		 *
		 *				[0] [1] [2] [3] [4] [5] [6] [7] [8] [9] [10] [11] [12] [13]	[14] [15]......
		 * e.g. Text	"U	 n	 i	 v	 e	 r	 s	 i	 t	 y		   o	f		  D	   u   n  d	 e	 e"
		 *
		 * text[i + pat_len - 1]	->	text[0 + (6 - 1)]	-> text[5] = 'r'	-> ASCII 'r' = 114
		 * s = skip[114]	= 6 - DEFAULT VALUE
		 * text[i + pat_len - 1]	->	text[
		 */
		int s = skip[int(text[i + pat_len - 1])];

		/*
		 * If s != 0, this means that we have found a letter that is contained in the pattern somewhere.
		 * OR the DEFAULT value of 6. We then skip by this amount.
		 * Enter the code block and advance the loop index 'i' by and amount (s - 1).
		 * Skip over the rest of this iteration, proceeding to the iteration where we check everything again.
		 *
		 * If s == 0 this means we have found the letter that corrosponds to the last letter of the pattern
		 * Which means we cannot skip any but must instead check over all the letters in the pattern against
		 * the text as in the normal skip algo function.
		 */
		if (s != 0)
		{
			i += (s - 1);	// Skip forwards by an amount maximum pat_len - 1
			continue;
		}

		Position j;

		// Advance along the pattern.
		for (j = 0; j < pat_len; j++)
		{
			show_context(pat, j);

			// Check each letter of the pattern against the text.
			if (text[i + j] != pat[j])
			{
				break; // Doesn't match here.
			}
		}


		// This is why the Position j variable is outside the condition.
		// If it were inside the for loops condition, the j variable would die
		// at the end of the for loop code block.
		if (j == pat_len)
		{
			matchedPositions.push_front(i);
		}
	}

	return matchedPositions; // Not found.
}

int main(int argc, char *argv[])
{
	string text;
	text = "University of Abertay Dundee, Bell Street, Dundee, Scotland";
	/*load_jute_book(text);
	load_file("jute-book.txt", text);*/

	string pat = "Dundee";
	//Position pos = find_bruteforce(pat, text);
	//Position pos = find_skipping(pat, text);
	//Position pos = find_boyer_moore(pat, text);
	list<Position> positions = find_boyer_moore_multi(pat, text);
	int count = 0;

	if (positions.empty())
	{
		std::cout << "No matches found!\n";

		return 0;
	}
	else
	{
		for (auto& it = positions.begin(); it != positions.end(); ++it)
		{
			std::cout << "Found '" << pat << "' at position " << *it << ":\n";
			++count;
		}

		std::cout << "\nThe word " << pat << " occurs " << count << " times.\n";
	}

	//cout << "Found '" << pat << "' at position " << pos << ":\n";

	//show_context(text, pos);

	return 0;
}