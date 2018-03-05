#include "Date.hpp"
#include <sstream>
using namespace std;
using std::string;

Date::Date() {
    m_year = m_month = m_day = m_hour = m_minute =0;
}

  /**
  * @brief constructor with arguments
  */
Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute):m_year(t_year),
    m_month(t_month), m_day(t_day), m_hour(t_hour), m_minute(t_minute) {}

/**
* @brief constructor with a string
*/
bool isValids(string dateString) {
    if (dateString.length() != 16) return false;
    for (int i = 0; i < 16; i++) {
        if (i == 4 || i == 7) {
            if (dateString[i] != '-') return false;
        }
        else if (i == 10) {
            if (dateString[i] != '/') return false;
        }
        else if (i == 13) {
            if (dateString[i] != ':') return false;
        } else {
            if (dateString[i] < '0' || dateString[i] > '9') return false;
        }
    }
    return true;
}
Date::Date(std::string dateString) {
    if (isValids(dateString)) {
    string a[5];
    a[0] = dateString.substr(0, 4);
    a[1] = dateString.substr(5, 2);
    a[2] = dateString.substr(8, 2);
    a[3] = dateString.substr(11, 2);
    a[4] = dateString.substr(14, 2);
    int b[5];
    for (int i = 0; i < 5; i++) {
        stringstream s;
        s << a[i];
        s >> b[i];
    }
    Date temp(b[0], b[1], b[2], b[3], b[4]);
    *this = temp;
    } else {
        m_year = m_month = m_day = m_hour = m_minute = 0;
    }
}

int Date::getYear(void) const {
    return m_year;
}

/**
* @brief set the year of a date
* @param a integer indicate the new year of a date
*/
void Date::setYear(const int t_year) {
    m_year = t_year;
}

/**
* @brief return the month of a Date
* @return   a integer indicate the month of a date
*/
int Date::getMonth(void) const {
    return m_month;
}

/**
* @brief set the month of a date
* @param a integer indicate the new month of a date
*/
void Date::setMonth(const int t_month) {
    m_month = t_month;
}

/**
* @brief return the day of a Date
* @return   a integer indicate the day of a date
*/
int Date::getDay(void) const {
    return m_day;
}

/**
* @brief set the day of a date
* @param a integer indicate the new day of a date
*/
void Date::setDay(const int t_day) {
    m_day = t_day;
}

/**
* @brief return the hour of a Date
* @return   a integer indicate the hour of a date
*/
int Date::getHour(void) const {
    return m_hour;
}

/**
* @brief set the hour of a date
* @param a integer indicate the new hour of a date
*/
void Date::setHour(const int t_hour) {
    m_hour = t_hour;
}

/**
* @brief return the minute of a Date
* @return   a integer indicate the minute of a date
*/
int Date::getMinute(void) const {
    return m_minute;
}

/**
* @brief set the minute of a date
* @param a integer indicate the new minute of a date
*/
void Date::setMinute(const int t_minute) {
    m_minute = t_minute;
}

/**
*   @brief check whether the date is valid or not
*   @return the bool indicate valid or not
*/

bool Date::isValid(const Date t_date) {
    if (t_date.getYear() < 1000 || t_date.getYear() > 9999) return false;
    if (t_date.getMonth() <= 0 ||t_date.getMonth() > 12) return false;
    if (t_date.getDay()  <= 0) return false;
    else if ((t_date.getMonth()%2 == 1 && t_date.getMonth() < 8) || (t_date.getMonth() >= 8 && t_date.getMonth()%2 == 0)) {
        if (t_date.getDay() > 31) return false;
    }
    else if (t_date.getMonth() != 2) {
        if (t_date.getDay() > 30) return false;
    }
    else if ((t_date.getYear()%4 == 0 && t_date.getYear()%100 != 0 )|| t_date.getYear()%400 == 0) {
        if (t_date.getDay() > 29) return false;
    } else {
        if (t_date.getDay() > 28) return false;
    }
    if (t_date.getHour() < 0 || t_date.getHour() >= 24) return false;
    if (t_date.getMinute()  < 0 || t_date.getMinute()>= 60) return false;
    return true;
}

