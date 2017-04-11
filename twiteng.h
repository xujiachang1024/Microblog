#ifndef TWITENG_H
#define TWITENG_H
#include <map>
#include <string>
#include <set>
#include <stack>
#include <vector>
#include "user.h"
#include "tweet.h"
#include "datetime.h"

using namespace std;

class TwitEng
{
 public:
  TwitEng();
  ~TwitEng();
  /**
   * Parses the Twitter database and populates internal structures
   * @param filename of the database file
   * @return true if there is an error, false if successful
   */
  bool parse(char* filename);

  /**
   * Allocates a tweet from its parts and adds it to internal structures
   * @param username of the user who made the tweet
   * @param timestamp of the tweet
   * @param text is the actual text of the tweet as a single string
   */
  void addTweet(std::string& username, DateTime& dt, std::string& text);

  /**
   * Searches for tweets with the given words and strategy
   * @param words is the hashtag terms in any case without the '#'
   * @param strategy 0=AND, 1=OR
   * @return the tweets that match the search
   */
  std::vector<Tweet*> const search(std::vector<std::string>& terms, int strategy);

  /**
   * Dump feeds of each user to their own file
   */
  void dumpFeeds();

  /* You may add other member functions */
  std::map< std::string, User* > const & returnUsers() const;
  std::set< Tweet* > const & returnTweets() const;
  //std::map< std::string, std::set<User*> > const & unFollowing() const;
  void addFollowing(std::string user, std::string target);
  // find strongly connected components
  std::set< std::vector<User*> > strongConnect();

 private:

  // data members
  std::map< std::string, User* > all_Users;
  //std::map< std::string, std::set<User*> > un_Following;
  std::set<Tweet*> all_Tweets;
  std::map< std::string, std::vector<Tweet*> > all_Hashtags;

  // helper functions:
  void resetIndex();
  void strongConnectHelper(User* user, int& index, stack<User*>& nodes, set< vector<User*> >& components);

};

#endif