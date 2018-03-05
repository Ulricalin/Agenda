#include "AgendaUI.hpp"
#include <iomanip>
#include <sstream>

using namespace std;
using std::string;
AgendaUI::AgendaUI() {
    startAgenda();
}
void AgendaUI::OperationLoop(void) {
    char c;
    do {
    cout << "-------------------------- Agenda --------------------------" << endl << endl;
    cout << "Action  :" << endl;
    cout << "l      -  log in Agenda by user name and password" << endl;
    cout << "r      -  register an Agenda account" << endl;
    cout << "q      -  quit Agenda" << endl << endl;
    cout << "------------------------------------------------------------" << endl << endl;
    cout << "Agenda : ~$";
    cin >> c;
    try {
        switch(c) {
            case 'l':
                userLogIn();
                break;
            case 'r':
                userRegister();
                break;
            case 'q':
                quitAgenda();
                break;
            default :
                break;
        }
    } catch (my_exception & e) {
        cout << e.what() << endl;
    } catch (char a) {
        c = 'a';
    }
    } while (c != 'q');
}



/**
 * constructor
 */
void AgendaUI::startAgenda(void) {
    m_agendaService.startAgenda();
}

/**
 * catch user's operation
 * @return the operation
 */
std::string AgendaUI::getOperation() {
    string s;
	cout << endl << "Agenda@" << m_userName << " :~# ";
	cin >> s;
	while (s == "help") {
    cout << "-------------------------- Agenda --------------------------" << endl << endl
         << "Action  :" << endl
         << "o      -  log out Agenda" << endl
         << "dc     -  delete Agenda user" << endl
         << "lu     -  list all Agenda user" << endl
         << "cm     -  create a meeting" << endl
         << "la     -  list all meetings" << endl
         << "las    -  list all sponsor meetings" << endl
         << "lap    -  list all participate meetings" << endl
         << "qm     -  query meeting by title" << endl
         << "qt     -  query meeting by time interval" << endl
         << "dm     -  delete meeting by title" << endl
         << "da     -  delete all meetings" << endl
         << "reset  -  reset password, email, phone" << endl
		 << "help   -  action help "<< endl
         << "------------------------------------------------------------" << endl << endl;
	cout << "Agenda@" << m_userName << " :~# ";
	cin >> s;
	}
    return s;
}

/**
 * execute the operation
 * @return if the operationloop continue
 */
bool AgendaUI::executeOperation(std::string t_operation) {
    try {
    if (t_operation == "o") {
        userLogOut();
        return false;
    }
    else if (t_operation == "dc") {
        deleteUser();
        return true;
    }
    else if (t_operation == "lu") {
        listAllUsers();
        return true;
    }
    else if (t_operation == "cm") {
        createMeeting();
        return true;
    }
    else if (t_operation == "la") {
        listAllMeetings();
        return true;
    }
    else if (t_operation == "las") {
        listAllSponsorMeetings();
        return true;
    }
    else if (t_operation == "lap") {
        listAllParticipateMeetings();
        return true;
    }
    else if (t_operation == "qm") {
        queryMeetingByTitle();
        return true;
    }
    else if (t_operation == "qt") {
        queryMeetingByTimeInterval();
        return true;
    }
    else if (t_operation == "dm") {
        deleteMeetingByTitle();
        return true;
    }
    else if (t_operation == "da") {
        deleteAllMeetings();
        return true;
    }
    else if (t_operation == "reset") {
        Reset();
        return true;
    }
    } catch (my_exception & e) {
        cout << e.what() << endl;
    } catch (char a) {
    } catch (int a) {
        return false;
    }
    return true;
}

/**
 * user Login
 */
void AgendaUI::userLogIn(void) {
    cout << endl;
    cout << "[log in] [username] [password]" << endl;
    cout << "[log in] ";
    std::string username, password;
    cin >> username >> password;
    if (m_agendaService.userLogIn(username, password)) {
        cout << "[log in] succeed!" << endl;
        m_userName = username;
        m_userPassword = password;
	cout << "-------------------------- Agenda --------------------------" << endl << endl
         << "Action  :" << endl
         << "o      -  log out Agenda" << endl
         << "dc     -  delete Agenda user" << endl
         << "lu     -  list all Agenda user" << endl
         << "cm     -  create a meeting" << endl
         << "la     -  list all meetings" << endl
         << "las    -  list all sponsor meetings" << endl
         << "lap    -  list all participate meetings" << endl
         << "qm     -  query meeting by title" << endl
         << "qt     -  query meeting by time interval" << endl
         << "dm     -  delete meeting by title" << endl
         << "da     -  delete all meetings" << endl
         << "reset  -  reset password, email, phone" << endl
		 << "help   -  action help "<< endl
         << "------------------------------------------------------------" << endl << endl;
        while (executeOperation(getOperation()));
    } else {
        cout << "[error] log in fail!" << endl;
    }
}

