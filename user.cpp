#include "datetime.h"
#include "user.h"
#include "tweet.h"
#include <string>
#include <set>
#include <list>
#include <vector>
#include <algorithm>

// Constructor
User::User(std::string name) {
	_name = name;
	index = -1;
	lowLink = -1;
	onStack = false;
}

// Destructor
User::~User() {

}

// returns the name of the user
std::string User::name() const {
	return _name;
}

// returns the set of followers of the user
std::set<User*> User::followers() const {
	return _followers;
}

// returns the set of following of the user
std::set<User*> User::following() const {
	return _following;
}

// returns the list of tweets of the user
std::list<Tweet*> User::tweets() const {
	return _tweets;
}

// adds one follower
void User::addFollower(User* u) {
	this->_followers.insert(u);
}

// adds one following
void User::addFollowing(User* u) {
	if (this != u) {
		this->_following.insert(u);
	}
}

// adds one tweet
void User::addTweet(Tweet* t) {
	this->_tweets.push_back(t);
}

// adds the tweet that mentioning this user
void User::addMentioned(Tweet* t) {
	this->_mentioned.push_back(t);
}

// returns all his and his following's tweets in timestamp order;
std::vector<Tweet*> User::mainFeed() {
	std::vector<Tweet*> feed;

	// save all his tweets into the feed
	for (std::list<Tweet*>::iterator it1 = this->_tweets.begin(); it1 != this->_tweets.end(); ++it1) {
		feed.push_back(*it1);
	}

	// save all his following's tweets into the feed
	for (std::set<User*>::iterator it2 = _following.begin(); it2 != _following.end(); ++it2) {
		std::list<Tweet*> follow = (*it2)->tweets();
		for (std::list<Tweet*>::iterator it3 = follow.begin(); it3 != follow.end(); ++it3) {
			feed.push_back(*it3);
		}
	}

	std::sort(feed.begin(), feed.end(), TweetComp());

	return feed;
}

// returns a vector of time-ordered mention feed
std::vector<Tweet*> User::mentionFeed() {
	std::vector<Tweet*> feed = _mentioned;
	std::sort(feed.begin(), feed.end(), TweetComp());
	return feed;
}