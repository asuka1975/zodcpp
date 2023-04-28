#ifndef ZOD_HPP
#define ZOD_HPP

#include <regex>
#include <string>
#include <utility>

template <class T>
struct empty_validator {
    using type = T;
    const T value;
    using this_t = empty_validator<T>;

    template <class... TArgs>
    constexpr empty_validator(TArgs... args) : value(std::forward<TArgs>(args)...) {}

    constexpr static void validate(const T& value) {}
};

template <class T, int N, class TValid=empty_validator<T>>
struct max_validator {

};

template <int N, class TValid>
struct max_validator<int, N, TValid>;

template <int N, class TValid>
struct max_validator<std::string, N, TValid>;

template <class T, int N, class TValid=empty_validator<T>>
struct min_validator {

};

template <int N, class TValid>
struct min_validator<int, N, TValid>;

template <int N, class TValid>
struct min_validator<std::string, N, TValid>;

template <int N, class TValid>
struct max_validator<int, N, TValid> {
    using type = int;
    using this_t = max_validator<int, N, TValid>;
    const int value;
    
    template <int M>
    using min = min_validator<int, M, this_t>;

    template <class... TArgs>
    constexpr max_validator(TArgs... args) : value(std::forward<TArgs>(args)...) {
        max_validator<int, N, TValid>::validate(value);
    }

    constexpr static void validate(const int& value) {
        if(value > N) {
            throw 0;
        }
        TValid::validate(value);
    }
};

template <int N, class TValid>
struct min_validator<int, N, TValid> {
    using type = int;
    using this_t = min_validator<int, N, TValid>;
    const int value;

    template <int M>
    using max = max_validator<int, M, this_t>;

    template <class... TArgs>
    constexpr min_validator(TArgs... args) : value(std::forward<TArgs>(args)...) {
        max_validator<int, N, TValid>::validate(value);
    }

    constexpr static void validate(const int& value) {
        if(value < N) {
            throw 0;
        }
        TValid::validate(value);
    }
};

template <class TValid>
struct email_validator;

template <class TValid>
struct url_validator;

template <int N, class TValid>
struct max_validator<std::string, N, TValid> {
    using type = std::string;
    using this_t = max_validator<std::string, N, TValid>;
    const std::string value;
    
    template <int M>
    using min = min_validator<std::string, M, this_t>;
    using email = email_validator<this_t>;

    template <class... TArgs>
    constexpr max_validator(TArgs... args) : value(std::forward<TArgs>(args)...) {
        max_validator<std::string, N, TValid>::validate(value);
    }

    constexpr static void validate(const std::string& value) {
        if(value.length() > N) {
            throw 0;
        }
        TValid::validate(value);
    }
};

template <int N, class TValid>
struct min_validator<std::string, N, TValid> {
    using type = std::string;
    using this_t = min_validator<std::string, N, TValid>;
    const std::string value;

    template <int M>
    using max = max_validator<std::string, M, this_t>;
    using email = email_validator<this_t>;

    template <class... TArgs>
    constexpr min_validator(TArgs... args) : value(std::forward<TArgs>(args)...) {
        max_validator<std::string, N, TValid>::validate(value);
    }

    constexpr static void validate(const std::string& value) {
        if(value.length() < N) {
            throw 0;
        }
        TValid::validate(value);
    }
};

template <class TValid=empty_validator<std::string>>
struct email_validator {
    using type = std::string;
    using this_t = email_validator<TValid>;
    const std::string value;

    template <int M>
    using max = max_validator<std::string, M, this_t>;
    template <int M>
    using min = min_validator<std::string, M, this_t>;

    template <class... TArgs>
    constexpr email_validator(TArgs... args) : value(std::forward<TArgs>(args)...) {
        email_validator<>::validate(value);
    }

    static void validate(const std::string& value) {
        static std::regex re(R"*(^[a-zA-Z0-9_.+-]+@([a-zA-Z0-9][a-zA-Z0-9-]*[a-zA-Z0-9]*\.)+[a-zA-Z]{2,}$)*");
        std::smatch m;
        if(!std::regex_match(value, m, re)) {
            throw 0;
        }
        TValid::validate(value);
    }

};

template <class TValid=empty_validator<std::string>>
struct url_validator {
    using type = std::string;
    using this_t = email_validator<TValid>;
    const std::string value;

    template <int M>
    using max = max_validator<std::string, M, this_t>;
    template <int M>
    using min = min_validator<std::string, M, this_t>;

    template <class... TArgs>
    constexpr url_validator(TArgs... args) : value(std::forward<TArgs>(args)...) {
        url_validator<>::validate(value);
    }

    static void validate(const std::string& value) {
        static std::regex re(R"*(\w+://[\w!?/+\-_~;.,*&@#$%()'[\]]+)*");
        std::smatch m;
        if(!std::regex_match(value, m, re)) {
            throw 0;
        }
        TValid::validate(value);
    }
};

template <class T> 
struct zod {

};

template <> 
struct zod<int> {
    using type = int;
    const int value;

    template <int N>
    using max = max_validator<int, N>;
    template <int N>
    using min = min_validator<int, N>;

    template <class... TArgs>
    constexpr zod(TArgs... args) : value(std::forward<TArgs>(args)...) {}
};

template <> 
struct zod<std::string> {
    using type = std::string;
    const std::string value;

    template <int N>
    using max = max_validator<std::string, N>;
    template <int N>
    using min = min_validator<std::string, N>;
    using email = email_validator<>;
    using url = url_validator<>;

    template <class... TArgs>
    constexpr zod(TArgs... args) : value(std::forward<TArgs>(args)...) {}
};

#endif