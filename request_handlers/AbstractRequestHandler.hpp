#ifndef ABSTRACT_HEADER_HANDLER_HPP
# define ABSTRACT_HEADER_HANDLER_HPP

#include "HeaderHandler.hpp"

class AbstractRequestHandler : public HeaderHandler {
  /**
   * @var Handler
   */
 private:
  HeaderHandler *next_handler_;

 public:
  AbstractRequestHandler() : next_handler_(nullptr) {
  }
  HeaderHandler *SetNext(HeaderHandler *handler) {
    this->next_handler_ = handler;
    // Возврат обработчика отсюда позволит связать обработчики простым способом,
    // вот так:
    // $monkey->setNext($squirrel)->setNext($dog);
    return handler;
  }
  std::string Handle(std::string request) {
    if (this->next_handler_) {
      return this->next_handler_->Handle(request);
    }

    return std::string();
  }
};

#endif