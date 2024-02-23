#ifndef BASE_EXPR_HPP
#define BASE_EXPR_HPP

namespace eeval::ast
{
    /**
     * @brief Interface for common behavior of exprs. Also the parent abstract class of exprs. 
     */
    class Expr
    {
    public:
        virtual ~Expr() = default;

        virtual double interpret() const = 0;
    };
}

#endif