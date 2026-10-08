#include <cstdint>
#include <cstring>
#include <cmath>

template <typename T = int>
    requires std::is_trivially_copyable_v<T>
class MyArray
{

    T *raw = nullptr;
    uint64_t size = 0;
    uint64_t capacity = 0;

public:
    explicit MyArray(uint64_t capacity = 0);

    ~MyArray();

    MyArray(const MyArray &other) noexcept;
    MyArray operator=(const MyArray &other) noexcept;
    MyArray(MyArray &&other) noexcept;
    MyArray operator=(MyArray &&other) noexcept;

    void resize(uint64_t newSize, T setter = {});

    void resizeCapacity(uint64_t capacity) noexcept;

    
    void insert(int placementIndex, T v);

    void add(T v);

    T remove(uint64_t index);

    T &operator[](uint64_t index);

    const T &operator[](uint64_t index) const;

    void clear() { size = 0; }

    bool contains(T v);

    [[nodiscard]] uint64_t getSize() const { return size; }

    [[nodiscard]] uint64_t getCapacity() const { return capacity; }


    void merge(int left, int mid, int right);

    void mergeSort();

    void mergeSort(int left, int right);
};
