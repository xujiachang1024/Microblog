#include "datetime.h"
#include "user.h"
#include "tweet.h"
#include "twiteng.h"
#include "interface.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <string>
#include <set>
#include <vector>
#include <Qt>
#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QGridLayout>
#include <QString>

using namespace std;

// Constructor w/ input file
Interface::Interface(char* filename) {

	// Twitter Engine
	_engine = new TwitEng();
	_engine->parse(filename);

	// Select User:
	selectLabel = new QLabel("Select User:", this);
	selectUser = new QComboBox(this);
	std::map<string, User*> allUsers = _engine->returnUsers();
	std::map<string, User*>::iterator it;
	for (it = allUsers.begin(); it != allUsers.end(); ++it) {
		QString username = QString::fromStdString(it->first);
		selectUser->addItem(username);
	}
	//cout << "Select User: Success!" << endl;

	// Post New Tweet:
	editLabel = new QLabel("Edit New Tweet:", this);
	editTweet = new QLineEdit(this);
	editTweet->setPlaceholderText("Edit new tweet here");
	postButton = new QPushButton("&Post", this);
	//cout << "Post New Tweet: Success!" << endl;

	// Main Feed:
	mainLabel = new QLabel("Main Feed:", this);
	mainFeed = new QListWidget(this);
	mainFeed->setFixedHeight(96);
	string name = (selectUser->currentText()).toStdString();
	this->updateMainFeed(name);
	//cout << "Main Feed: Success!" << endl;

	// Mention Feed:
	mentionLabel = new QLabel("Mention Feed:", this);
	mentionFeed = new QListWidget(this);
	name = (selectUser->currentText()).toStdString();
	this->updateMentionFeed(name);
	//cout << "Mention Feed: Success!" << endl;

	// List Following:
	listLabel = new QLabel("I'm Following:", this);
	listFollowing = new QListWidget(this);
	name = (selectUser->currentText()).toStdString();
	this->updateListFollowing(name);
	//cout << "List Following: Success!" << endl;

	// Add Following:
	addLabel = new QLabel("Follow Him/Her:", this);
	addFollowing = new QComboBox(this);
	for (it = allUsers.begin(); it != allUsers.end(); ++it) {
		QString username = QString::fromStdString(it->first);
		addFollowing->addItem(username);
	}
	//cout << "Add Following: Success!" << endl;

	// Hashtag Search:
	searchWindow = new QWidget;
	hashtagLabel = new QLabel("Enter Hashtags:", searchWindow);
	hashtagEnter = new QLineEdit(searchWindow);
	hashtagEnter->setPlaceholderText("Enter hashtags here");
	andButton = new QPushButton("AND Search", searchWindow);
	orButton = new QPushButton("OR Search", searchWindow);
	listResults = new QListWidget(searchWindow);
	searchLayout = new QGridLayout;
	searchLayout->addWidget(hashtagLabel, 0, 0, 1, 2);
	searchLayout->addWidget(hashtagEnter, 1, 0, 1, 2);
	searchLayout->addWidget(andButton, 2, 0, 1, 1);
	searchLayout->addWidget(orButton, 2, 1, 1, 1);
	searchLayout->addWidget(listResults, 3, 0, 5, 2);
	searchWindow->setWindowTitle("Hashtag Search");
	searchWindow->setLayout(searchLayout);
	searchButton = new QPushButton("Find &Hashtags...", this);
	//cout << "Hashtag Search: Success!" << endl;

	// Save Progress:
	saveLabel = new QLabel("Extract To:", this);
	outputFile = new QLineEdit(this);
	outputFile->setPlaceholderText("Enter filename here");
	saveButton = new QPushButton("&Save Progress", this);
	connectButton = new QPushButton("Find &Components", this);
	//cout << "Save Progress: Success!" << endl;

	// Quit Button:
	quitButton = new QPushButton("&Quit", this);

	// Main Layout:
	mainLayout = new QGridLayout;
	//cout << "Layout: New Success!" << endl;
	mainLayout->addWidget(selectLabel, 0, 0, 1, 2);
	mainLayout->addWidget(selectUser, 0, 2, 1, 2);
	mainLayout->addWidget(editLabel, 1, 0, 1, 4);
	mainLayout->addWidget(editTweet, 2, 0, 1, 4);
	mainLayout->addWidget(postButton, 3, 3, 1, 1);
	mainLayout->addWidget(mainLabel, 4, 0, 1, 4);
	mainLayout->addWidget(mainFeed, 5, 0, 5, 4);
	mainLayout->addWidget(mentionLabel, 10, 0, 1, 4);
	mainLayout->addWidget(mentionFeed, 11, 0, 5, 4);
	mainLayout->addWidget(listLabel, 16, 0, 1, 2);
	mainLayout->addWidget(listFollowing, 17, 0, 4, 2);
	mainLayout->addWidget(addLabel, 16, 2, 1, 1);
	mainLayout->addWidget(addFollowing, 16, 3, 1, 1);
	mainLayout->addWidget(searchButton, 17, 2, 1, 2);
	mainLayout->addWidget(saveLabel, 18, 2, 1, 2);
	mainLayout->addWidget(outputFile, 19, 2, 1, 2);
	mainLayout->addWidget(saveButton, 20, 2, 1, 1);
	mainLayout->addWidget(connectButton, 20, 3, 1, 1);
	mainLayout->addWidget(quitButton, 21, 0, 1, 4);
	//cout << "Main Layout: Add Success!" << endl;
	this->setLayout(mainLayout);
	//cout << "Main Layout: All Success!" << endl;
	//cout << "Constructor: All Success!" << endl;

	connect(selectUser, SIGNAL(currentIndexChanged(QString)), this, SLOT(userClicked(QString)));
	connect(postButton, SIGNAL(clicked()), this, SLOT(postClicked()));
	connect(addFollowing, SIGNAL(currentIndexChanged(QString)), this, SLOT(addClicked(QString)));
	connect(andButton, SIGNAL(clicked()), this, SLOT(andClicked()));
	connect(orButton, SIGNAL(clicked()), this, SLOT(orClicked()));
	connect(searchButton, SIGNAL(clicked()), this, SLOT(searchClicked()));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));
	connect(connectButton, SIGNAL(clicked()), this, SLOT(connectClicked()));
	connect(quitButton, SIGNAL(clicked()), this, SLOT(quitClicked()));
}

