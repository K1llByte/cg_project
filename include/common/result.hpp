#ifndef RESULT_HPP
#define RESULT_HPP

#include <variant>


// Perhaps to be used later
template<typename T, typename E>
class result
{
    private:
        std::variant<T,E> res;

    public:
        result(const T& v) : res(v) {}
        result(const E& e) : res(e) {}
        ~result() = default;

        constexpr T value() const
        {
            return std::get<0>(res);
        }

        constexpr E error() const
        {
            return std::get<1>(res);
        }

        constexpr std::size_t index() const
        {
            return res.index();
        }
};

#endif // RESULT_HPP