#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

const int SONGAMOUNT = 120450;
const int ARTIST = 2;
string songList[SONGAMOUNT][ARTIST];

bool readSongs(string fileName, string songs[][2], int& count);
void printSongs(const string songs[][2], int count);

int main() {
	string songInfo = "spotify_songs_tab_artists.dat";
	int songNum = 0;
	bool fileRead = false;

	fileRead = (readSongs(songInfo, songList, songNum));
	string status = (fileRead) ? "Good" : "Bad";

	if (fileRead) {
		cout << left << setw(7) << "File:" << status << endl;
		cout << left << setw(7) << "Songs:" << songNum << '\n' << endl;
	}

	printSongs(songList, 20);

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
