#pragma once

#include "Database.hpp"
#include "error_code.hpp"

namespace SQLite3
{
    /* Initializes SQLite3 */
    error_code init();
    /* Terminates SQLite3 */
    error_code shutdown();
} // namespace SQLite3