/**
* @brief convert a string to date, if the format is not correct return
* 0000-00-00/00:00
* @return a date
*/
Date Date::stringToDate(const std::string dateString) {
    Date d;
    if (isValids(dateString)) {
    string a[5];
    a[0] = dateString.substr(0, 4);
    a[1] = dateString.substr(5, 2);
    a[2] = dateString.substr(8, 2);
    a[3] = dateString.substr(11, 2);
    a[4] = dateString.substr(14, 2);
    int b[5];
    for (int i = 0; i < 5; i++) {
        stringstream s;
        s << a[i];
        s >> b[i];
    }
    Date temp(b[0], b[1], b[2], b[3], b[4]);
    d = temp;
} else {
    Date temp(0,0,0,0,0);
    d = temp;
}
    return d;
}

/**
* @brief convert a date to string, if the format is not correct return
* 0000-00-00/00:00
*/
string toSTR1(const int a) {
    stringstream ss;
    ss << a;
    string s = ss.str();
    while (s.length() < 2) {
        s = "0" + s;
    }
    return s;
}
std::string Date::dateToString(Date t_date) {
    string s;
    if (!isValid(t_date)) {
        s = "0000-00-00/00:00";
    } else {
        stringstream ss;
        ss << t_date.getYear();
		string year = ss.str();
		while (year.length() < 4) {
        	year = "0" + year;
    	}
        s = year + "-" + toSTR1(t_date.getMonth()) + "-" + toSTR1(t_date.getDay()) + "/" + toSTR1(t_date.getHour()) + ":" + toSTR1(t_date.getMinute());
    }
    return s;
}

/**
*  @brief overload the assign operator
*/
Date &Date::operator=(const Date &t_date) {
    m_year = t_date.m_year;
    m_month = t_date.m_month;
    m_day = t_date.m_day;
    m_hour = t_date.m_hour;
    m_minute = t_date.m_minute;
    return *this;
}

/**
* @brief check whether the CurrentDate is equal to the t_date
*/
bool Date::operator==(const Date &t_date) const {
    if (m_year == t_date.m_year &&
    m_month == t_date.m_month &&
    m_day == t_date.m_day &&
    m_hour == t_date.m_hour &&
    m_minute == t_date.m_minute) return true;
    return false;
}

/**
* @brief check whether the CurrentDate is  greater than the t_date
*/
bool Date::operator>(const Date &t_date) const {
    if (m_year > t_date.m_year) return true;
    if (m_year < t_date.m_year) return false;
    if (m_month > t_date.m_month) return true;
    if (m_month < t_date.m_month) return false;
    if (m_day > t_date.m_day) return true;
    if (m_day < t_date.m_day) return false;
    if (m_hour > t_date.m_hour) return true;
    if (m_hour < t_date.m_hour) return false;
    if (m_minute > t_date.m_minute) return true;
    if (m_minute < t_date.m_minute) return false;
    return false;
}

/**
* @brief check whether the CurrentDate is  less than the t_date
*/
bool Date::operator<(const Date &t_date) const {
    if (m_year < t_date.m_year) return true;
    if (m_year > t_date.m_year) return false;
    if (m_month < t_date.m_month) return true;
    if (m_month > t_date.m_month) return false;
    if (m_day < t_date.m_day) return true;
    if (m_day > t_date.m_day) return false;
    if (m_hour < t_date.m_hour) return true;
    if (m_hour > t_date.m_hour) return false;
    if (m_minute < t_date.m_minute) return true;
    if (m_minute > t_date.m_minute) return false;
    return false;
}

/**
* @brief check whether the CurrentDate is  greater or equal than the t_date
*/
bool Date::operator>=(const Date &t_date) const {
    if (m_year > t_date.m_year) return true;
    if (m_year < t_date.m_year) return false;
    if (m_month > t_date.m_month) return true;
    if (m_month < t_date.m_month) return false;
    if (m_day > t_date.m_day) return true;
    if (m_day < t_date.m_day) return false;
    if (m_hour > t_date.m_hour) return true;
    if (m_hour < t_date.m_hour) return false;
    if (m_minute > t_date.m_minute) return true;
    if (m_minute < t_date.m_minute) return false;
    return true;
}

/**
* @brief check whether the CurrentDate is  less than or equal to the t_date
*/
bool Date::operator<=(const Date &t_date) const {
    if (m_year < t_date.m_year) return true;
    if (m_year > t_date.m_year) return false;
    if (m_month < t_date.m_month) return true;
    if (m_month > t_date.m_month) return false;
    if (m_day < t_date.m_day) return true;
    if (m_day > t_date.m_day) return false;
    if (m_hour < t_date.m_hour) return true;
    if (m_hour > t_date.m_hour) return false;
    if (m_minute < t_date.m_minute) return true;
    if (m_minute > t_date.m_minute) return false;
    return true;
}
