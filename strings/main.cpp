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
Position find_bruteforce(const string& pat, const string& text) {
	Position pat_len = pat.size();
	Position text_len = text.size();

	for (Position i = 0; i < text_len - pat_len; ++i) {
		Position j;
		for (j = 0; j < pat_len; j++) {
			if (text[i + j] != pat[j]) {
				break; // Doesn't match here.
			}
		}
		if (j == pat_len) {
			return i; // Matched here.
		}
	}
	return -1; // Not found.
}

int main(int argc, char *argv[]) {
	string text;
	text = "University of Abertay Dundee, Bell Street, Dundee, Scotland";
	//load_jute_book(text);
	//load_file("my-file.txt", text);

	string pat = "Dundee";
	Position pos = find_bruteforce(pat, text);
	cout << "Found '" << pat << "' at position " << pos << ":\n";
	show_context(text, pos);

	return 0;
}