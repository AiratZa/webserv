#ifndef CONTENT_LENGTH_HANDLER_HPP
# define CONTENT_LENGTH_HANDLER_HPP

#include "AbstractRequestHandler.hpp"

class ContentLengthHandler : public AbstractRequestHandler {
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