/**
 * user regist
 */
void AgendaUI::userRegister(void) {
    cout << endl;
    cout << "[register] [username] [password] [email] [phone]" << endl;
    cout << "[register] ";
    string username, password, email, phone;
    cin >> username >> password >> email >> phone;
    if (m_agendaService.userRegister(username, password, email, phone)) {
        cout << "[register] succeed!" << endl;
    } else {
        cout << "register fail!" << endl;
    }
}

/**
 * quit the Agenda
 */
void AgendaUI::quitAgenda(void) {
    char a;
    cout << "[Attention] Do you want to quit Agenda ?  y / n   ";
    cin >> a;
    if (a == 'n') throw a;
    m_agendaService.quitAgenda();
}

/**
 * user logout
 */
void AgendaUI::userLogOut(void) {
    char a;
    cout << "[Attention] Do you want to logout ?  y / n   ";
    cin >> a;
    if (a == 'n') throw a;
    m_userName.clear();
    m_userPassword.clear();
    m_agendaService.quitAgenda();
}

/**
 * delete a user from storage
 */
void AgendaUI::deleteUser(void) {
	cout << "[please enter your paasword] [password]" << endl;
	cout << "[please enter your paasword] ";
	string pass;
	cin >> pass;
	if (pass != m_userPassword) {
		throw PasswordIncorrect();
	}
    if (m_agendaService.deleteUser(m_userName, m_userPassword)) {
        cout << endl << "[delete agenda account] succeed!" << endl;
        m_userName.clear();
        m_userPassword.clear();
        int i;
        throw i;
    }
}

/**
 * list all users from storage
 */
void AgendaUI::listAllUsers(void) {
    std::list<User> list = m_agendaService.listAllUsers();
    cout << endl << "[list all users] " << endl << endl;
	cout << std::left << setw(20) << "Name"
        << std::left << setw(30) << "Email" << "Phone" << endl;
    for (auto p = list.begin(); p != list.end(); p++) {
        string n = p->getName(), e = p->getEmail();
        if (n.length() > 17) n = n.substr(0, 17) + "..";
        if (e.length() > 27) e = e.substr(0, 27) + "..";
        cout << std::left;
	cout << std::left << setw(20) << n
        << std::left << setw(30) << e << p->getPhone() << endl;
    }

}

/**
 * user create a meeting with someone else
 */
void AgendaUI::createMeeting(void) {
	string isnum;
	bool flag = false;
    int num;
    string startdate, enddate, title, ss;
    std::vector<std::string> participator;
    cout << endl;
    do {
	flag = false;
    cout << "[create meeting] [the number of participators]" << endl;
    cout << "[create meeting] ";
    cin >> isnum;
	for (int i = 0; i < isnum.length(); i++) {
		if (isnum[i] < '0' || isnum[i] > '9') {
			flag = true;
			break;
		}
	}
	} while (flag);
	stringstream stream;
	stream << isnum;
	stream >> num;
    for (int i = 1; i <= num; i++) {
        cout << "[create meeting] [please enter the participator " << i
            << "]" << endl;
        cout << "[create meeting] ";
        cin >> ss;
	participator.push_back(ss);
    }
    cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] \
[end time(yyyy-mm-dd/hh:mm)] " << endl;
	cout << "[create meeting] ";
    cin >> title >> startdate >> enddate;
    if (m_agendaService.createMeeting(m_userName, title, startdate, enddate, participator)) {
        cout << "[create meeting] succeed!" << endl;
    } else {
        cout << "[create meeting] error!" << endl;
    }
}

/**
 * list all meetings from storage
 */
void AgendaUI::listAllMeetings(void) {
    std::list<Meeting> list = m_agendaService.listAllMeetings(m_userName);
    cout << endl << "[list all meetings] " << endl;
    printMeetings(list);
}

/**
 * list all meetings that this user sponsored
 */
void AgendaUI::listAllSponsorMeetings(void) {
    std::list<Meeting> list = m_agendaService.listAllSponsorMeetings(m_userName);
    cout << endl << "[list all sponsor meetings] " << endl;
    printMeetings(list);
}

/**
 * list all meetings that this user take part in
 */
