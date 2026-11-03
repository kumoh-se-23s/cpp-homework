#ifndef CPP_HOMEWORK_WEEK6_SEQUENCE_H
#define CPP_HOMEWORK_WEEK6_SEQUENCE_H


template<typename E, int TAIL = 1>
class Sequence {
    public:
        int size() const;

        bool empty() const;

        int capacity() const;

        const E& at(int index) const;

        E& at(int index);


        const E& operator[](int index) const;

        E& operator[](int index);

    protected:
        static constexpr int DEFAULT_CAPACITY = 8;
        static constexpr int GROWTH_FACTOR = 2;

        E *container;
        int m_capacity;
        int m_size;


        Sequence();

        Sequence(const E *elements, int size);

        Sequence(const Sequence &other);

        virtual ~Sequence();


        void insert(int position, const E &element);

        void insert(int position, const E *elements, int amount);

        void append(const E &element);

        void append(const E *elements, int amount);

        void erase(int start, int amount = 1);

        bool equals(const Sequence &other) const;

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

        static void require_valid_range(const E *elements, int amount);

        static E* create_empty_container(int new_capacity) {
            return new E[new_capacity + TAIL]();
        }

        constexpr static int calc_new_capacity(int amount) {
            if (amount < DEFAULT_CAPACITY) {
                return DEFAULT_CAPACITY;
            }

            return (amount + DEFAULT_CAPACITY - 1) / DEFAULT_CAPACITY * DEFAULT_CAPACITY;
        }

        static void swap(Sequence &sequence1, Sequence &sequence2) {
            E *temp_container = sequence1.container;
            sequence1.container = sequence2.container;
            sequence2.container = temp_container;

            int temp_size = sequence1.m_size;
            sequence1.m_size = sequence2.m_size;
            sequence2.m_size = temp_size;

            int temp_capacity = sequence1.m_capacity;
            sequence1.m_capacity = sequence2.m_capacity;
            sequence2.m_capacity = temp_capacity;
        }

        static void copy_array(const E *src, int src_capacity, E *dest, int dest_capacity) {
            for (int index = 0; index < src_capacity && index < dest_capacity; ++index) {
                dest[index] = src[index];
            }
        }
};


#include "Sequence.tpp"

#endif //CPP_HOMEWORK_WEEK6_SEQUENCE_H
