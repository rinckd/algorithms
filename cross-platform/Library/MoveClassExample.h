//
// Created by David Rinck on 12/17/15.
//

#ifndef BATSCODINGEXERCISE_MOVECLASSEXAMPLE_H
#define BATSCODINGEXERCISE_MOVECLASSEXAMPLE_H

#include <string>

class Annotation
{
public:
    explicit Annotation(const std::string text) : value(std::move(text)) {}

private:
    std::string value;
};


// p158
// lvalue - can take its address - has a name
// rvalue - can't

// int& rx = x; // lvalue reference
// const int& crx = foo() // lifetime extension..
// int&& rrx = // rvalue reference
// int&& rrx = foo() // good;
//
// move constructors

std::string s("Beef");
std::string s2(s); // copy instruction



class MoveClassExample
{
    double d_;
    int* p_;
    std::string str_;

    std::string makeMeat() { return "pork"; }
public:
    MoveClassExample(MoveClassExample&& rhs) :
        d_(rhs.d_),
        p_(rhs.p_),
        str_(rhs.str_)
    {
        // now that it's moved, rhs becomes nullptr and clear
        rhs.p_ = nullptr;
        rhs.str_.clear();
    }

    
    MoveClassExample& operator=(MoveClassExample&& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }
        delete p_; // delete before assigning ??
        d_ = rhs.d_;
        p_ = rhs.p_;
        //str_ = rhs.str_; // this fails because rhs.str_ is an lvalue
        str_ = std::move(rhs.str_);
        rhs.p_ = nullptr;
        rhs.str_.clear();
        return *this;
    }

    void set_str(std::string&& str)
    {
        str_ = std::move(str);
        str.clear(); // have to actually move
    }

    // lvalue that isn't used any more.
    // A a;
    // v.push_back(move(a)) // move overload used.

    // rValue References to const Values
    // const r values can't be moved
    // don't use f(const &&)
    // Derived(Derived&& rhs) : Base(move(rhs)) {}
    // B(B&& rhs)
    // { * this = rhs; // wrong: invokes copy assignment operator




};


#endif //BATSCODINGEXERCISE_MOVECLASSEXAMPLE_H