void AgendaUI::listAllParticipateMeetings(void) {
    std::list<Meeting> list = m_agendaService.listAllParticipateMeetings(m_userName);
    cout << endl << "[list all participator meetings] " << endl;
    printMeetings(list);
}

/**
 * search meetings by title from storage
 */
void AgendaUI::queryMeetingByTitle(void) {
    cout << endl << "[query meeting] [title]:" << endl;
    cout << "[query meeting] ";
    string meeting;
    cin >> meeting;
    std::list<Meeting> list = m_agendaService.meetingQuery(m_userName, meeting);
    printMeetings(list);
}

/**
 * search meetings by timeinterval from storage
 */
void AgendaUI::queryMeetingByTimeInterval(void) {
    cout << endl << "[query meeting] [start time(yyyy-mm-dd/hh:mm)] \
[end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[query meeting] ";
    string startdate, enddate;
    cin >> startdate >> enddate;
    cout << "[query meeting] " << endl;
    std::list<Meeting> list = m_agendaService.meetingQuery(m_userName, startdate, enddate);
    printMeetings(list);
}

/**
 * delete meetings by title from storage
 */
void AgendaUI::deleteMeetingByTitle(void) {
    cout << endl << "[delete meeting] [titile]" << endl;
    cout << "[delete meeting] ";
    string meeting;
    cin >> meeting;
    if (m_agendaService.deleteMeeting(m_userName, meeting)) {
        cout << "[delete meeting by title] succeed!" << endl;
    } else {
        cout << "[error] delete meeting fail!" << endl;
    }
}

/**
 * delete all meetings that this user sponsored
 */
void AgendaUI::deleteAllMeetings(void) {
	cout << "[please enter your paasword] [password]" << endl;
    cout << "[please enter your paasword] ";
    string pass;
    cin >> pass;
    if (pass != m_userPassword) {
        throw PasswordIncorrect();
    }
    m_agendaService.deleteAllMeetings(m_userName);
    cout << "[delete all meeting] succeed!" << endl;
}
void AgendaUI::Reset(void) {
    string s;
    do {
    cout << "---------------------- Reset -------------------------" << endl;
    cout << "pa      -----   Reset password" << endl
         << "e       -----   Reset email" << endl
         << "ph      -----   Reset phone" << endl
         << "q       -----   quit reset" << endl
         << "------------------------------------------------------" << endl;
    cout << "Agenda@" << m_userName << " :~# ";
    cin >> s;
    if (s == "pa") {
        cout << "[please enter your old paasword] [password]" << endl;
        cout << "[please enter your old paasword] ";
        string pass;
        cin >> pass;
        if (pass != m_userPassword) {
            throw PasswordIncorrect();
        } else {
            cout << "[please enter your new paasword] [password]" << endl;
            cout << "[please enter your new paasword] ";
            cin >> m_userPassword;
            m_agendaService.ResetPassword(m_userName, m_userPassword);
        }
    }
    else if (s == "e") {
        cout << "[please enter your new email] [email]" << endl;
        cout << "[please enter your new email] ";
        string email;
        cin >> email;
        m_agendaService.ResetEmail(m_userName, email);
    }
    else if (s == "ph") {
        cout << "[please enter your new phone] [phone]" << endl;
        cout << "[please enter your new phone] ";
        string phone;
        cin >> phone;
        m_agendaService.ResetEmail(m_userName, phone);
    }
    else if (s == "q") {
        cout << "[Attention] Do you want to quit? y / n  ";
        cin >> s;
        if (s == "y") s = "q";
    }
    } while (s != "q");
    
}

/**
 * show the meetings in the screen
 */
void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {
    cout << endl;
	cout << std::left << setw(30) << "Title" << std::left << setw(30) << "Sponsor" << std::left << setw(30) << "Start time"
        << std::left << setw(30) << "End time" << "participators" << endl;
    for (auto p = t_meetings.begin(); p != t_meetings.end(); p++) {
	string s[4];
    s[0] = p->getTitle();
    s[1] = p->getSponsor();
    s[2] = Date::dateToString(p->getStartDate());
    s[3] = Date::dateToString(p->getEndDate());
    for (int i = 0; i < 4; i++) {
        if (s[i].length() > 25) {
            s[i] = s[i].substr(0, 25)+"..";
        }
            cout << std::left << setw(30) << s[i];
    }
        std::vector<std::string> par = p->getParticipator();
        int i = 0;
        for (i = 0; i < par.size()-1; i++) {
            cout << par[i] << ",";
        }
        cout << par[i] << endl;
    }
}
