//
// Created by apsode on 26. 9. 4..
//

#ifndef CPP_HOMEWORK_SCOPEDPOINTER_H
#define CPP_HOMEWORK_SCOPEDPOINTER_H



namespace ScopedPointer {
    template <typename T>
    class ScopedPointer{
    private:
        T* target;

    public:
        // UB 방지용 explicit 키워드
        explicit ScopedPointer(T* target) : target(target) {}

        T* get() const;

        T& operator*() const;

        T* operator->() const;









    };

} // ScopedPointer

#endif //CPP_HOMEWORK_SCOPEDPOINTER_H
