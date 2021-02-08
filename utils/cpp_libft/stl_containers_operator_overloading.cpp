//
// Created by airat on 08.02.2021.
//

#ifndef STL_CONTAINERS_OPERATOR_OVERLOADING_HPP
#define STL_CONTAINERS_OPERATOR_OVERLOADING_HPP

#include <iostream>
#include <list>

template <class T>
std::ostream & operator<<(std::ostream & output, std::list<T> const & rhs) {
    output << "[ ";
    for (typename std::list<T>::const_iterator it = rhs.begin(); it != rhs.end(); ++it) {
        output << *it << "; ";
    }
    output << " ]" << std::endl;
    return output;
}

#endif