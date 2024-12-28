#ifndef SHARED_PTR
#define SHARED_PTR

#include <utility>

template <typename T>
class SharedPtr{
private:
    T* _ptr;
    int* _count;
public:
    SharedPtr(T* p);
    SharedPtr(const SharedPtr<T>& sp);
    SharedPtr();
    ~SharedPtr();
    int use_count() const;
    T* get() const {return _ptr;}
    T& operator*() const {return *_ptr;}
    T* operator->() const {return _ptr;}
    void reset(T* p);
    void reset();
    explicit operator bool() const {return _ptr != nullptr;}
    SharedPtr<T>& operator=(const SharedPtr<T>& sp);
};



template <typename T>
inline SharedPtr<T>::SharedPtr(T *p)
{
    _ptr = p;
    _count = new int(1);
}
template <typename T>
inline SharedPtr<T>::SharedPtr(const SharedPtr<T>& sp){
    _ptr = sp._ptr;
    _count = sp._count;
    (*_count)++;
}
template <typename T>
inline SharedPtr<T>::SharedPtr()
{
    _ptr = nullptr;
    _count = new int(1);
}
template <typename T>
inline SharedPtr<T>::~SharedPtr()
{
    (*_count)--;
    if(*_count == 0){
        _ptr= nullptr;
    }
}

template <typename T>
inline int SharedPtr<T>::use_count() const
{
    return *_count;
}

template <typename T>
inline void SharedPtr<T>::reset(T *p)
{
    if(_ptr != p){
        _ptr = p;
        _count = new int(1);
    }
}

template <typename T>
inline void SharedPtr<T>::reset()
{
    _ptr = nullptr;
    _count = new int(0);
}

template <typename T>
inline SharedPtr<T> &SharedPtr<T>::operator=(const SharedPtr<T> &sp)
{
    if(this != &sp){
        _ptr = sp._ptr;
        _count = sp._count;
        (*_count)++;
    }
    return *this;
}

template<typename T,typename... Args>
inline SharedPtr<T> make_shared(Args... args){
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

#endif // SHARED_PTR_H
