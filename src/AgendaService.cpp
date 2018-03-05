#include "AgendaService.hpp"
#include <iostream>
using namespace std;
AgendaService::AgendaService() {
    startAgenda();
}

/**
 * destructor
 */
AgendaService::~AgendaService() {
    quitAgenda();
}

/**
 * check if the username match password
 * @param userName the username want to login
 * @param password the password user enter
 * @return if success, true will be returned
 */
bool AgendaService::userLogIn(const std::string userName, const std::string password) {
     auto filter = [userName, password] (const User & user) {
        return (user.getName() == userName && user.getPassword() == password);
     };
     if (m_storage->queryUser(filter).empty()) throw PasswordIncorrect();
     return true;
}

/**
 * regist a user
 * @param userName new user's username
 * @param password new user's password
 * @param email new user's email
 * @param phone new user's phone
 * @return if success, true will be returned
 */
bool AgendaService::userRegister(const std::string userName, const std::string password,
                  const std::string email, const std::string phone) {
    auto filter = [userName] (const User & user) {
        return user.getName() == userName;
     };
     if (!(m_storage->queryUser(filter)).empty()) {
        throw Userexisted();
    } else {
        m_storage->createUser(User(userName, password, email, phone));
        return true;
    }
}

/**
 * delete a user
 * @param userName user's username
 * @param password user's password
 * @return if success, true will be returned
 */
bool AgendaService::deleteUser(const std::string userName, const std::string password) {
    auto filter = [userName, password] (const User & user) {
        return (user.getName() == userName && user.getPassword() == password);
     };
    if (m_storage->deleteUser(filter) > 0) {
        auto filter1 = [userName] (const Meeting & meeting) {
            if (userName == meeting.getSponsor()) return true;
            std::vector<std::string> par = meeting.getParticipator();
            for (int i = 0; i < par.size(); i++) {
                if (userName == par[i]) return true;
            }
            return false;
        };
        m_storage->deleteMeeting(filter1);
        return true;
    } else {
        throw Usernotexisted();
    }
    return false;
}

/**
 * list all users from storage
 * @return a user list result
 */
std::list<User> AgendaService::listAllUsers(void) const {
    auto filter = [] (const User & user) {
        return true;
     };
     return m_storage->queryUser(filter);
}

/**
 * create a meeting
 * @param userName the sponsor's userName
 * @param title the meeting's title
 * @param participator the meeting's participator
 * @param startData the meeting's start date
 * @param endData the meeting's end date
 * @return if success, true will be returned
 */
bool AgendaService::createMeeting(const std::string userName, const std::string title,
                   const std::string startDate, const std::string endDate,
                   const std::vector<std::string> participator) {
    Date startdate = Date::stringToDate(startDate), enddate = Date::stringToDate(endDate);
    if (!Date::isValid(startdate) || !Date::isValid(enddate)) throw DateInvalid();
    if (startdate >= enddate) throw MeetingTimeInvalid();
	if (participator.empty()) throw WithoutParticipator();
    auto filter0 = [userName] (const User & user) {
            return user.getName() == userName;
        };
    if ((m_storage->queryUser(filter0)).empty()) throw Usernotexisted();
    for (int i = 0; i < participator.size()-1; i++) {
        for (int j = i+1; j < participator.size(); j++) {
            if (participator[i] == participator[j]) throw InvalidParticipators();
        }
    }
	for (int i = 0; i < participator.size(); i++) {
		if (userName == participator[i]) throw InvalidParticipators();
	}
    auto filter2 = [participator] (const User & user) ->bool {
		for (int i = 0; i < participator.size(); i++) {
            if (user.getName() == participator[i]) {
                return true;
            }
		}
		return false;
    };
    if ((m_storage->queryUser(filter2)).size() < participator.size()) throw Usernotexisted();
    auto filter1 = [userName, startdate, enddate, title, participator] (const Meeting & meeting) {
        if (meeting.getTitle() == title) throw Meetingtitleexist();
        if (!(meeting.getStartDate() >= enddate) && !(meeting.getEndDate() <= startdate)) {
            if (userName == meeting.getSponsor()) throw Meetingoverlap();
			std::vector<std::string> par = meeting.getParticipator();
            for (int i = 0; i < par.size(); i++) {
                if (userName == par[i]) throw Meetingoverlap();
            }
            for (int i = 0; i < participator.size(); i++) {
                if (participator[i] == meeting.getSponsor()) throw Meetingoverlap();
                for (int j = 0; j < par.size(); j++) {
                    if (par[j] == participator[i]) throw Meetingoverlap();
                }
            }
        }
        return false; 
    };
    if ((m_storage->queryMeeting(filter1)).empty()) {
        m_storage->createMeeting(Meeting(userName, participator, startdate, enddate, title));
        return true;
    } else {
        return false;
    }
    
}

