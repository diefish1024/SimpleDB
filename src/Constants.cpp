#include "Constants.hpp"
#include "Row.hpp"

namespace Constants {
    const uint32_t ROW_SIZE = sizeof(Row);
    const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
    const uint32_t MAX_PAGES = (MAX_ROWS / ROWS_PER_PAGE + 1) * 2 + 2;
}