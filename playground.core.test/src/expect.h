/**
 * @brief Expects actual to be true.
 */
#define expect_to_be_true(actual)                                              \
  if (actual != 1) {                                                           \
    printf("--> Expected true, but got: false. File: %s:%d.", __FILE__,        \
           __LINE__);                                                          \
    return 0;                                                                  \
  }

/**
 * @brief Expects expected to be equal to actual.
 */
#define expect_should_be(expected, actual)                                     \
  if (actual != expected) {                                                    \
    printf("--> Expected %d, but got: %d. File: %s:%d.", expected, actual,     \
           __FILE__, __LINE__);                                                \
    return 0;                                                                  \
  }

/**
 * @brief Expects expected to NOT be equal to actual.
 */
#define expect_should_not_be(expected, actual)                                 \
  if (actual == expected) {                                                    \
    printf("--> Expected %d != %d, but they are equal. File: %s:%d.",          \
           expected, actual, __FILE__, __LINE__);                              \
    return 0;                                                                  \
  }
