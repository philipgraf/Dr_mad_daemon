#include "Game.h"
#include <boost/filesystem.hpp>

using namespace std;

int main() {
	string home = "";
	if (getenv("HOME") != NULL) {
		home = getenv("HOME");
	} else {
		home = ".";
	}

	if(!boost::filesystem::create_directory(boost::filesystem::path(home+"/.DIS"))){
		cout << "unable to create " << home + "/.DIS Directory" << endl;
	}

	// create new game object and run
	Game game;
	return game.execute();

}
