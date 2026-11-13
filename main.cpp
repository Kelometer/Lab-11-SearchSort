#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

const int SONGAMOUNT = 120450;
const int ARTIST = 2;
const int SEARCHNUM = 2;
const int SORTNUM = 2;

string songList[SONGAMOUNT][ARTIST];

bool readSongs(string fileName, string songs[][2], int& count);
void printSongs(const string songs[][2], int count);
char selectMethod(string optionOne, string optionTwo);
int linearSearchforSong(const string songs[][2], int size, string grail, int& comparisons);
int binarySearchForSong(const string songs[][2], int size, string grail, int& comparisons);
void swap(string line1[], string line2[]);
void bubbleSort(string songs[][2], int size);
void selectionSort(string songs[][2], int size);
void updateDurationList(int timeList[], auto duration);
void displayProcessTime(const int arr[], int size, string type);

int main() {
	string songInfo = "spotify_songs_tab_artists.dat";

	int songNum = 0;
	int searchComparison = 0;
	int indexFound;

	string songReq;
	char methodType;

	int searchTypes[SEARCHNUM];
	int sortTypes[SORTNUM];

	bool fileRead = false;

	fileRead = (readSongs(songInfo, songList, songNum));
	string status = (fileRead) ? "Good" : "Bad";

	if (fileRead) {
		cout << left << setw(7) << "File:" << status << endl;
		cout << left << setw(7) << "Songs:" << songNum << '\n' << endl;
	}

	cout << "Search up a Song\nEnter: ";
	getline(cin, songReq);
	cout << endl;
	
	// Selects sort type
	methodType = selectMethod("Bubble Sort", "Selection Sort");

	// 1 : Bubble Sort
	// 2 : Selection Sort
	if (methodType == '1') {
		auto start = high_resolution_clock::now();

		bubbleSort(songList, 100);

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);

		updateDurationList(sortTypes, duration);
	}
	else {
		auto start = high_resolution_clock::now();

		selectionSort(songList, 100);

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);

		updateDurationList(sortTypes, duration);
	}

	// Selects search type
	methodType = selectMethod("Linear Search", "Binary Search");

	// 1 : Linear Search
	// 2 : Binary Search
	if (methodType == '1') {
		auto start = high_resolution_clock::now();

		indexFound = linearSearchforSong(songList, SONGAMOUNT, songReq, searchComparison);

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);

		cout << "\nLinear Search Time: " << duration.count() << " microseconds" << endl;
		
	}
	else {
		auto start = high_resolution_clock::now();

		indexFound = binarySearchForSong(songList, SONGAMOUNT, songReq, searchComparison);

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);

		cout << "\nBinary Search Time: " << duration.count() << " microseconds" << endl;
	}
	cout << left << setw(15) << "Index:" << indexFound << endl;
	cout << left << setw(15) << "Comparisons: " << searchComparison << endl;

	// printSongs(songList, 100);

	return 0;
}

bool readSongs(string fileName, string songs[][2], int& count) {

	ifstream openFile(fileName);
	string title, author;

	if (openFile) {
		while (count < SONGAMOUNT && getline(openFile, title, '	') && getline(openFile, author)) {
			// handles multiple authors
			
			
			// removes the ' characters enclosing the string
			if (author.size() >= 2) {
				author.erase(0, 1);
				author.pop_back();
			}

			songs[count][0] = title;
			songs[count][1] = author;
			count++;
		}
		openFile.close();
		return true;
	}
	else {
		cout << "File not found." << endl;
		return false;
	}
}
void printSongs(const string songs[][2], int count) {
	cout << "Song Information\n==================================" << endl;
	
	for (int i = 0; i < count; i++) {
		cout << left << setw(8) << "Title:" << songs[i][0] << endl;
		cout << left << setw(8) << "Artist:" << songs[i][1] << '\n' << endl;
	}

}

char selectMethod(string optionOne, string optionTwo) {
	char input;

	cout << "Select a method" << endl;
	cout << "1. " << optionOne << endl; 
	cout << "2. " << optionTwo << endl;

	while (true) {
		cout << "Enter: ";
		if (cin >> input) {
			if (input == '1' || input == '2')
				break;
		}
		else {
			cout << "Not 1 or 2." << endl;
		}
	}

	return input;
}

void swap(string line1[], string line2[]) {
	string temp1 = line1[0];
	string temp2 = line1[1];

	line1[0] = line2[0];
	line2[0] = temp1;
	line1[1] = line2[1];
	line2[1] = temp2;
}

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
	for (int i = 0; i < (size-1); i++){
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

void updateDurationList(int timeList[], auto duration) {
	static int callAmount = 0;
	timeList[callAmount] = duration.count();
	callAmount++;
	if (callAmount == 2)
		callAmount = 0;
}
void displayProcessTime(const int arr[], int size, string type) {
	// use switch statement for more precise chart later
	for (int i = 0; i < size; i++) {
		cout << type << " " << i+1 << ": " << arr[i] << " microseconds" << endl;
	}
}
