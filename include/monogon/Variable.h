//
// Created by Mateusz Paszkowski on 21.08.22.
//

#ifndef MONOGON_VARIABLE_H
#define MONOGON_VARIABLE_H

//------------------------------------------------- Class Definition ---------------------------------------------------

#include "Operation.h"
#include <any>
#include <memory>
#include <optional>
#include <variant>

class AbstractVariableNode
{
public:
    AbstractVariableNode() = default;
    AbstractVariableNode(const AbstractVariableNode &) = delete;
    AbstractVariableNode(AbstractVariableNode &&) noexcept = delete;

    AbstractVariableNode &operator=(const AbstractVariableNode &) = default;
    AbstractVariableNode &operator=(AbstractVariableNode &&) noexcept = default;

    virtual ~AbstractVariableNode() = default;

    virtual void back_propagation() const = 0;

protected:
    virtual void back_propagation(std::any) const = 0;
    virtual void zero_grad() const = 0;
    virtual std::any get_any_value() const = 0;

    template <typename U>
    friend class VariableNode;
};

template <typename T>
class VariableNode : public AbstractVariableNode
{
public:
    VariableNode();

    explicit VariableNode(T val);

    VariableNode(T val, std::shared_ptr<const AbstractVariableNode> l, std::shared_ptr<Operation> op);

    VariableNode(T val,
                 std::shared_ptr<const AbstractVariableNode> l,
                 std::shared_ptr<const AbstractVariableNode> r,
                 std::shared_ptr<Operation> op);

public:
    template <typename U>
    auto operator=(VariableNode<U> &rhs) = delete;
    template <typename U>
    auto operator=(VariableNode<U> &&rhs) = delete;

    void back_propagation() const override;
    void back_propagation(std::any) const override;
    void zero_grad() const override;

public:
    const T &get_value() const;
    const T &get_grad() const;
    void set_value(const T &value);

public:
protected:
    std::any get_any_value() const override;

private:
    T value;

private:
    mutable std::optional<T> grad;
    mutable std::shared_ptr<Operation> operation;

    mutable std::shared_ptr<const AbstractVariableNode> lhs;
    mutable std::shared_ptr<const AbstractVariableNode> rhs;

    template <typename U>
    friend std::ostream &operator<<(std::ostream &os, const VariableNode<U> &variable);
};

//--------------------------------------------------- Constructors -----------------------------------------------------

template <typename T>
VariableNode<T>::VariableNode() : value(), grad(), operation(), lhs(), rhs()
{
}

template <typename T>
VariableNode<T>::VariableNode(T val) : value(val), grad(), operation(), lhs(), rhs()
{
}

template <typename T>
VariableNode<T>::VariableNode(T val, std::shared_ptr<const AbstractVariableNode> l, std::shared_ptr<Operation> op)
    : value(val), grad(), operation(std::move(op)), lhs(std::move(l)), rhs()
{
}

template <typename T>
VariableNode<T>::VariableNode(T val,
                              std::shared_ptr<const AbstractVariableNode> l,
                              std::shared_ptr<const AbstractVariableNode> r,
                              std::shared_ptr<Operation> op)
    : value(std::move(val)), grad(), operation(std::move(op)), lhs(std::move(l)), rhs(std::move(r))
{
}

//----------------------------------------------------- Methods --------------------------------------------------------

template <typename T>
const T &VariableNode<T>::get_value() const
{
    return this->value;
}

template <typename T>
const T &VariableNode<T>::get_grad() const
{
    return this->grad.value();
}

template <typename T>
void VariableNode<T>::set_value(const T &val)
{
    this->value = val;
}

template <typename T>
void VariableNode<T>::back_propagation() const
{
    grad = std::make_optional(Init<T>().initialize(this->value, 1.0));
    if (operation && lhs && rhs)
    {
        auto result = operation->perform(grad.value(), lhs->get_any_value(), rhs->get_any_value());
        lhs->back_propagation(std::get<0>(result));
        rhs->back_propagation(std::get<1>(result));
    }
    else if (operation && lhs)
    {
        auto result = operation->perform(grad.value(), lhs->get_any_value(), std::any());
        lhs->back_propagation(std::get<0>(result));
    }
}

template <typename T>
void VariableNode<T>::back_propagation(std::any df_val) const
{
    if (this->grad)
        this->grad = std::make_optional(this->grad.value() + std::any_cast<T>(df_val));
    else
        this->grad = std::make_optional(std::any_cast<T>(df_val));

    if (this->operation && this->lhs && this->rhs)
    {
        auto result = this->operation.get()->perform(std::any_cast<T>(df_val),
                                                     this->lhs->get_any_value(),
                                                     this->rhs->get_any_value());
        this->lhs->back_propagation(std::get<0>(result));
        this->rhs->back_propagation(std::get<1>(result));
    }
    else if (this->operation && this->lhs)
    {
        auto result = this->operation.get()->perform(std::any_cast<T>(df_val), this->lhs->get_any_value(), std::any());
        this->lhs->back_propagation(std::get<0>(result));
    }
}

template <typename T>
void VariableNode<T>::zero_grad() const
{
    this->grad = std::optional<T>();
    if (this->lhs)
        this->lhs->zero_grad();
    if (this->rhs)
        this->rhs->zero_grad();
}

template <typename T>
std::any VariableNode<T>::get_any_value() const
{
    return value;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const VariableNode<T> &variable)
{
    os << variable.value;
    return os;
}

//------------------------------------------------- Class Definition ---------------------------------------------------

