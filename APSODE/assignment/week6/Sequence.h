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


        void insert(int position, const T &element);

        void insert(int position, const T *elements, int amount);

        void append(const T &element);

        void append(const T *elements, int amount);

        void erase(int start, int amount = 1);

        void clear();


        Sequence& operator=(const Sequence &other);

    private:
        bool need_shrink() const {
            return this->m_capacity > DEFAULT_CAPACITY && this->m_size <= this->m_capacity / 4;
        }

        bool need_extend(int size_delta = 1) const {
            return this->m_size + size_delta >= this->m_capacity;
        }

        void shrink();

        void extend(int required);

        void fill_tail();

        void require_valid_index(int index) const;

        void requre_valid_position(int position) const;

        static void require_valid_range(const T *elements, int amount);

        static T* create_empty_container(int new_capacity) {
            return new T[new_capacity + TAIL]();
        }

        constexpr static int calc_new_capacity(int amount) {
            if (amount < DEFAULT_CAPACITY) {
                return DEFAULT_CAPACITY;
            }

            return (amount + DEFAULT_CAPACITY - 1) / DEFAULT_CAPACITY * DEFAULT_CAPACITY;
        }

        static void swap(Sequence &sequence1, Sequence &sequence2) {
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

        static void copy_array(const T *src, int src_capacity, T *dest, int dest_capacity) {
            for (int index = 0; index < src_capacity && index < dest_capacity; ++index) {
                dest[index] = src[index];
            }
        }
};


#include "Sequence.tpp"

#endif //SEQUENCE_H
