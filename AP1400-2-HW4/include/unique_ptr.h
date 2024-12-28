#ifndef UNIQUE_PTR
#define UNIQUE_PTR

#include <utility>
#include <stdexcept>
template<typename T>
class UniquePtr{
    T* _ptr;
public:
    UniquePtr(T* ptr);
    UniquePtr(const UniquePtr<T>&) = delete;
    UniquePtr();
    ~UniquePtr();
    T* get();
    T& operator*();
    UniquePtr<T>& operator=(const UniquePtr<T>&);
    T* operator->();
    void reset(T* ptr);
    void reset();
    explicit operator bool() const noexcept;
    T* release();
};

#endif //UNIQUE_PTR

template <typename T>
inline UniquePtr<T>::UniquePtr(T *ptr)
{
    this->_ptr = ptr;
}
template <typename T>
inline UniquePtr<T>::UniquePtr()
{
    _ptr = nullptr;
}
template <typename T>
inline UniquePtr<T>::~UniquePtr()
{
    delete _ptr;
}

template <typename T>
inline T *UniquePtr<T>::get()
{
    return _ptr;
}

template <typename T>
inline T &UniquePtr<T>::operator*()
{
    return *_ptr;
}

template <typename T>
inline UniquePtr<T> &UniquePtr<T>::operator=(const UniquePtr<T> &)
{
    throw std::runtime_error("Can't assign unique pointers");
}

template <typename T>
inline T *UniquePtr<T>::operator->()
{
    return _ptr;
}

template <typename T>
inline void UniquePtr<T>::reset(T *ptr)
{
    delete _ptr;
    _ptr = ptr;
}

template <typename T>
inline void UniquePtr<T>::reset()
{
    delete _ptr;
    _ptr = nullptr;
}

template <typename T>
inline UniquePtr<T>::operator bool() const noexcept
{
    return _ptr != nullptr;
}

template <typename T>
inline T *UniquePtr<T>::release()
{
    T* temp = _ptr;
    _ptr = nullptr;
    return temp;
}

template <typename T,typename... Args>
UniquePtr<T> make_unique(Args... args)
{
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}
