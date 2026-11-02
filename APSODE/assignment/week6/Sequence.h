#ifndef SEQUENCE_H
#define SEQUENCE_H


template<typename T, int TAIL = 1>
class Sequence {
    T *container;
    int m_capacity;
    int m_size;

    public:
        int size() const;

        bool empty() const;

        int capacity() const;

        const T& at(int index) const;

        T& at(int index);


        const T& operator[](int index) const;

        T& operator[](int index);

    protected:
        static constexpr int DEFAULT_CAPACITY = 8;
        static constexpr int GROWTH_FACTOR = 2;

        Sequence();

        Sequence(const T *elements, int size);

        Sequence(const Sequence &other);

        virtual ~Sequence();


        void ensure_capacity(int min_size);

        void copy_elements(const Sequence &other);

        void copy_elements(const T *elements, int size);

        void insert(int index, const T *element);

        void insert(int index, const T *elements, int count);

        void erase(int start, int count = 1);

        void append(const T *element);

        void append(const T *elements, int count);

        void clear();


        Sequence& operator=(const Sequence &other);

    private:
        int extend(int add_amount);

        void fill_tail();

        void require_valid_index(int index) const;

        static void require_valid_range(const T *elements, int count);

        constexpr static T* create_empty_container(int size) {
            return new T[calc_new_capacity(size) + TAIL]();
        }

        constexpr static int calc_new_capacity(int amount) {
            if (amount < DEFAULT_CAPACITY) {
                return DEFAULT_CAPACITY;
            }

            return (amount + DEFAULT_CAPACITY - 1) / DEFAULT_CAPACITY * DEFAULT_CAPACITY;
        }

        constexpr static void swap(Sequence& sequence1, Sequence& sequence2) {
            T* temp_container = sequence1.container;
            sequence1.container = sequence2.container;
            sequence2.container = temp_container;

            int temp_size = sequence1.m_size;
            sequence1.m_size = sequence2.m_size;
            sequence2.m_size = temp_size;

            int temp_capacity = sequence1.m_capacity;
            sequence1.m_capacity = sequence2.m_capacity;
            sequence2.m_capacity = temp_capacity;
        }
};



#include "Sequence.tpp"

#endif //SEQUENCE_H
