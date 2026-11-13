#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

const int SONGAMOUNT = 120450;
const int ARTIST = 2;
const int SEARCHES = 2;
const int SORTS = 2;

string songList[SONGAMOUNT][ARTIST];

bool readSongs(string fileName, string songs[][2], int& count);
void printSongs(const string songs[][2], int count);
int linearSearchforSong(const string songs[][2], int size, string grail, int& comparisons);
void updateDurationList(int timeList[], auto duration);
void displaySearchTimes(const int arr[], int size);


int main() {
	string songInfo = "spotify_songs_tab_artists.dat";
	int songNum = 0;
	bool fileRead = false;
	int comparisons = 0;
	string songReq;
	
	int searchTypes[SEARCHES];

	fileRead = (readSongs(songInfo, songList, songNum));
	string status = (fileRead) ? "Good" : "Bad";

	if (fileRead) {
		cout << left << setw(7) << "File:" << status << endl;
		cout << left << setw(7) << "Songs:" << songNum << '\n' << endl;
	}

	printSongs(songList, 20);

	cout << "Search up a Song\nEnter: ";
	cin >> songReq;

	auto start = high_resolution_clock::now();

	comparisons = linearSearchforSong(songList, SONGAMOUNT, songReq, comparisons);

	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(stop - start);

	updateDurationList(searchTypes, duration);
	displaySearchTimes(searchTypes, SEARCHES);

	cout << comparisons << endl;
	
	
	return 0;
}

bool readSongs(string fileName, string songs[][2], int& count) {

	ifstream openFile(fileName);
	string title, author;

	if (openFile) {
		while (count < SONGAMOUNT && getline(openFile, title, '	') && getline(openFile, author)) {
			getline(openFile, title, '	');
			getline(openFile, author);
			
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
void updateDurationList(int timeList[], auto duration) {
	static int callAmount = 0;
	timeList[callAmount] = duration.count();
	callAmount++;
}
void displaySearchTimes(const int arr[], int size) {
	// use switch statement for more precise chart later
	for (int i = 0; i < size; i++) {
		cout << "Search " << i+1 << ": " << arr[i] << " microseconds" << endl;
	}
}
