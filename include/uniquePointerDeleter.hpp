#ifndef UNIQUE_PTR_DELETER
#define UNIQUE_PTR_DELETER

#include <memory>

template <typename T>
using unique_ptr_deleter = std::unique_ptr<T, void (*)(T *)>;

#endif //! UNIQUE_PTR_DELETER