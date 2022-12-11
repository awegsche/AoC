#pragma once

#include <iostream>
#include <vector>
#include <variant>
#include <sstream>

#include <re2/re2.h>

#include <AocDay.h>
#include <AocObject.h>
#include <Split.h>

using std::cout, std::endl;

struct Value {
	struct Old {};

	std::variant<uint64_t, Old> data;

	explicit Value(std::string const& pattern) {
		if (pattern == "old")
			data = Old{};
		else {
			data = static_cast<uint64_t>(std::stoll(pattern));
		}

	}
	Value(uint64_t value) : data(value) {}
	Value() : data(Old{}) {}

	uint64_t get(uint64_t old) const {
		switch (data.index()) {
		case 0: return std::get<0>(data);
		default: return old;
		}
	}
};

struct MonkeyOperation {
	// some defs
	enum Op {
		Add,
		Mul
	};

	// fields
	Op op;
	Value lhs;
	Value rhs;

	uint64_t calc(uint64_t old) const {
		switch (op) {
		case Add:
			return lhs.get(old) + rhs.get(old);
		case Mul:
			return lhs.get(old) * rhs.get(old);
		default:
			cout << "Err" << endl;
			return -1;
		}
	}
};

struct Monkey : public aoc::AocObject<Monkey> {
	std::string number;
	std::vector<uint64_t> starting_items;
	int divisible_by;
	int throw_true;
	int throw_false;
	MonkeyOperation operation;
	int activity = 0;

	static auto get_object(std::istream& stream, Monkey& monkey) -> bool {
		monkey.starting_items.clear();

		static const re2::RE2 RE_OPERATION{ R"(\s*(\w+)\s*(\S)\s*(\w+)\s*)" };
		static const re2::RE2 RE_TEST{ R"(\s+Test: divisible by (\d+))" };

		std::string line;

		if (!std::getline(stream, line, ' ')) return false; // skip 'Monkey ', if no new monkey comes, bail
		std::getline(stream, monkey.number, ':'); // read monkey number
		//std::getline(stream, line); // skip until end of line (1 char)

		std::getline(stream, line, ':'); // skip until "items:"
		std::getline(stream, line); // read starting items

		int pos = 0;
		int last = 0;
		while (std::string::npos != (pos = line.find(", ", last))) {
			monkey.starting_items.push_back(std::stoi(line.substr(last, pos)));
			last = pos + 2;
		}
		monkey.starting_items.push_back(std::stoi(line.substr(last, pos)));

		std::getline(stream, line, ':'); // skipl "Operation:"
		std::getline(stream, line, '='); // skip until the assignment (we assume that it always starts with "new = "
		std::getline(stream, line);

		std::string lhs;
		std::string rhs;
		char op;
		re2::RE2::FullMatch(line, RE_OPERATION, &lhs, &op, &rhs);

		monkey.operation.lhs = Value{ lhs };
		monkey.operation.rhs = Value{ rhs };

		switch (op) {
		case '+': monkey.operation.op = MonkeyOperation::Add; break;
		case '*': monkey.operation.op = MonkeyOperation::Mul; break;
		}

		std::getline(stream, line); // line "Test: divisible by \d+"
		re2::RE2::FullMatch(line, RE_TEST, &monkey.divisible_by);

		std::getline(stream, line);
		monkey.throw_true = std::stoi(line.substr(line.find_last_of(' ')));

		std::getline(stream, line);
		line.find_last_of(' ');
		monkey.throw_false = std::stoi(line.substr(line.find_last_of(' ')));

		std::getline(stream, line); // skip empty line in between the monkeys
		return true;
	}
};


std::ostream& operator<<(std::ostream& os, Value const& v) {
	switch (v.data.index()) {
	case 0: return os << std::get<uint64_t>(v.data);
	case 1: return os << "old";
	}
	return os;
}


std::ostream& operator<<(std::ostream& os, MonkeyOperation const& o) {
	switch (o.op) {
	case MonkeyOperation::Add:
		return os << o.lhs << " + " << o.rhs;
	case MonkeyOperation::Mul:
		return os << o.lhs << " * " << o.rhs;
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, Monkey const& m) {
	os << "Monkey " << m.number << endl;
	os << "Starting Items: [";
	for (int item : m.starting_items) {
		os << item << ", ";
	}
	os << "]" << endl;
	os << "Operation: " << m.operation;
	os << endl;
	os << "if new%" << m.divisible_by << " == 0\n"
		<< "    " << "throw_to(" << m.throw_true << ")\n"
		<< "else\n"
		<< "    " << "throw_to(" << m.throw_false << ")\n";
	return os;
}

class Day11 : public aoc::AocObject<Day11>, public aoc::AocDay<Day11, uint64_t> {
	std::vector<Monkey> m_monkeys;

public:
	static constexpr char FILENAME[] = "11";
	static constexpr char YEAR[] = "2022";
	static constexpr char TITLE[] = "Monkey in the Middle";

	static auto get_object(std::istream& stream, Day11& day) -> bool {
		day.m_monkeys.clear();

		Monkey monkey{};

		while (Monkey::get_object(stream, monkey)) {
			day.m_monkeys.push_back(monkey);
		}

		return true;
	}

	uint64_t part1() const override {
		auto monkeys = m_monkeys;

		uint64_t factor = 1;
		for (const auto& m : monkeys) {
			factor *= m.divisible_by;
		}

		for (int round = 0; round < 20; ++round) {
			for (Monkey& monkey : monkeys) {
				//cout << "Monkey " << monkey.number << endl;
				for (int item : monkey.starting_items) {
					//cout << " Monkey inspects an item with a worry level of " << item << endl;
					item = monkey.operation.calc(item);
					//cout << " Worry level -> " << item << endl;
					item /= 3;
					item %= factor;
					//cout << " Worry level -> " << item << endl;
					if (item % monkey.divisible_by == 0) {
						//cout << "throw " << item << " to " << monkey.throw_true << endl;
						monkeys[monkey.throw_true].starting_items.push_back(item);
					}
					else {
						//cout << "throw " << item << " to " << monkey.throw_false << endl;
						monkeys[monkey.throw_false].starting_items.push_back(item);
					}
					++monkey.activity;
				}
				monkey.starting_items.clear();
			}
		}
		std::vector<int> activities;
		for (Monkey const& m : monkeys)
			activities.push_back(m.activity);
		std::sort(activities.begin(), activities.end());

		return activities[activities.size() - 1] * activities[activities.size() - 2];
	}

	uint64_t part2() const override {
		auto monkeys = m_monkeys;

		uint64_t factor = 1;
		for (const auto& m : monkeys) {
			factor *= m.divisible_by;
		}

		for (int round = 0; round < 10000; ++round) {
			for (Monkey& monkey : monkeys) {
				for (uint64_t item : monkey.starting_items) {
					item = monkey.operation.calc(item);
					item %= factor;
					if (item % monkey.divisible_by == 0) {
						monkeys[monkey.throw_true].starting_items.push_back(item);
					}
					else {
						monkeys[monkey.throw_false].starting_items.push_back(item);
					}
					++monkey.activity;
				}
				monkey.starting_items.clear();
			}
		}
		std::vector<uint64_t> activities;
		for (Monkey const& m : monkeys) {
			activities.push_back(m.activity);
		}
		std::sort(activities.begin(), activities.end());


		return activities[activities.size() - 1] * activities[activities.size() - 2];
	}

};
