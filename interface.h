#ifndef INTERFACE_H
#define INTERFACE_H
#include "datetime.h"
#include "user.h"
#include "tweet.h"
#include "twiteng.h"
#include <string>
#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QGridLayout>
#include <QString>

using namespace std;

class Interface : public QWidget {

	Q_OBJECT

public:
	Interface(char* filename);
	~Interface();

private slots:
	void userClicked(QString);
	void postClicked();
	void addClicked(QString);
	void searchClicked();
	void andClicked();
	void orClicked();
	void saveClicked();
	void connectClicked();
	void quitClicked();

private:
	void updateMainFeed(string name);
	void updateMentionFeed(string name);
	void updateListFollowing(string name);
	//void updateAddFollowing(string name);
	void updateListResults(string intput, int strategy);
	// Twitter Engine:
	TwitEng* _engine;
	// Select User:
	QLabel* selectLabel;
	QComboBox* selectUser;
	// Post New Tweet
	QLabel* editLabel;
	QLineEdit* editTweet;
	QPushButton* postButton;
	// Main Feed:
	QLabel* mainLabel;
	QListWidget* mainFeed;
	// Mention Feed:
	QLabel* mentionLabel;
	QListWidget* mentionFeed;
	// List Following:
	QLabel* listLabel;
	QListWidget*  listFollowing;
	// Add Following:
	QLabel* addLabel;
	QComboBox* addFollowing;
	// Hashtag Search:
	QWidget* searchWindow;
	QLabel* hashtagLabel;
	QLineEdit* hashtagEnter;
	QPushButton* andButton;
	QPushButton* orButton;
	QListWidget* listResults;
	QPushButton* searchButton;
	// Save Progress:
	QLabel* saveLabel;
	QLineEdit* outputFile;
	QPushButton* saveButton;
	QPushButton* connectButton;
	// Quit Button:
	QPushButton* quitButton;
	// Grid Layout:
	QGridLayout* mainLayout;
	QGridLayout* searchLayout;
};

#endif