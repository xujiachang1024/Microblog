#include "user.h"
#include "tweet.h"
#include "datetime.h"
#include "twiteng.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
#include <stdexcept>

using namespace std;

// Default Constructor
TwitEng::TwitEng() {

}

// Destructor
TwitEng::~TwitEng() {
 	
 	// delete all users in the heap
 	std::map< std::string, User* >::iterator itUser;
 	for (itUser = all_Users.begin(); itUser != all_Users.end(); ++itUser) {
 		delete itUser->second;
 	}

 	// delete all tweets in the heap
 	std::set<Tweet*>::iterator itTweet;
 	for (itTweet = all_Tweets.begin(); itTweet != all_Tweets.end(); ++itTweet) {
 		delete *itTweet;
 	}
}

// parses the Twitter database
// populates internal structures
// parameter: filename of the database file
// returns true if failure, or false if success
bool TwitEng::parse(char* filename) {

	// ifstream
	std::ifstream ifile(filename);
	std::string numLine;
	getline(ifile, numLine);
	std::stringstream ss(numLine);
	int num_ppl; // number of people
	ss >> num_ppl;

	// read in all users
	for (int i = 0; i < num_ppl; i++) {
		
		// stringstream: the whole line.
		std::string userLine;
		getline(ifile, userLine);
		std::stringstream ss(userLine);

		// THIS USER: the first name <first, user>
		std::string first;
		ss >> first;
		// if this user is NOT already in the map: insert
		if (all_Users.find(first) == all_Users.end() && first != "") {
			User* user = new User(first);
			all_Users.insert(make_pair(first, user));
		}

		// FOLLOWINGS: the rest names <next, following>
		std::string next;
		while (ss >> next) {
			// if this following is NOT already in the map: insert
			if (all_Users.find(next) == all_Users.end() && next != "") {
				User* following = new User(next);
				all_Users.insert(make_pair(next, following));
			}
			// confirm following/follower relationship
			all_Users[first]->addFollowing(all_Users[next]);
			all_Users[next]->addFollower(all_Users[first]);
		}
	}

	// read in all tweets
	std::string tweetLine;
	while (std::getline(ifile, tweetLine)) {
		
		if (tweetLine != "") {
			std::stringstream ss(tweetLine);
			DateTime dt;
			std::string name;
			std::string text;

			// read in one tweet
			ss >> dt;
			ss >> name;
			getline(ss, text);

			try { this->addTweet(name, dt, text); }
			catch (std::string& o) { return true; }
		}
	}

	/*
	// find everyone's unfollowed people
	map<string, User*>::iterator it;
	for (it = all_Users.begin(); it != all_Users.end(); ++it) {
		string hisName = it->first;
		set<User*> hisFollowing = (it->second)->following();
		set<User*> hisStrangers;
		map<string, User*>::iterator jt;
		for (jt = all_Users.begin(); jt != all_Users.end(); ++jt) {
			if (jt->first != hisName && hisFollowing.find(jt->second) == hisFollowing.end()) {
				hisStrangers.insert(jt->second);
			}
		}
		un_Following.insert(make_pair(hisName, hisStrangers));
	}
	*/

	ifile.close();
	return false;
}

// adds one tweet into the system
// parameters: username, timestamp, text
void TwitEng::addTweet(std::string& username, DateTime& dt, std::string& text) {
	
	// if the corresponding name does NOT already exist in the system: throw
	if (all_Users.find(username) == all_Users.end()) {
		std::cout << username << " is an invalid user!" << std::endl;
		throw std::string("Invalid user!");
	}

	// add this tweet into the system
	Tweet* oneTweet = new Tweet(all_Users[username], dt, text);
	all_Tweets.insert(oneTweet);
	all_Users[username]->addTweet(oneTweet);

	// add this tweet to mentioned users
	std::set<std::string> mentions = oneTweet->mentions();
	std::set<std::string>::iterator it;
	for (it = mentions.begin(); it != mentions.end(); ++it) {
		// if this mentioned user does NOT exist in the system: throw
		if (all_Users.find(*it) == all_Users.end()) {
			std::cout << "@" << *it << " is an invalid mention!" << std::endl;
			throw std::string("Invalid mention!");
		}
		// if this mentioned user does exist in the system;
		else {
			all_Users[*it]->addMentioned(oneTweet);
		}
	}

	// add this tweet into the hashtag map;
	std::set<std::string> tweetHash = oneTweet->hashTags();
	for (it = tweetHash.begin(); it != tweetHash.end(); ++it) {
		// if this hashtag does NOT already exist in the system
		if (all_Hashtags.find(*it) == all_Hashtags.end()) {
			std::vector<Tweet*> tweetVector;
			tweetVector.push_back(oneTweet);
			all_Hashtags.insert( make_pair(*it, tweetVector) );
		}
		// if this hashtag does already exist in the system
		else {
			all_Hashtags[*it].push_back(oneTweet);
		}
	}
}

