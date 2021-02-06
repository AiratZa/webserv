#ifndef HEADER_HANDLER_HPP
# define HEADER_HANDLER_HPP

#include <string>

/**
 * Интерфейс Обработчика объявляет метод построения цепочки обработчиков. Он
 * также объявляет метод для выполнения запроса.
 */
class HeaderHandler {
 public:
  virtual HeaderHandler *SetNext(HeaderHandler *handler) = 0;
  virtual std::string Handle(std::string request) = 0;
};

#endif