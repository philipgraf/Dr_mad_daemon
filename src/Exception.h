#ifndef EXCEPTION_H_
#define EXCEPTION_H_

/**
 * Exception Class.
 * This Class will handle all exceptions soon
 * @author Philip Graf
 * @date 14.04.2013
 * @version 0.1.0 Alpha-State
 */
class Exception: public std::exception {
public:
	enum ExceptionType {
		UNKNOWN_EXCEPTION = 0,
		UNKNOWN_ERRORSTR
	};
	Exception();
	virtual ~Exception();

};

class FileOpenException: public Exception {

};

#endif// EXCEPTION_H_
