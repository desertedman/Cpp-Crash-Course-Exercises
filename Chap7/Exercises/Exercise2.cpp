#include <cstddef>
#include <cstdio>
#include <new>

static const size_t n_heap_buckets{10};
constexpr const int MEGABYTE = 1000000;

struct Bucket
{
  const static size_t data_size{4096};
  std::byte data[data_size];
};

struct LargeBucket : Bucket
{
  const static size_t data_size{MEGABYTE};
  std::byte data[data_size];
};

struct Heap
{
  void *allocate(size_t bytes)
  {
    Bucket *bucket_ptr{nullptr};
    bool *bucket_used_ptr{nullptr};
    bool isLargeBucket{false};

    // Trying to allocate more than maximum bucket size
    if (bytes > LargeBucket::data_size)
    {
      throw std::bad_alloc{};
    }

    else if (bytes > Bucket::data_size)
    {
      // Allocate large bucket
      bucket_ptr = &large_buckets[0];
      bucket_used_ptr = &large_bucket_used[0];
      isLargeBucket = true;
    }

    else
    {
      bucket_ptr = &bucket_ptr[0];
      bucket_used_ptr = &bucket_used[0];
    }

    // Search for free bucket and return it
    for (size_t i{}; i < n_heap_buckets; i++)
    {
      if (!bucket_used_ptr[i])
      {
        bucket_used_ptr[i] = true;

        if (isLargeBucket)
        {
          printf("Large bucket allocated\n");
          return &large_buckets[i].data;
        }

        else
        {
          printf("Small bucket allocated\n");
          return &buckets[i].data;
        }
      }
    }

    // All buckets used
    throw std::bad_alloc{};
  }

  void free(void *p)
  {
    bool *bucket_used_ptr = nullptr;

    // Search for desired bucket address and free it
    bool found{false};
    for (size_t i{}; i < n_heap_buckets; i++)
    {
      Bucket *bucket_ptr = &buckets[i];
      LargeBucket *large_bucket_ptr = &large_buckets[i];

      if (bucket_ptr->data == p)
      {
        printf("Small bucket freed\n");
        bucket_used_ptr = &bucket_used[i];
        found = true;
      }

      else if (large_bucket_ptr->data == p)
      {
        printf("Large bucket freed\n");
        bucket_used_ptr = &large_bucket_used[i];
        found = true;
      }

      if (found)
      {
        bucket_used_ptr[i] = false;
        return;
      }
    }
  }

  Bucket buckets[n_heap_buckets];
  LargeBucket large_buckets[n_heap_buckets];

  bool bucket_used[n_heap_buckets];
  bool large_bucket_used[n_heap_buckets];
};

Heap heap;

// n_bytes is the size of the type being allocated
void *operator new(size_t n_bytes)
{
  return heap.allocate(n_bytes);
}

void *operator new[](size_t n_bytes)
{
  return heap.allocate(n_bytes);
}

void operator delete(void *p) noexcept
{
  return heap.free(p);
}

void operator delete[](void *p) noexcept
{
  heap.free(p);
}

int main()
{
  printf("Buckets: %p\n", heap.buckets);

  auto breakfast = new unsigned int{0xC0FFEE};
  auto dinner = new unsigned int{0XDEADBEEF};

  printf("Breakfast: %p 0x%x\n", breakfast, *breakfast);
  printf("Dinner: %p 0x%x\n", dinner, *dinner);

  delete breakfast;
  delete dinner;

  auto largeAlloc = new char[50000]; // 50 KB > 4096, so LargeBucket should be used

  for (size_t i = 0; i < 10; ++i)
  {
    largeAlloc[i] = 'A' + i;
  }

  delete[] largeAlloc;
}