/**
 * search meetings by username and title (user as sponsor or participator)
 * @param uesrName the user's userName
 * @param title the meeting's title
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
                                const std::string title) const {
    auto filter = [userName, title] (const Meeting & meeting) {
        if (title == meeting.getTitle()) {
            if (userName == meeting.getSponsor()) return true;
		std::vector<std::string> par = meeting.getParticipator();
            for (int i = 0; i < par.size(); i++) {
                if (userName == par[i]) return true;
            }
        }
        return false;
    };
    return m_storage->queryMeeting(filter);
}
/**
 * search a meeting by username, time interval (user as sponsor or participator)
 * @param uesrName the user's userName
 * @param startDate time interval's start date
 * @param endDate time interval's end date
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
                                const std::string startDate,
                                const std::string endDate) const {
    std::list<Meeting> l;
    Date startdate = Date::stringToDate(startDate), enddate = Date::stringToDate(endDate);
    if (!Date::isValid(startdate) || !Date::isValid(enddate) ||
        startdate > enddate) {
        return l;
    }
    auto filter = [userName, startdate, enddate] (const Meeting & meeting) {
        if (!(meeting.getStartDate() > enddate) && !(meeting.getEndDate() < startdate)) {
            if (userName == meeting.getSponsor()) return true;
            std::vector<std::string> par = meeting.getParticipator();
            for (int i = 0; i < par.size(); i++) {
                if (userName == par[i]) return true;
            }
            return false;
        }
		return false;
    };
    return m_storage->queryMeeting(filter);
}

/**
 * list all meetings the user take part in
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllMeetings(const std::string userName) const {
    auto filter = [userName] (const Meeting & meeting) {
        if (userName == meeting.getSponsor()) return true;
        std::vector<std::string> par = meeting.getParticipator();
            for (int i = 0; i < par.size(); i++) {
                if (userName == par[i]) return true;
            }
        return false;
    };
    return m_storage->queryMeeting(filter);
}

/**
 * list all meetings the user sponsor
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string userName) const {
    auto filter = [userName] (const Meeting & meeting) {
        return userName == meeting.getSponsor();
    };
    return m_storage->queryMeeting(filter);
}

/**
 * list all meetings the user take part in and sponsor by other
 * @param userName user's username
 * @return a meeting list result
 */
std::list<Meeting> AgendaService::listAllParticipateMeetings(
    const std::string userName) const {
    auto filter = [userName] (const Meeting & meeting) {
        std::vector<std::string> par = meeting.getParticipator();
            for (int i = 0; i < par.size(); i++) {
                if (userName == par[i]) return true;
            }
        return false;
    };
    return m_storage->queryMeeting(filter);
}

/**
 * delete a meeting by title and its sponsor
 * @param userName sponsor's username
 * @param title meeting's title
 * @return if success, true will be returned
 */
bool AgendaService::deleteMeeting(const std::string userName, const std::string title) {
    auto filter = [userName, title] (const Meeting & meeting) {
        if (title == meeting.getTitle() && userName == meeting.getSponsor()) return true;
        else return false;
    };
    if (m_storage->deleteMeeting(filter) > 0) {
        return true;
    } else {
        throw Meetingnotexisted();
    }
}

/**
 * delete all meetings by sponsor
 * @param userName sponsor's username
 * @return if success, true will be returned
 */
bool AgendaService::deleteAllMeetings(const std::string userName) {
    auto filter = [userName] (const Meeting & meeting) {
        if (userName == meeting.getSponsor()) return true;
        else return false;
    };
    return m_storage->deleteMeeting(filter) > 0;
}

/**
 * start Agenda service and connect to storage
 */
void AgendaService::startAgenda(void) {
    m_storage = Storage::getInstance();
}

/**
 * quit Agenda service
 */
void AgendaService::quitAgenda(void) {
    m_storage->sync();
}

bool AgendaService::ResetPassword(const string username, const string password) {
    auto filter = [username] (const User & user) {
        if (username == user.getName()) return true;
        else return false;
    };
    auto switcher = [&] (User & user) -> void {
        user.setPassword(password);
    };
    return m_storage->updateUser(filter, switcher);
}

bool AgendaService::ResetEmail(const string username, const string email) {
    auto filter = [username] (const User & user) {
        if (username == user.getName()) return true;
        else return false;
    };
    auto switcher = [&] (User & user) -> void{
        user.setEmail(email);
    };
    return m_storage->updateUser(filter, switcher);
}

bool AgendaService::ResetPhone(const string username, const string phone) {
    auto filter = [username] (const User & user) {
        if (username == user.getName()) return true;
        else return false;
    };
    auto switcher = [&] (User & user) -> void{
        user.setPhone(phone);
    };
    return m_storage->updateUser(filter, switcher);
}