#include <QApplication>
#include "datetime.h"
#include "user.h"
#include "tweet.h"
#include "twiteng.h"
#include "interface.h"

using namespace std;

int main (int argc, char* argv[]) {

	if (argc < 2) {
    	cerr << "Please provide the twitter data file" << endl;
    	return 1;
  	}

	QApplication app(argc, argv);
	//cout << 0 << endl;
	Interface twitter(argv[1]);
	//cout << 1 << endl;
	twitter.setWindowTitle("Twitter");
	//cout << 2 << endl;
	twitter.show();
	//cout << 3 << endl;
	return app.exec();
}