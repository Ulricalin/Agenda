#include "Storage.hpp"
#include "Path.hpp"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
using std::string;
using std::cin;
using std::cout;
using std::endl;
std::shared_ptr<Storage> Storage::m_instance = NULL;
Storage::Storage() {
    readFromFile();
    m_dirty = false;
}

/**
*   read file content into memory
*   @return if success, true will be returned
*/
bool Storage::readFromFile(void) {
    ifstream fin(Path::userPath);
    if (!fin.is_open()) return false;
    string line;
    while (getline(fin, line)) {
    if (line.length() == 0) break;
        string a[4];
        int index = 0, low = 1, i = 0;
        while (index != line.npos) {
            index = line.find("\",\"", index+1);
            if (index == line.npos) break;
            a[i] = line.substr(low, index-low);
            low = index+3;
            i++;
        }
        a[i] = line.substr(low, line.length()-1-low);
        m_userList.push_back(User(a[0], a[1], a[2], a[3]));
    }
    fin.close();
    fin.open(Path::meetingPath);
    if (!fin.is_open()) return false;
    while (getline(fin, line)) {
    if (line.length() == 0) break;
        string a[5];
        int index = 0, low = 1, i = 0;
        while (index != line.npos) {
            index = line.find("\",\"", index+1);
            if (index == line.npos) break;
            a[i] = line.substr(low, index-low);
            low = index+3;
            i++;
        }
        a[i] = line.substr(low, line.length()-1-low);
        std::vector<std::string> par;
        index = low = 0;
        while (index != a[1].npos) {
            index = a[1].find("&", index+1);
            if (index == line.npos) break;
            par.push_back(a[1].substr(low, index-low));
            low = index+1;
        }
        par.push_back(a[1].substr(low, a[1].length()-low));
        m_meetingList.push_back(Meeting(a[0], par, Date(a[2]), Date(a[3]), a[4]));
    }
    fin.close();
    return true;
}

/**
*   write file content from memory
*   @return if success, true will be returned
*/
bool Storage::writeToFile(void) {
    if (!m_dirty) return true;
    ofstream fout(Path::userPath);
    if (!fout.is_open()) return false;

    for (auto p = m_userList.begin(); p != m_userList.end(); ++p) {
        fout << "\"" << p->getName() << "\",\"" << p->getPassword()
        << "\",\"" << p->getEmail() << "\",\"" << p->getPhone() << "\"" << endl;
    }
    fout.close();
    fout.open(Path::meetingPath);
    if (!fout.is_open()) return false;

    for (auto p = m_meetingList.begin(); p != m_meetingList.end(); ++p) {

    fout << "\"" << p->getSponsor() << "\",\"";
    std::vector<std::string> v = p->getParticipator();
    int i = 0;
    if (!v.empty()) {
        fout << v[i];
        for (i = 1; i < v.size(); i++) {
            fout << "&" << v[i];
        }
    }
    fout << "\",\"" << Date::dateToString(p->getStartDate())
        << "\",\"" << Date::dateToString(p->getEndDate()) << "\",\"" << p->getTitle()<< "\"" << endl;

    }
    fout.close();
    m_dirty = false;
    return true;
}


/**
* get Instance of storage
* @return the pointer of the instance
*/
std::shared_ptr<Storage> Storage::getInstance(void) {
    if (m_instance == NULL) m_instance =(std::shared_ptr<Storage>) new Storage();
    return m_instance;
}

/**
*   destructor
*/
Storage::~Storage() {
    writeToFile();
}

// CRUD for User & Meeting
// using C++11 Function Template and Lambda Expressions

/**
* create a user
* @param a user object
*/
void Storage::createUser(const User & t_user) {
    m_userList.push_back(t_user);
    m_dirty = true;
}

/**
* query users
* @param a lambda function as the filter
* @return a list of fitted users
*/
std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const {
    std::list<User> newlist;
    for (auto p = m_userList.begin(); p != m_userList.end(); ++p) {
        if (filter(*p)) newlist.push_back(*p);
    }
    return newlist;
}

/**
* update users
* @param a lambda function as the filter
* @param a lambda function as the method to update the user
* @return the number of updated users
*/
int Storage::updateUser(std::function<bool(const User &)> filter,
             std::function<void(User &)> switcher) {
    int num = 0;
    for (auto p = m_userList.begin(); p != m_userList.end(); ++p) {
        if (filter(*p)) {
            switcher(*p);
            num++;
        }
    }
    if (num > 0) m_dirty = true;
    return num;
}
/**
* delete users
* @param a lambda function as the filter
* @return the number of deleted users
*/
int Storage::deleteUser(std::function<bool(const User &)> filter) {
    int num = 0;
    for (auto p = m_userList.begin(); p != m_userList.end();) {
        if (filter(*p)) {
            p = m_userList.erase(p);
            num++;
        } else {
	        ++p;
		}
    }
    if (num > 0) m_dirty = true;
    return num;
}

/**
* create a meeting
* @param a meeting object
*/
void Storage::createMeeting(const Meeting & t_meeting) {
    m_meetingList.push_back(t_meeting);
    m_dirty = true;
}

/**
* query meetings
* @param a lambda function as the filter
* @return a list of fitted meetings 
*/
std::list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting &)> filter) const {
    std::list<Meeting> newlist;
    for (auto p = m_meetingList.begin(); p != m_meetingList.end(); ++p) {
        if (filter(*p)) newlist.push_back(*p);
    }
    return newlist;
}

/**
* update meetings
* @param a lambda function as the filter
* @param a lambda function as the method to update the meeting
* @return the number of updated meetings
*/
int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                std::function<void(Meeting &)> switcher) {
    int num = 0;
    for (auto p = m_meetingList.begin(); p != m_meetingList.end(); ++p) {
        if (filter(*p)) {
            switcher(*p);
            num++;
        }
    }
    if (num > 0) m_dirty = true;
    return num;
}

/**
* delete meetings
* @param a lambda function as the filter
* @return the number of deleted meetings
*/
int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
    int num = 0;
    for (auto p = m_meetingList.begin(); p != m_meetingList.end();) {
        if (filter(*p)) {
            p = m_meetingList.erase(p);
            num++;
        } else {
	    ++p;
	}
    }
    if (num > 0) m_dirty = true;
    return num;
}

/**
* sync with the file
*/
bool Storage::sync(void) {
    return writeToFile();
}
