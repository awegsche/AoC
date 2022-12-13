//
// Created by andiw on 07/12/2022.
//

#ifndef AOC_DAY7_H
#define AOC_DAY7_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <set>

#include <AocLines.h>
#include <AocObject.h>
#include <AocDay.h>

using std::cout, std::cerr, std::endl;

constexpr int LIMIT     = 100000;
constexpr int CAPACITY  = 70000000;
constexpr int NEEDED    = 30000000;

struct File {
    std::string name;
    int size;
};

struct Dir {
    std::string name;
    std::vector<std::shared_ptr<Dir>> dirs;
    std::vector<File> files;
    std::shared_ptr<Dir> parent;

    explicit Dir(std::string name, std::shared_ptr<Dir> parent = nullptr)
    : name(std::move(name)), parent(std::move(parent))
    {}

    [[nodiscard]] int size() const {
        int total_size = std::accumulate(files.begin(), files.end(),
                                         0,
                                         [](int s, File const& a) { return a.size + s; });

        return std::accumulate(dirs.begin(),
                               dirs.end(),
                               total_size,
                               [](int s, std::shared_ptr<Dir> const& a){ return a->size() + s; });
    }

    bool operator==(std::string const& other) const {
        return name == other;
    }
};

int flatten_and_sort(std::shared_ptr<Dir> const& parent, std::multiset<int>& candidates) {
    int total_size = 0;
    for (auto const& dir: parent->dirs) {
        int size = flatten_and_sort(dir, candidates);
        candidates.insert(size);
        total_size += size;
    }
    for (auto const& file: parent->files)
        total_size += file.size;
    return total_size;
}

class Day7 : public AocObject<Day7>, public AocDay<Day7, int> {
    std::shared_ptr<Dir> root;

public:

    static constexpr char FILENAME[] = "7";
    static constexpr char YEAR[] = "2022";
    static constexpr char TITLE[] = "No Space Left On Device";

    Day7(): root(std::make_shared<Dir>("/")) {}

    static auto get_object(std::istream &stream, Day7 &day) -> bool {
        day.root = std::make_shared<Dir>("/");
        std::string line{};

        auto current_dir = day.root;

        aoc::getline(stream, line); // skip first cd, we are already in "/"
        aoc::getline(stream, line);
        do {
            if (line == "$ ls") {
                aoc::getline(stream, line);
                while (!line.starts_with("$")) {
                    if (line.starts_with("dir ")) {
                        current_dir->dirs.emplace_back(std::make_shared<Dir>(line.substr(4), current_dir));
                    }
                    else {
                        size_t pos = 0;
                        int size = std::stoi(line, &pos);
                        current_dir->files.push_back({line.substr(pos), size});
                    }
                    if (!aoc::getline(stream, line)) return true;
                }
            }
            else if (line.starts_with("$ cd")) {
                auto target = line.substr(5);

                if (target == "..") {
                    current_dir = current_dir->parent;
                }
                else {
                    auto found = std::find_if(current_dir->dirs.begin(), current_dir->dirs.end(),
                                              [&target](std::shared_ptr<Dir> const& dir) { return dir->name == target; }
                    );
                    if (found == current_dir->dirs.end()) {
                        cerr << "couldn't find subdir \"" << target << "\" in \"" << current_dir->name << "\"" << endl;
                        return false;
                    }
                    current_dir = *found;
                }
                if (!aoc::getline(stream, line)) return true;
            }
            else {
                cerr << "unknown command \"" << line << "\"" <<endl;
                return false;
            }
        } while (true);
    }

    int part1() const override {
        std::multiset<int> candidates;
        flatten_and_sort(root, candidates);

        int sum = 0;
        for (int c: candidates) {
            if (c > LIMIT) break;
                sum += c;
        }

        return sum;
    }

    int part2() const override {
        int to_free = NEEDED - CAPACITY + root->size();

        std::multiset<int> candidates;
        flatten_and_sort(root, candidates);

        for (int c: candidates)
            if (c >= to_free)
                return c;
        return -1;
    }
};

#endif //AOC_DAY7_H
