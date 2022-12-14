//
// Created by andiw on 03/12/2022.
//

#ifndef AOCOBJECT__H
#define AOCOBJECT__H

#include <iostream>
#include <fstream>
#include <optional>
#include <sstream>

namespace aoc {

    template<typename Object>
    class AocObject {
    public:
        static auto from_istream(std::istream &stream) -> std::optional<Object> {
            Object obj{};
            if (Object::get_object(stream, obj))
                return obj;
            return {};
        }

        /// Constructs the Object from a string
        ///
        /// Example:
        /// ```
        /// auto obj = Object::from_string("[1,1,1]");
        /// ```
        static auto from_string(std::string const& s) -> std::optional<Object> {
            std::stringstream ss{s};
            return from_istream(ss);
        }
    };

} // namespace aoc

#endif
