//
// Created by airat on 10.02.2021.
//

#ifndef WEBSERV_PAIR_HPP
#define WEBSERV_PAIR_HPP

template <class First, class Second>
class Pair {
public:
    Pair<First, Second>(const First& first, const Second& second)
            : first(first), second(second)  {
    }


    First first;
    Second second;

};

#endif //WEBSERV_PAIR_HPP
