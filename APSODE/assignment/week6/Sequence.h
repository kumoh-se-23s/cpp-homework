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

        void insert(int index, const T &element);

        void insert(int index, const T *elements, int amount);

        void erase(int start, int amount = 1);

        void append(const T &element);

        void append(const T *elements, int amount);

        void clear();


        Sequence& operator=(const Sequence &other);

    private:
        constexpr bool need_shrink() {
            return this->m_capacity > DEFAULT_CAPACITY && this->m_size <= this->m_capacity / 4;
        }

        void shrink();

        constexpr bool need_extend(int size_delta = 1) {
            return this->m_size + size_delta >= this->m_capacity;
        }

        void extend();

        void fill_tail();

        void require_valid_index(int index) const;

        static void require_valid_range(const T *elements, int count);

        static T* create_empty_container(int new_capacity) {
            return new T[new_capacity + TAIL]();
        }

        constexpr static int calc_new_capacity(int amount) {
            if (amount < DEFAULT_CAPACITY) {
                return DEFAULT_CAPACITY;
            }

            return (amount + DEFAULT_CAPACITY - 1) / DEFAULT_CAPACITY * DEFAULT_CAPACITY;
        }

        constexpr static void swap(Sequence &sequence1, Sequence &sequence2) {
            T *temp_container = sequence1.container;
            sequence1.container = sequence2.container;
            sequence2.container = temp_container;

            int temp_size = sequence1.m_size;
            sequence1.m_size = sequence2.m_size;
            sequence2.m_size = temp_size;

            int temp_capacity = sequence1.m_capacity;
            sequence1.m_capacity = sequence2.m_capacity;
            sequence2.m_capacity = temp_capacity;
        }

        constexpr static void copy_array(T *src, int src_capacity, T *dest, int dest_capacity) {
            for (int index = 0; index < src_capacity && index < dest_capacity; ++index) {
                dest[index] = src[index];
            }
        }
};


#include "Sequence.tpp"

#endif //SEQUENCE_H