// searches for tweets with the given words and strategy
// parameters:
//     words: the hashtag terms
//     strategy: 0 = AND, 1 = OR
// returns the tweets that match the search
std::vector<Tweet*> const TwitEng::search(std::vector<std::string>& terms, int strategy) {
	
	std::vector<Tweet*> results;
	std::vector<Tweet*> temp;

	// 0 = AND
	if (strategy == 0) {
		bool start = false;
		for (int i = 0; i < (int)terms.size(); i++) {
			if (start == false) {
				if (all_Hashtags.find(terms[i]) != all_Hashtags.end()) {
					results.clear();
					std::set_union(temp.begin(), temp.end(),
						all_Hashtags[terms[i]].begin(), all_Hashtags[terms[i]].end(), 
						std::back_inserter(results));
					temp.clear();
					temp = results;
					start = true;
				}
			}
			else if (start == true) {
				if (all_Hashtags.find(terms[i]) != all_Hashtags.end()) {
					results.clear();
					std::set_intersection(temp.begin(), temp.end(),
						all_Hashtags[terms[i]].begin(), all_Hashtags[terms[i]].end(), 
						std::back_inserter(results));
					temp.clear();
					temp = results;
				}
			}
		}
	}

	// 1 = OR
	else if (strategy == 1 ) {
		for (int i = 0; i < (int)terms.size(); i++) {
			if (all_Hashtags.find(terms[i]) != all_Hashtags.end()) {
				results.clear();
				std::set_union(temp.begin(), temp.end(),
					all_Hashtags[terms[i]].begin(), all_Hashtags[terms[i]].end(), 
					std::back_inserter(results));
				temp.clear();
				temp = results;
			}
		}
	}

	// strategy = other integers
	else {
		throw std::string("Invalid strategy!");
	}
	
	return results;
}

// dumps feeds of each user to their own file
void TwitEng::dumpFeeds() {

	// iterate through every user
	std::map< std::string, User* >::iterator itUser;
	for (itUser = all_Users.begin(); itUser != all_Users.end(); ++itUser) {
		
		// print the name of this user
		std::string name = itUser->first;
		std::string file = name + std::string(".feed");
		std::ofstream ofile( file.c_str() );
		ofile << name << std::endl;

		// print the feed of this user
		std::vector<Tweet*> feed = itUser->second->mainFeed();
		for (int i = 0; i < (int)feed.size(); i++) {
			ofile << *(feed[i]) << std::endl;
		}
	}
}

std::map< std::string, User* > const & TwitEng::returnUsers() const {
	return all_Users;
}

std::set< Tweet* > const & TwitEng::returnTweets() const {
	return all_Tweets;
}

/*
std::map< std::string, std::set<User*> > const & TwitEng::unFollowing() const {
	return un_Following;
}
*/


void TwitEng::addFollowing(string user, string target) {
	if (all_Users.find(user) != all_Users.end() && all_Users.find(target) != all_Users.end()) {
		all_Users[user]->addFollowing(all_Users[target]);
		
		/*
		// clear the map for safety
		un_Following.clear();

		// find everyone's unfollowed people
		map<string, User*>::iterator it;
		for (it = all_Users.begin(); it != all_Users.end(); ++it) {
			string hisName = it->first;
			set<User*> hisFollowing = (it->second)->following();
			set<User*> hisStrangers;
			map<string, User*>::iterator jt;
			for (jt = all_Users.begin(); jt != all_Users.end(); ++jt) {
				if (jt->first != hisName && hisFollowing.find(jt->second) == hisFollowing.end()) {
					hisStrangers.insert(jt->second);
				}
			}
			un_Following.insert(make_pair(hisName, hisStrangers));
		}
		*/
	}
}

set< vector<User*> > TwitEng::strongConnect() {
	this->resetIndex();
	int index = 0;
	stack<User*> nodes;
	set< vector<User*> > components;
	map<string, User*>::iterator it;
	for (it = all_Users.begin(); it != all_Users.end(); ++it) {
		if (it->second->index == -1) {
			this->strongConnectHelper(it->second, index, nodes, components);
		}
	}
	return components;
}

void TwitEng::resetIndex() {
	map<string, User*>::iterator it;
	for (it = all_Users.begin(); it != all_Users.end(); ++it) {
		it->second->index = -1;
		it->second->lowLink = -1;
		it->second->onStack = false;
	}
}

void TwitEng::strongConnectHelper(User* user, int& index, stack<User*>& nodes, set< vector<User*> >& components) {
	
	// set the depth index for v to the smallest unused index
	user->index = index;
	user->lowLink = index;
	index++;
	nodes.push(user);
	user->onStack = true;

	// consider successors of v
	set<User*> following = user->following();
	set<User*>::iterator it;
	for (it = following.begin(); it != following.end(); ++it) {
		// successor w has not yet been visited: recurse on it
		if ((*it)->index == -1) {
			strongConnectHelper(*it, index, nodes, components);
			user->lowLink = min(user->lowLink, (*it)->lowLink);
		}
		// successor w is in stack and hence in the current SCC
		else if((*it)->onStack) {
			user->lowLink = min(user->lowLink, (*it)->index);
		}
	}

	// if v is a root node, pop the stack and generate an SCC
	if (user->lowLink == user->index) {
		vector<User*> oneComp;
		while (!nodes.empty()) {
			User* w = nodes.top();
			nodes.pop();
			w->onStack = false;
			oneComp.push_back(w);
			if (w == user) {
				break;
			}
		}
		components.insert(oneComp);
	}
}