#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;
using namespace std::chrono;	
// std::chrono is used for time functions. And so I don't clog up the lines with std::chrono::duration_cast<std::chrono::microseconds>
const int SONGAMOUNT = 120450;
const int ARTIST = 2;
string songList[SONGAMOUNT][ARTIST];

bool readSongs(string fileName, string songs[][2], int& count);
int getTestAmount();
void printSongs(const string songs[][2], int count);
char selectMethod(string optionOne, string optionTwo);
void swap(string line1[], string line2[]);
void bubbleSort(string songs[][2], int size);
void selectionSort(string songs[][2], int size);
int linearSearchforSong(const string songs[][2], int size, string grail, int& comparisons);
int binarySearchForSong(const string songs[][2], int size, string grail, int& comparisons);
void displayResultInfo(string name[], int type[], long long time[], int index, int comparisons);

int main() {
	string songInfo = "spotify_songs_tab_artists.dat";
	int songNum = 0;
	int comparisons = 0;
	int indexFound = 0;

	string methodName[4] = { "Bubble Sort", "Selection Sort", "Linear Search", "Binary Search" };
	int methodChoice[2] = {};
	long long methodTime[2] = {};

	string songReq;
	char methodType;
	int testAmount;

	bool fileRead = false;

	fileRead = (readSongs(songInfo, songList, songNum));
	string status = (fileRead) ? "Good" : "Bad";

	cout << left << setw(7) << "File:" << status << endl;
	cout << left << setw(7) << "Songs:" << songNum << '\n' << endl;
	
	if (!fileRead) {
		cout << "File not found. Exiting program." << endl;
		exit(1);
	}

	testAmount = getTestAmount();
	printSongs(songList,testAmount);

	cout << "Search up a Song\nEnter: ";
	getline(cin, songReq);
	cout << endl;

	/*  Selects sort type
		1 : Bubble Sort
		2 : Selection Sort
	*/ 
	methodType = selectMethod(methodName[0], methodName[1]);
	methodChoice[0] = (methodType-'0');			// int to char conversion

	cout << testAmount << " songs successfully sorted!\n" << endl;

	if (methodType == '1') {
		auto start = high_resolution_clock::now();

		bubbleSort(songList, testAmount);

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);

		methodTime[0] = duration.count();
	}
	else {
		auto start = high_resolution_clock::now();

		selectionSort(songList, testAmount);

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);

		methodTime[0] = duration.count();
	}

	/*  Selects sort type
		1 : Linear Search
		2 : Binary Search
	*/
	methodType = selectMethod(methodName[2], methodName[3]);
	methodChoice[1] = (methodType - '0');		// int to char conversion
	
	if (methodType == '1') {
		auto start = high_resolution_clock::now();

		indexFound = linearSearchforSong(songList, testAmount, songReq, comparisons);

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);

		methodTime[1] = duration.count();

	}
	else {
		auto start = high_resolution_clock::now();

		indexFound = binarySearchForSong(songList, testAmount, songReq, comparisons);

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);

		methodTime[1] = duration.count();
	}

	displayResultInfo(methodName, methodChoice, methodTime, indexFound, comparisons);

	return 0;
}

// reads file and appends title and author info to songs. also cleans author string
bool readSongs(string fileName, string songs[][2], int& count) {
	ifstream openFile(fileName);
	string title, author;

	if (openFile) {
		openFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		while (count < SONGAMOUNT && getline(openFile, title, '\t') && getline(openFile, author)) {
			//removes quotes and brackets, accounts for single or multi author songs
			if (author.size() >= 2) {
				author.erase(remove(author.begin(), author.end(), '['), author.end());
				author.erase(remove(author.begin(), author.end(), '\''), author.end());
				author.erase(remove(author.begin(), author.end(), '\"'), author.end());
				author.erase(remove(author.begin(), author.end(), ']'), author.end());
			}

			songs[count][0] = title;
			songs[count][1] = author;
			count++;
		}

		openFile.close();

		return true;
	}
	else
		return false;
}

