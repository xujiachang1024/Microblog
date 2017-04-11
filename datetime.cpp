#include "datetime.h"
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <stdexcept>

using namespace std;

DateTime::DateTime () {
	time_t rawtime;
  	struct tm * timeinfo;
  	time (&rawtime);
  	timeinfo = localtime(&rawtime);

  	this->year = timeinfo->tm_year + 1900;
  	this->month = timeinfo->tm_mon + 1;
  	this->day = timeinfo->tm_mday;
  	this->hour = timeinfo->tm_hour;
  	this->min = timeinfo->tm_min;
  	this->sec = timeinfo->tm_sec;
}

DateTime::DateTime(int hh, int mm, int ss, int year, int month, int day) {
	this->hour = hh;
	this->min = mm;
	this->sec = ss;
	this->year = year;
	this->month = month;
	this->day = day;
}

bool DateTime::operator<(const DateTime& other) const {
	
	// year comparison: less than
	if (this->year < other.year) { return true; }

	// year comparison: larger than
	else if (this->year > other.year) { return false; }

	// year comparison: equal to
	else {

		// month comparison: less than
		if (this->month < other.month) { return true; }

		// month comparison: larger than
		else if (this->month > other.month) { return false; }

		// month comparison: equal to
		else {

			// day comparison: less than
			if (this->day < other.day) { return true; }

			// day comparison: larger than
			else if (this->day > other.day) { return false; }

			// day comparison: equal to
			else {

				// hour comparison: less than
				if (this->hour < other.hour) { return true; }

				// hour comparison: larger than
				else if (this->hour > other.hour) { return false; }

				// hour comparison: equal to 
				else {

					// minute comparison: less than 
					if (this->min < other.min) { return true; }

					// minute comparison: larger than
					else if (this->min > other.min) { return false; }

					// minute comparison: equal to 
					else {

						// second comparison: less than
						if (this->sec < other.sec) { return true; }

						// second comparison: larger than or equal to
						else { return false; }
					}
				}
			}
		}
	}
}

std::ostream& operator<<(std::ostream& os, const DateTime& other) {
	
	// YYYY-
	os << other.year << "-";

	// MM-
	if (other.month < 10) {
		os << "0" << other.month << "-";
	}
	else {
		os << other.month << "-";
	}

	// DD 
	if (other.day < 10) {
		os << "0" << other.day << " ";
	}
	else {
		os << other.day << " ";
	}

	// HH:
	if (other.hour < 10) {
		os << "0" << other.hour << ":";
	}
	else {
		os << other.hour << ":";
	}

	// MM:
	if (other.min < 10) {
		os << "0" << other.min << ":";
	}
	else {
		os << other.min << ":";
	}

	// SS
	if (other.sec < 10) {
		os << "0" << other.sec;
	}
	else {
		os << other.sec;
	}

	return os;
}

std::istream& operator>>(std::istream& is, DateTime& dt) {
	
	DateTime current;
	bool formatError = false;

	std::string strDate;
	std::string strTime;
	is >> strDate >> strTime;
	std::stringstream ssDate(strDate);
	std::stringstream ssTime(strTime);

	// read in the date module
	std::string bufDate;
	if (std::getline(ssDate, bufDate, '-')) {
		// year
		stringstream ssYear(bufDate);
		if (ssYear >> dt.year) {}
		else { formatError = true; }
		if ((int)bufDate.size() != 4) { formatError = true; }

		if (std::getline(ssDate, bufDate, '-')) {
			// month
			stringstream ssMonth(bufDate);
			if (ssMonth >> dt.month) {}
			else { formatError = true; }
			if (dt.month < 1 || dt.month > 12) { formatError = true; }

			if (ssDate >> bufDate) {
				// day
				stringstream ssDay(bufDate);
				if (ssDay >> dt.day) {}
				else { formatError = true; }

				if (dt.month == 2) {
					if ((dt.year) % 4 == 0) {
						if (dt.day < 1 || dt.day > 29) { formatError = true; }
					}
					else {
						if (dt.day < 1 || dt.day > 28) { formatError = true; }
					}
				}
				else if (dt.month == 4 ||
						 dt.month == 6 ||
						 dt.month == 9 ||
						 dt.month == 11) {
					if (dt.day < 1 || dt.day > 30) { formatError = true; }
				}
				else {
					if (dt.day < 1 || dt.day > 31) { formatError = true; }
				}
			}
			else { formatError = true; }
		}
		else { formatError = true; }
	}
	else { formatError = true; }

	// read in the time module
	std::string bufTime;
	if (std::getline(ssTime, bufTime, ':')) {
		// hour
		stringstream ssHour(bufTime);
		if (ssHour >> dt.hour) {}
		else { formatError = true; }
		if (dt.hour < 0 || dt.hour > 23) { formatError = true; }

		if (std::getline(ssTime, bufTime, ':')) {
			// minute
			stringstream ssMin(bufTime);
			if (ssMin >> dt.min) {}
			else { formatError = true; }
			if (dt.min < 0 || dt.min > 59) { formatError = true; }

			if (ssTime >> bufTime) {
				// second
				stringstream ssSec(bufTime);
				if (ssSec >> dt.sec) {}
				else { formatError = true; }
				if (dt.sec < 0 || dt.sec > 59) { formatError = true; }
			}
			else { formatError = true; }
		}
		else { formatError = true; }
	}
	else { formatError = true; }

	// CANNOT be larger than current system time
	if (current < dt) { formatError = true; }

	// if formart error: reset to current system time
	if (formatError == true) {
		dt = current;
	}

	return is;
}