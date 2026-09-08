//
// Created by leegu on 26. 9. 8..
//

#ifndef CPP_HOMEWORK_ITERATOR_H
#define CPP_HOMEWORK_ITERATOR_H

namespace iter {
    template <typename E>
    class Iterable {
        public:
            // Iterable하게 만들기 위한 기본 조건
            // 기본적인 데이터 이외에도 레퍼런스도 지원할수 있도록 const 키워드가 붙은 begin/end도 구현해야함
            virtual ~Iterable() = default;

            virtual E *begin() = 0;

            virtual E *end() = 0;

            virtual const E *begin() const = 0;

            virtual const E *end() const = 0;
    };
}

#endif //CPP_HOMEWORK_ITERATOR_H
