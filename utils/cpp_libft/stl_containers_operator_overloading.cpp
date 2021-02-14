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

template <class Key_1, class Key_2, class Value_2>
std::ostream & operator<<(std::ostream & output, std::map<Key_1, std::map<Key_2, Value_2> > const & rhs) {
    typename std::map<Key_1, std::map<Key_2, Value_2> >::const_iterator map_it = rhs.begin();
    typename std::map<Key_1, std::map<Key_2, Value_2> >::const_iterator map_ite = rhs.end();

    output << "{ ";
    while (map_it != map_ite) {
        output << "\"" << (*map_it).first <<  "\": ";

        typename std::map<Key_2, Value_2>::const_iterator map_2_it = (*map_it).second.begin();
        typename std::map<Key_2, Value_2>::const_iterator map_2_ite = (*map_it).second.end();
        output << "{ ";
        while (map_2_it != map_2_ite) {
            output << "\"" << (*map_2_it).first << "\": " << "\"" <<  (*map_2_it).second << "\"";
            output<< "; ";
            ++map_2_it;
        }
        output << "}" << std::endl;

        ++map_it;
    }
    output << "}" << std::endl;
    return output;
}


#endif