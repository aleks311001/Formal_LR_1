//
// Created by aleks311001 on 18.12.2020.
//

#ifndef LR_1__TESTUTILS_H_
#define LR_1__TESTUTILS_H_

#include <gtest/gtest.h>
#include <set>

template<typename T>
std::string containerToString(const std::set<T>& set) {
    std::string result = "{";
    for (auto& item: set) {
        result += std::string() + item + ", ";
    }
    result += "}";

    return result;
}

template<typename T, typename U>
std::string containerToString(const std::unordered_multimap<T, U>& map) {
    std::string result = "{";
    for (auto& item: map) {
        result += std::string() + "(" + item.first + ": " + item.second + "), ";
    }
    result += "}";

    return result;
}

template<typename T>
std::string containerToString(const std::set<Situation<T>>& set) {
    std::string result = "{\n";
    for (auto& item: set) {
        result += std::string() + "(" + item.left + " -> " + item.right + ", " +
                item.first + "; point = " + std::to_string(item.point) + ")\n";
    }
    result += "}";

    return result;
}

template<typename T>
std::string containerToString(const std::unordered_map<T, std::set<T>>& map) {
    std::string result = "{\n";
    for (auto& item: map) {
        result += std::string() + "(" + item.first + ": " + containerToString(item.second) + ")\n";
    }
    result += "}";

    return result;
}

template<typename T>
std::string containerToString(const std::unordered_map<T, size_t>& map) {
    std::string result = "{";
    for (auto& item: map) {
        result += std::string() + "(" + item.first + ": " + std::to_string(item.second) + "), ";
    }
    result += "}";

    return result;
}

template< template <typename...> typename Container, typename ...Args>
testing::AssertionResult equalContainers(const Container<Args...>& left, const Container<Args...>& right) {
    if (left.size() != right.size()) {
        return testing::AssertionFailure() << containerToString(left) << " not equal " << containerToString(right);
    }

    for (auto& itemLeft: left) {
        bool found = false;
        for (auto& itemRight: right) {
            if (itemLeft == itemRight) {
                found = true;
                break;
            }
        }
        if (!found) {
            return testing::AssertionFailure() << containerToString(left) << " not equal " << containerToString(right);
        }
    }

    return testing::AssertionSuccess() << containerToString(left) << " equal " << containerToString(right);
}

#endif //LR_1__TESTUTILS_H_