// Destructor
Interface::~Interface() {
	delete _engine;
	/*
	delete selectLabel;
	delete selectUser;
	delete editLabel;
	delete editTweet;
	delete postButton;
	delete mainLabel;
	delete mainFeed;
	delete mentionLabel;
	delete mentionFeed;
	delete listLabel;
	delete listFollowing;
	delete addLabel;
	delete addFollowing;
	delete searchWindow;
	delete hashtagLabel;
	delete hashtagEnter;
	delete andButton;
	delete orButton;
	delete listResults;
	delete searchButton;
	delete saveLabel;
	delete outputFile;
	delete saveButton;
	delete mainLayout;
	delete searchLayout;
	*/
	//cout << "Destructor: All Success!" << endl;
}

void Interface::userClicked(QString) {
	string name = (selectUser->currentText()).toStdString();
	// update Main Feed:
	this->updateMainFeed(name);
	// update Mention Feed:
	this->updateMentionFeed(name);
	// update List Following:
	this->updateListFollowing(name);
	// update Add Following:
	//this->updateAddFollowing(name);
}

void Interface::postClicked() {

	if (editTweet->text() == "") {
		editTweet->setPlaceholderText("Empty Tweet!");
	}

	else {
		string name = selectUser->currentText().toStdString();
		DateTime curr;
		string buffer = editTweet->text().toStdString();
		stringstream ssText(buffer);
		string text;
		if (getline(ssText, text)) {
			_engine->addTweet(name, curr, text);
			// update Main Feed:
			this->updateMainFeed(name);
			// update Mention Feed:
			this->updateMentionFeed(name);
			editTweet->clear();
			editTweet->setPlaceholderText("Edit new tweet here");
		}
		else {
			editTweet->setPlaceholderText("Empty Tweet!");
		}
	}
}

void Interface::addClicked(QString) {
	string name = (selectUser->currentText()).toStdString();
	string target = (addFollowing->currentText()).toStdString();
	_engine->addFollowing(name, target);
	// update Main Feed:
	this->updateMainFeed(name);
	// update List Following:
	this->updateListFollowing(name);
	// update Add Following:
	//this->updateAddFollowing(name);
}

void Interface::searchClicked() {
	searchWindow->show();
}

void Interface::andClicked() {

	if (hashtagEnter->text() == "") {
		hashtagEnter->setPlaceholderText("Empty hashtags!");
	}

	else {
		string buffer = hashtagEnter->text().toStdString();
		stringstream ss(buffer);
		string input;
		if (getline(ss, input)) {
			this->updateListResults(input, 0);
			hashtagEnter->clear();
			hashtagEnter->setPlaceholderText("Enter hashtags here");
		}
		else {
			hashtagEnter->setPlaceholderText("Empty hashtags!");
		}
	}
}

void Interface::orClicked() {

	if (hashtagEnter->text() == "") {
		hashtagEnter->setPlaceholderText("Empty hashtags!");
	}

	else {
		string buffer = hashtagEnter->text().toStdString();
		stringstream ss(buffer);
		string input;
		if (getline(ss, input)) {
			this->updateListResults(input, 1);
			hashtagEnter->clear();
			hashtagEnter->setPlaceholderText("Enter hashtags here");
		}
		else {
			hashtagEnter->setPlaceholderText("Empty hashtags!");
		}
	}
}

