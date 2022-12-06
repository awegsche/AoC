//
// Created by andiw on 03/12/2022.
//

#ifndef AOCOBJECT__H
#define AOCOBJECT__H

#include <iostream>
#include <fstream>
#include <optional>

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
    };

} // namespace aoc

#endif