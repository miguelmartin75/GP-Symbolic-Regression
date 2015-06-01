#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include <QValidator>
#include <utility>

namespace
{
    template <class T>
    struct Generator
    {
        template <class... Args>
        QValidator* operator()(Args&&...) { return nullptr; }
    };
}

template <class T, class... Args>
QValidator* validator(Args&&... args)
{
    return Generator<T>{}(std::forward<Args>(args)...);
}

namespace
{
    template<>
    struct Generator<int>
    {
        template <class... Args>
        QValidator* operator()(Args&&... args) { return new QIntValidator(std::forward<Args>(args)...); }
    };

    template<>
    struct Generator<float>
    {
        template <class... Args>
        QValidator* operator()(Args&&... args) { return new QDoubleValidator(std::forward<Args>(args)...); }
    };

    template<>
    struct Generator<double>
    {
        template <class... Args>
        QValidator* operator()(Args&&... args) { return new QDoubleValidator(std::forward<Args>(args)...); }
    };
}

#endif // VALIDATOR_HPP
