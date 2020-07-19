#ifndef SMART_PTR_H
#define SMART_PTR_H

#include <atomic>   // std::atomic
#include <utility>  // std::swap
#include <iostream>

class shared_count {
public:
    shared_count() noexcept
        : count_(1)
    {
    }
    void add_count() noexcept
    {
        count_.fetch_add(1, std::memory_order_relaxed);
    }
    long reduce_count() noexcept
    {
        return --count_;
    }
    long get_count() const noexcept
    {
        return count_;
    }

private:
    std::atomic_long count_;
};

template <typename T>
class smart_ptr {
public:
    template <typename U>
    friend class smart_ptr;

    explicit smart_ptr(T* ptr = nullptr)
        : ptr_(ptr)
    {
        if (ptr) {
            shared_count_ = new shared_count();
        }
    }
    ~smart_ptr()
    {
        if (ptr_ && !shared_count_->reduce_count()) {
            delete ptr_;
            delete shared_count_;
        }
    }

    smart_ptr(const smart_ptr& other) noexcept
    {
        ptr_ = other.ptr_;
        if (ptr_) {
            other.shared_count_->add_count();
            shared_count_ = other.shared_count_;
        }
    }
    template <typename U>
    smart_ptr(const smart_ptr<U>& other) noexcept
    {
        ptr_ = other.ptr_;
        if (ptr_) {
            other.shared_count_->add_count();
            shared_count_ = other.shared_count_;
        }
    }
    template <typename U>
    smart_ptr(smart_ptr<U>&& other) noexcept
    {
        ptr_ = other.ptr_;
        if (ptr_) {
            shared_count_ = other.shared_count_;
            other.ptr_ = nullptr;
        }
    }
    template <typename U>
    smart_ptr(const smart_ptr<U>& other, T* ptr) noexcept
    {
        ptr_ = ptr;
        if (ptr_) {
            other.shared_count_->add_count();
            shared_count_ = other.shared_count_;
        }
    }
    /**
     * 这里的参数是 smart_ptr 类型，先构造一个右值，
     * 智能指针计数加1，在swap给左值
     * 为什么 smart_ptr::operator= 对左值和右值都有效，而且不需要对等号两边是否引用同一对象进行判断？
     * 左值和右值都有效是因为构造参数时，如果是左值，就用拷贝构造构造函数，右值就用移动构造函数
     * 无论是左值还是右值，构造参数时直接生成新的智能指针，因此不需要判断
     */
    smart_ptr& operator=(smart_ptr rhs) noexcept
    {
        std::cout << "operator=(smart_ptr rhs)" << std::endl;
        rhs.swap(*this);
        return *this;
    }

    T* get() const noexcept
    {
        return ptr_;
    }
    long use_count() const noexcept
    {
        if (ptr_) {
            return shared_count_->get_count();
        } else {
            return 0;
        }
    }
    void swap(smart_ptr& rhs) noexcept
    {
        std::cout << "swap(smart_ptr& rhs)" << std::endl;
        using std::swap;
        swap(ptr_, rhs.ptr_);
        swap(shared_count_, rhs.shared_count_);
    }

    T& operator*() const noexcept
    {
        return *ptr_;
    }
    T* operator->() const noexcept
    {
        return ptr_;
    }
    operator bool() const noexcept
    {
        return ptr_;
    }

private:
    T* ptr_;
    shared_count* shared_count_;
};

template <typename T>
void swap(smart_ptr<T>& lhs, smart_ptr<T>& rhs) noexcept
{
    lhs.swap(rhs);
}

template <typename T, typename U>
smart_ptr<T> static_pointer_cast(const smart_ptr<U>& other) noexcept
{
    T* ptr = static_cast<T*>(other.get());
    return smart_ptr<T>(other, ptr);
}

template <typename T, typename U>
smart_ptr<T> reinterpret_pointer_cast(const smart_ptr<U>& other) noexcept
{
    T* ptr = reinterpret_cast<T*>(other.get());
    return smart_ptr<T>(other, ptr);
}

template <typename T, typename U>
smart_ptr<T> const_pointer_cast(const smart_ptr<U>& other) noexcept
{
    T* ptr = const_cast<T*>(other.get());
    return smart_ptr<T>(other, ptr);
}

template <typename T, typename U>
smart_ptr<T> dynamic_pointer_cast(const smart_ptr<U>& other) noexcept
{
    T* ptr = dynamic_cast<T*>(other.get());
    return smart_ptr<T>(other, ptr);
}

#endif // SMART_PTR_H
