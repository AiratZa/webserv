#ifndef PARSE_REQUEST_HANDLER
# define PARSE_REQUEST_HANDLER

#include "AbstractRequestHandler.hpp"

class ParseRequestHandler : public AbstractRequestHandler {
 public:
  std::string Handle(std::string request) {
    if (request == "MeatBall") {
      return "Dog: I'll eat the " + request + ".\n";
    } else {
      return AbstractRequestHandler::Handle(request);
    }
  }
};


#endif