template <typename T>
class Variable
{
public:
    explicit Variable(T value);

private:
    explicit Variable(std::shared_ptr<VariableNode<T>> cn);

public:
    template <typename U>
    auto operator+(const Variable<U> &rhs) const;
    template <typename U>
    auto operator-(const Variable<U> &rhs) const;
    template <typename U>
    auto operator*(const Variable<U> &rhs) const;
    template <typename U>
    auto operator/(const Variable<U> &rhs) const;

    template <typename U>
    auto dot(const Variable<U> &rhs) const;
    template <typename U>
    auto max(const Variable<U> &rhs) const;
    auto exp() const;
    auto avg() const;

public:
    void back_propagation() const;
    void zero_grad() const;

    const T &get_value() const;
    const T &get_grad() const;

    void set_value(const T &value);

private:
    std::shared_ptr<VariableNode<T>> core_node;

    template <typename U>
    friend class Variable;
};

//--------------------------------------------------- Constructors -----------------------------------------------------

template <typename T>
Variable<T>::Variable(T value) : core_node(std::make_shared<VariableNode<T>>(value))
{
}

template <typename T>
Variable<T>::Variable(std::shared_ptr<VariableNode<T>> cn) : core_node(cn)
{
}

//---------------------------------------------------- Operators -------------------------------------------------------

template <typename T>
template <typename U>
auto Variable<T>::operator+(const Variable<U> &rhs) const
{
    using result_val_type = decltype(std::declval<T>() + std::declval<U>());
    Variable<result_val_type> result(
        std::make_shared<VariableNode<result_val_type>>(core_node->get_value() + rhs.core_node->get_value(),
                                                        core_node,
                                                        rhs.core_node,
                                                        std::make_shared<AddOperation<result_val_type, T, U>>()));
    return result;
}

template <typename T>
template <typename U>
auto Variable<T>::operator-(const Variable<U> &rhs) const
{
    using result_val_type = decltype(std::declval<T>() + std::declval<U>());
    Variable<result_val_type> result(
        std::make_shared<VariableNode<result_val_type>>(core_node->get_value() - rhs.core_node->get_value(),
                                                        core_node,
                                                        rhs.core_node,
                                                        std::make_shared<SubOperation<result_val_type>>()));
    return result;
}

template <typename T>
template <typename U>
auto Variable<T>::operator*(const Variable<U> &rhs) const
{
    using result_val_type = decltype(std::declval<T>() * std::declval<U>());
    Variable<result_val_type> result(
        std::make_shared<VariableNode<result_val_type>>(core_node->get_value() * rhs.core_node->get_value(),
                                                        core_node,
                                                        rhs.core_node,
                                                        std::make_shared<MulOperation<result_val_type, T, U>>()));
    return result;
}

template <typename T>
template <typename U>
auto Variable<T>::operator/(const Variable<U> &rhs) const
{
    using result_val_type = decltype(std::declval<T>() / std::declval<U>());
    Variable<result_val_type> result(
        std::make_shared<VariableNode<result_val_type>>(core_node->get_value() / rhs.core_node->get_value(),
                                                        core_node,
                                                        rhs.core_node,
                                                        std::make_shared<DivOperation<result_val_type, T, U>>()));
    return result;
}

//----------------------------------------------------- Methods --------------------------------------------------------

template <typename T>
template <typename U>
auto Variable<T>::dot(const Variable<U> &rhs) const
{
    using result_val_type = decltype(std::declval<T>().dot(std::declval<U>()));
    Variable<result_val_type> result(
        std::make_shared<VariableNode<result_val_type>>(core_node->get_value().dot(rhs.core_node->get_value()),
                                                        core_node,
                                                        rhs.core_node,
                                                        std::make_shared<DotOperation<result_val_type, T, U>>()));
    return result;
}

template <typename T>
template <typename U>
auto Variable<T>::max(const Variable<U> &rhs) const
{
    using result_val_type = decltype(std::declval<T>().max(std::declval<U>()));

    Variable<result_val_type> result(
        std::make_shared<VariableNode<result_val_type>>(core_node->get_value().max(rhs.core_node->get_value()),
                                                        core_node,
                                                        rhs.core_node,
                                                        std::make_shared<MaxOperation<result_val_type, T, U>>()));
    return result;
}

template <typename T>
auto Variable<T>::exp() const
{
    using result_val_type = decltype(std::declval<T>().exp());
    Variable<result_val_type> result(
        std::make_shared<VariableNode<result_val_type>>(core_node->get_value().exp(),
                                                        core_node,
                                                        std::make_shared<ExpOperation<result_val_type, T>>()));
    return result;
}


template <typename T>
auto Variable<T>::avg() const
{
    using result_val_type = decltype(std::declval<T>().avg());
    Variable<result_val_type> result(
        std::make_shared<VariableNode<result_val_type>>(core_node->get_value().avg(),
                                                        core_node,
                                                        std::make_shared<AvgOperation<result_val_type, T>>()));
    return result;
}

template <typename T>
void Variable<T>::back_propagation() const
{
    this->core_node->back_propagation();
}

template <typename T>
void Variable<T>::zero_grad() const
{
    this->core_node->zero_grad();
}

//---------------------------------------------------- Accessors -------------------------------------------------------

template <typename T>
const T &Variable<T>::get_value() const
{
    return core_node->get_value();
}

template <typename T>
const T &Variable<T>::get_grad() const
{
    return core_node->get_grad();
}

template <typename T>
void Variable<T>::set_value(const T &value)
{
    this->core_node->set_value(value);
}

#endif //MONOGON_VARIABLE_H
