#ifndef ZOD_HPP
#define ZOD_HPP

#include <regex>
#include <string>
#include <utility>

template <class T>
struct empty_annotator {
    using type = T;
    const T value;

    template <class... TArgs>
    empty_annotator(TArgs... args) : value(std::forward<TArgs>(args)...) {}

    static void annotate(const T& value) {}
};

template <class T, int N, class TAnnot=empty_annotator<T>>
struct max_annotator {

};

template <int N, class TAnnot>
struct max_annotator<int, N, TAnnot>;

template <int N, class TAnnot>
struct max_annotator<std::string, N, TAnnot>;

template <class T, int N, class TAnnot=empty_annotator<T>>
struct min_annotator {

};

template <int N, class TAnnot>
struct min_annotator<int, N, TAnnot>;

template <int N, class TAnnot>
struct min_annotator<std::string, N, TAnnot>;

template <int N, class TAnnot>
struct max_annotator<int, N, TAnnot> {
    using type = int;
    const int value;
    
    template <int M>
    using min = min_annotator<int, M, max_annotator<int, N, TAnnot>>;

    template <class... TArgs>
    max_annotator(TArgs... args) : value(std::forward<TArgs>(args)...) {
        max_annotator<int, N, TAnnot>::annotate(value);
    }

    static void annotate(const int& value) {
        if(value > N) {
            throw 0;
        }
        TAnnot::annotate(value);
    }
};

template <int N, class TAnnot>
struct min_annotator<int, N, TAnnot> {
    using type = int;
    const int value;

    template <int M>
    using max = max_annotator<int, M, min_annotator<int, N, TAnnot>>;

    template <class... TArgs>
    min_annotator(TArgs... args) : value(std::forward<TArgs>(args)...) {
        max_annotator<int, N, TAnnot>::annotate(value);
    }

    static void annotate(const int& value) {
        if(value < N) {
            throw 0;
        }
        TAnnot::annotate(value);
    }
};

template <class TAnnot>
struct email_annotator;

template <int N, class TAnnot>
struct max_annotator<std::string, N, TAnnot> {
    using type = std::string;
    const std::string value;
    
    template <int M>
    using min = min_annotator<std::string, M, max_annotator<std::string, N, TAnnot>>;
    using email = email_annotator<max_annotator<std::string, N, TAnnot>>;

    template <class... TArgs>
    max_annotator(TArgs... args) : value(std::forward<TArgs>(args)...) {
        max_annotator<std::string, N, TAnnot>::annotate(value);
    }

    static void annotate(const std::string& value) {
        if(value.length() > N) {
            throw 0;
        }
        TAnnot::annotate(value);
    }
};

template <int N, class TAnnot>
struct min_annotator<std::string, N, TAnnot> {
    using type = std::string;
    const std::string value;

    template <int M>
    using max = max_annotator<std::string, M, min_annotator<std::string, N, TAnnot>>;
    using email = email_annotator<min_annotator<std::string, N, TAnnot>>;

    template <class... TArgs>
    min_annotator(TArgs... args) : value(std::forward<TArgs>(args)...) {
        max_annotator<std::string, N, TAnnot>::annotate(value);
    }

    static void annotate(const std::string& value) {
        if(value.length() < N) {
            throw 0;
        }
        TAnnot::annotate(value);
    }
};

template <class TAnnot=empty_annotator<std::string>>
struct email_annotator {
    using type = std::string;
    const std::string value;

    template <int M>
    using max = max_annotator<std::string, M, email_annotator<TAnnot>>;
    template <int M>
    using min = min_annotator<std::string, M, email_annotator<TAnnot>>;

    template <class... TArgs>
    email_annotator(TArgs... args) : value(std::forward<TArgs>(args)...) {
        email_annotator<>::annotate(value);
    }

    static void annotate(const std::string& value) {
        static std::regex re(R"*(^[a-zA-Z0-9_.+-]+@([a-zA-Z0-9][a-zA-Z0-9-]*[a-zA-Z0-9]*\.)+[a-zA-Z]{2,}$)*");
        std::smatch m;
        if(!std::regex_match(value, m, re)) {
            throw 0;
        }
        TAnnot::annotate(value);
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
    using max = max_annotator<int, N>;
    template <int N>
    using min = min_annotator<int, N>;

    template <class... TArgs>
    zod(TArgs... args) : value(std::forward<TArgs>(args)...) {}
};

template <> 
struct zod<std::string> {
    using type = std::string;
    const std::string value;

    template <int N>
    using max = max_annotator<std::string, N>;
    template <int N>
    using min = min_annotator<std::string, N>;
    using email = email_annotator<>;

    template <class... TArgs>
    zod(TArgs... args) : value(std::forward<TArgs>(args)...) {}
};

#endif