#ifndef Exception_HPP
#define Exception_HPP

#include <string>
using std::string;
class my_exception {
	public:
		virtual const string what() {
			return "Exception occurred";
		}
};
class UserException : public my_exception {
public:
	virtual const string what() {
		return "[error] : Exception about user occurred!";
	}
};
class MeetingException : public my_exception {
public:
	virtual const string what() {
		return "[error] : Exception about meeting occurred!";
	}
};
class DateException : public my_exception {
public:
	virtual const string what() {
		return "[error] : Exception about Date occurred!";
	}
};

class PasswordIncorrect : public UserException {
public:
	virtual const string what() {
		return "[error] : Password Incorrect!";
	}
};
class Userexisted : public UserException {
public:
	virtual const string what() {
		return "[error] : User is existed!";
	}
};
class Usernotexisted : public UserException {
public:
	virtual const string what() {
		return "[error] : Inputed a person who is not a user!";
	}
};
class DateInvalid : public DateException {
public:
	virtual const string what() {
		return "[error] : Date Invalid!";
	}
};
class Meetingoverlap: public MeetingException {
public:
	virtual const string what() {
		return "[error] : Meetingoverlap!";
	}
};

class Meetingtitleexist : public MeetingException {
public:
	virtual const string what() {
		return "[error] : Meetingtitle is existed!";
	}
};
class MeetingTimeInvalid : public MeetingException {
public:
	virtual const string what() {
		return "[error] : MeetingTimeInvalid (Startdate must be earlier than enddate)!";
	}
};
class WithoutParticipator : public MeetingException {
public:
	virtual const string what() {
		return "[error] : Without Participator!";
	}
};
class InvalidParticipators : public MeetingException {
public:
	virtual const string what() {
		return "[error] : InvalidParticipators!(maybe there are two Participators or sponsor with the same name";
	}
};
class Meetingnotexisted : public MeetingException {
public:
	virtual const string what() {
		return "[error] : Meetingtitle is not existed!";
	}
};

#endif