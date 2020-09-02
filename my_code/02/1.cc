

#include <iostream>
#include <algorithm>

class shape{

};

class circle : public shape {

};

template <typename T>
class smart_ptr {
public:
  explicit smart_ptr(T *ptr = nullptr)
    : ptr_(ptr) {}
  ~smart_ptr() {
    if (ptr_) delete ptr_;
  }
  T *get() const {
    return ptr_;
  }
  T& operator *() const {
    return *ptr_;
  }
  T* operator ->() const {
    return ptr_;
  }
  operator bool() const {
    return ptr_;
  }
  // 拷贝构造函数
  // smart_ptr(smart_ptr &other) {
  //   ptr_ = other.release();
  // };
  // 拷贝赋值运算符
  /// 提供了强异常安全性
  /// 赋值分为拷贝构造和交换两步
  /// 异常只可能发生在第一步，而第一步发生异常的话
  /// this对象完全不受影响（第一步发生异常，第二步不会进行，因此*this不会受影响）
  // smart_ptr &operator=(smart_ptr &rhs) {
  //   smart_ptr(rhs).swap(*this);
  //   return *this;
  // };
  // 移动构造函数
  // 根据 C++ 的规则，如果我提供了移动构造函数而没有手动提供拷贝构造函数，那后者自动被禁用
  // smart_ptr(smart_ptr &&other) {
  //   ptr_ = other.release();
  // }
  // 赋值函数的行为是移动还是拷贝，
  // 完全依赖于参数构造时走的是移动构造还是拷贝构造
  // (operator=()的参数在接收参数的时候，会调用构造函数，如果调用的是拷贝构造，那赋值操作就是拷贝，如果调用的是移动构造，那么赋值操作就是移动。)
  // (传递左值就会触发拷贝构造，没有拷贝构造函数就会编译错误；传递右值就会触发移动构造，就不会有问题。 因此， 1. 赋值只能是右值； 2. 不可以拷贝构造。)
  smart_ptr &operator=(smart_ptr rhs) {
    rhs.swap(this);
    return *this;
  }
  // 子类指针向基类指针的转换
  // 构造函数不被编译器看作移动构造函数，因而不能自动触发删除拷贝构造函数的行为。
  // 如果我们想消除代码重复、删除移动构造函数的话，就需要把拷贝构造函数标记成 = delete 了
  // 不过，更通用的方式仍然是同时定义标准的拷贝 / 移动构造函数和所需的模板构造函数。
  template <typename U>
  smart_ptr (smart_ptr<U> &&other) {
    ptr_ = other.release();
  }

  T *release() {
    T *ptr = ptr_;
    ptr_ = nullptr;
    return ptr;
  }
  void swap(smart_ptr &rhs) {
    using std::swap;
    swap(ptr_, rhs.ptr_);
  }


private:
  T *ptr_;
};

int main() {

  shape *s = new shape;
  smart_ptr<shape> sp(s);

  // shape_wrapper sw1 = sw;

  return 0;
}