// ensures test amount is less than SONGAMOUNT, more than 0, and also an integer.
int getTestAmount() {
	int amount;

	while (true) {
		cout << "Choose an amount of songs to display\nEnter: ";
		cin >> amount;
		cout << endl;
		if (cin.fail()) {
			cout << "Must be an integer and less than " << SONGAMOUNT << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else if (amount > SONGAMOUNT) {
			cout << "That value is higher than the amount of songs indexed." << endl;
		}
		else {
			if (amount > 10000) {
				cout << "It may take a bit to complete functions" << endl;
				break;
			}
			else if (amount <= 0)
				cout << "Value must be more than 0" << endl;
			else
				break;
		}
	}

	cin.ignore(numeric_limits<streamsize>::max(), '\n'); // prevents cin from being carried over to select method function

	return amount;
}

// displays a given amount of songs in the command line
void printSongs(const string songs[][2], int count) {
	cout << "Song Information\n==================================" << endl;

	for (int i = 0; i < count; i++) {
		cout << left << setw(8) << "Title:" << songs[i][0] << endl;
		cout << left << setw(8) << "Artist:" << songs[i][1] << '\n' << endl;
	}
}

// displays method menus and returns the char input
char selectMethod(string optionOne, string optionTwo) {
	char input;

	cout << "Select a method" << endl;
	cout << "1. " << optionOne << endl;
	cout << "2. " << optionTwo << endl;

	while (true) {
		cout << "Enter: ";
		cin >> input;
		if (input == '1' || input == '2')
			break;
		else {
			cout << "Not 1 or 2." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}

	cout << endl;

	return input;
}

// swaps the position of two values, accounts for 2d array
void swap(string line1[], string line2[]) {
	string temp1 = line1[0];
	string temp2 = line1[1];

	line1[0] = line2[0];
	line2[0] = temp1;
	line1[1] = line2[1];
	line2[1] = temp2;
}

// standard sort algorithms
void bubbleSort(string songs[][2], int size) {
	int maxElement;
	int index;

	for (maxElement = size - 1; maxElement > 0; maxElement--) {
		for (index = 0; index < maxElement; index++) {
			if (songs[index][0] > songs[index + 1][0])
				swap(songs[index], songs[index + 1]);
		}
	}
}
void selectionSort(string songs[][2], int size) {
	for (int i = 0; i < (size - 1); i++) {
		int minPos = i;
		for (int j = i + 1; j < size; j++) {
			if (songs[j][0] < songs[minPos][0])
				minPos = j;
		}

		if (minPos != i) {
			swap(songs[i], songs[minPos]);
		}
	}
}

// standard search algorithms
int linearSearchforSong(const string songs[][2], int size, string grail, int& comparisons) {
	int position = -1;
	int index = 0;
	bool found = false;

	while (!found && index < size) {
		if (songs[index][0] == grail) {
			found = true;
			position = index;
		}
		index++;
		comparisons++;
	}

	return position;
}
int binarySearchForSong(const string songs[][2], int size, string grail, int& comparisons) {
	int first = 0;
	int middle;
	int last = size - 1;
	int position = -1;
	bool found = false;

	while (!found && first <= last) {
		middle = (first + last) / 2;
		if (songs[middle][0] == grail) {
			found = true;
			position = middle;
		}
		else if (songs[middle][0] > grail)
			last = middle - 1;
		else
			first = middle + 1;
		comparisons++;
	}

	return position;
}

// displays song information and the amount of microseconds for both methods
void displayResultInfo(string name[], int type[], long long time[], int index, int comparisons) {
	int sortIndex = type[0] - 1;
	int searchIndex = type[1] + 1;
	size_t borderSize = 20;	// set to 20 by default
	// matches the border size to the largest variable
	if (index != -1)
		borderSize += max(songList[index][0].size(), songList[index][1].size());
	else
		borderSize += 20;

	cout << "Result Information\n" << string(borderSize, '=') << endl;
	cout << left << setw(20) << "Song Found" << ((index != -1)? "True" : "False") << endl;
	if (index != -1) {
		cout << left << setw(20) << "Title" << songList[index][0] << endl;
		cout << left << setw(20) << "Artist(s)" << songList[index][1] << endl;
	}
	cout << left << setw(20) << name[sortIndex] << time[0] << " microseconds" << endl;
	cout << left << setw(20) << name[searchIndex] << time[1] << " microseconds" << endl;
	cout << left << setw(20) << "Index" << index << endl;
	cout << left << setw(20) << "Comparisons" << comparisons << endl;
}
