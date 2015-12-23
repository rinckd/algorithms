#ifndef BATSCODINGEXERCISE_EXPECT_H
#define BATSCODINGEXERCISE_EXPECT_H

#include <stdexcept>
#include <typeinfo>

// Built from Andrei Alexandrescu's talk on Expected<T> - Systematic Error Handling in C++
// There is a proposal to add to standard, std::expected (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4109.pdf)
// The following is a light-weight implementation based directly off his lecture notes
namespace Algorithms
{
template<typename T>
class Expected
{
public:
    Expected(const T &value) :
        valid_(true),
        value_(value) { };

    Expected(T &&value) :
        valid_(true),
        value_(std::move(value)) { }

    Expected(const Expected &rhs) :
        valid_(rhs.valid_)
    {
        if (valid_) new(&value_) T(std::move(rhs.value_));
        else new(&exception_) std::exception_ptr(std::move(rhs.exception_));
    }

    Expected() : valid_(false) { }

    Expected(Expected &&rhs) :
        valid_(rhs.valid_)
    {
        if (valid_)
        {
            new(&value_) T(std::move(rhs.value_));
        }
        else
        {
            new(&exception_) std::exception_ptr(std::move(rhs.exception_));
        }
    };

    ~Expected()
    {
        if (!valid_)
        {
            exception_.~exception_ptr();
        }
    }

    template<class E>
    static Expected<T> from_exception(const E &exception)
    {
        if (typeid(exception) != typeid(E)) throw std::invalid_argument("slicing detected");
        return from_exception(std::make_exception_ptr(exception));
    }

    static Expected<T> from_exception(std::exception_ptr exception)
    {
        Expected<T> result;
        result.valid_ = false;
        new(&result.exception_) std::exception_ptr(std::move(exception));
        return result;
    }

    static Expected<T> from_exception() { return from_exception(std::current_exception()); }
    bool valid() const { return valid_; }
    T &operator*() { return value(); }
    const T &operator*() const { return value(); }
    T *operator->() { return &value(); }
    const T *operator->() const { return &value(); }

    T &value()
    {
        if (!valid_) std::rethrow_exception(exception_);
        return value_;
    }

    T &value() const
    {
        if (!valid_) std::rethrow_exception(exception_);
        return value_;
    }

private:
    bool valid_;
    union
    {
        T value_;
        std::exception_ptr exception_;
    };
};
}
#endif
