#include "datetime.h"
#include "user.h"
#include "tweet.h"
#include <iostream>
#include <sstream>
#include <string>
#include <string>
#include <vector>
#include <set>
#include <ctype.h>

// Default Constructor
Tweet::Tweet() {

}

// ALternative Constructor
Tweet::Tweet(User* user, DateTime& dt, std::string& text) {
	_user = user;
	_time = dt;
	_text = text;

	// hashtags:
	std::stringstream ss(text);
	std::string buffer;
	while (getline(ss, buffer, '#')) {
		if (ss >> buffer) {
			for (int i = 0; i < (int)buffer.size(); i++) {
				buffer[i] = tolower(buffer[i]);
			}
			_hashtags.insert(buffer);
		}
	}

	// mentions:
	std::stringstream ss2(text);
	while (getline(ss2, buffer, '@')) {
		if (ss2 >> buffer) {
			_mentions.insert(buffer);
		}
	}
}

// returns the timestamp of the tweet
DateTime const & Tweet::time() const {
	return _time;
}

// returns the actual text of the tweet
std::string const & Tweet::text() const {
	return _text;
}

// returns the hashtagged words of the tweet
std::set<std::string> Tweet::hashTags() const {
	return _hashtags;
}

// returns the names of mentioned users
std::set<std::string> Tweet::mentions() const {
	return _mentions;
}

// returns of result of less-than comparison of two tweets
bool Tweet::operator<(const Tweet& other) const {
	return (this->_time < other._time);
}

// returns the ostream passed into the function
std::ostream& operator<<(std::ostream& os, const Tweet& t) {
	os << t._time << " " << t._user->name() << " " << t._text;
	return os;
}

// returns the user of the tweet
User* Tweet::user() const {
	return _user;
}