void Interface::saveClicked() {

	if (outputFile->text() == "") {
		outputFile->setPlaceholderText("Empty file name!");
	}

	else {
		string filename = outputFile->text().toStdString();
		ofstream ofile(filename.c_str());

		if (ofile.fail()) {
			outputFile->setPlaceholderText("Invalid file name!");
		}

		else {
			std::map<string, User*> allUsers = _engine->returnUsers();
			std::set<Tweet*> allTweets = _engine->returnTweets();

			// number of users
			ofile << (int)allUsers.size() << endl;

			// each user and his following
			std::map<string, User*>::iterator itUser;
			for (itUser = allUsers.begin(); itUser != allUsers.end(); ++itUser) {
				ofile << itUser->first << " ";
				set<User*> hisFoll = itUser->second->following();
				set<User*>::iterator itFoll;
				for (itFoll = hisFoll.begin(); itFoll != hisFoll.end(); ++itFoll) {
					ofile << (*itFoll)->name() << " ";
				}
				ofile << endl;
			}

			// each tweet
			std::set<Tweet*>::iterator itTwt;
			for (itTwt = allTweets.begin(); itTwt != allTweets.end(); ++itTwt) {
				ofile << *(*itTwt) << endl;
			}

			// clear the content
			outputFile->clear();
			outputFile->setPlaceholderText("Enter filename here");
		}
	}
}

void Interface::connectClicked() {

	if (outputFile->text() == "") {
		outputFile->setPlaceholderText("Empty file name!");
	}

	else {
		string filename = outputFile->text().toStdString();
		ofstream ofile(filename.c_str());

		if (ofile.fail()) {
			outputFile->setPlaceholderText("Invalid file name!");
		}

		else {
			set< vector<User*> > components = _engine->strongConnect();
			set< vector<User*> >::iterator it;
			int count = 0;

			for (it = components.begin(); it != components.end(); ++it) {
				vector<User*> oneComp = *it;
				if ((int)oneComp.size() != 0) {
					count++;
					ofile << "Component " << count << endl;
					vector<User*> oneComp = *it;
					for (int i = 0; i < (int)oneComp.size(); i++) {
						ofile << oneComp[i]->name() << endl;
					}
					ofile << endl;
				}
			}

			// clear the content
			outputFile->clear();
			outputFile->setPlaceholderText("Enter filename here");
		}
	}
}

void Interface::quitClicked() {
	this->close();
}

void Interface::updateMainFeed(string name) {
	mainFeed->clear();
	std::map<string, User*> allUsers = _engine->returnUsers();
	vector<Tweet*> feed = allUsers[name]->mainFeed();
	for (int i = 0; i < (int)feed.size(); i++) {
		stringstream ss;
		ss << *(feed[i]);
		string oneTweet;
		getline(ss, oneTweet);
		mainFeed->addItem(QString::fromStdString(oneTweet));
	}
}

void Interface::updateMentionFeed(string name) {
	mentionFeed->clear();
	std::map<string, User*> allUsers = _engine->returnUsers();
	vector<Tweet*> feed = allUsers[name]->mentionFeed();
	for (int i = 0; i < (int)feed.size(); i++) {
		stringstream ss;
		ss << *(feed[i]);
		string oneTweet;
		getline(ss, oneTweet);
		mentionFeed->addItem(QString::fromStdString(oneTweet));
	}
}

void Interface::updateListFollowing(string name) {
	listFollowing->clear();
	map<string, User*> allUsers = _engine->returnUsers();
	set<User*> following = allUsers[name]->following();
	set<User*>::iterator it;
	for (it = following.begin(); it != following.end(); ++it) {
		string other = (*it)->name();
		listFollowing->addItem(QString::fromStdString(other));
	}
}

/*
void Interface::updateAddFollowing(string name) {
	addFollowing->clear();
	map<string, set<User*> > unFollowing = _engine->unFollowing();
	set<User*> strangers = unFollowing[name];
	set<User*>::iterator it;
	for (it = strangers.begin(); it != strangers.end(); ++it) {
		string other = (*it)->name();
		addFollowing->addItem(QString::fromStdString(other));
	}
}
*/

void Interface::updateListResults(string input, int strategy) {
	listResults->clear();
	stringstream ss(input);
	string buffer;
	vector<string> terms;
	while (ss >> buffer) {
		terms.push_back(buffer);
	}
	vector<Tweet*> results = _engine->search(terms, strategy);
	if ((int)results.size() == 0) {
		listResults->addItem("No match found.");
	}
	else {
		for (int i = 0; i < (int)results.size(); i++) {
			stringstream ss;
			ss << *(results[i]);
			string oneTweet;
			getline(ss, oneTweet);
			listResults->addItem(QString::fromStdString(oneTweet));
		}
	}
}