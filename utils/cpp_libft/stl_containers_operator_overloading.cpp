//
// Created by airat on 08.02.2021.
//

#ifndef STL_CONTAINERS_OPERATOR_OVERLOADING_HPP
#define STL_CONTAINERS_OPERATOR_OVERLOADING_HPP

#include <iostream>
#include <list>
#include <map>

template <class T>
std::ostream & operator<<(std::ostream & output, std::list<T> const & rhs) {
    output << "[ ";
    for (typename std::list<T>::const_iterator it = rhs.begin(); it != rhs.end(); ++it) {
        output << *it << "; ";
    }
    output << " ]" << std::endl;
    return output;
}

template <class Key, class ValueType>
std::ostream & operator<<(std::ostream & output, std::map<Key, std::list<ValueType> > const & rhs) {
    typename std::map<Key, std::list<ValueType> >::const_iterator map_it = rhs.begin();
    typename std::map<Key, std::list<ValueType> >::const_iterator map_ite = rhs.end();

    output << "{ ";
    while (map_it != map_ite) {
        output << "\"" << (*map_it).first <<  "\": " << (*map_it).second;
        ++map_it;
    }
    output << "}" << std::endl;
    return output;
}

#endif