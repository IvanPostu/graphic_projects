#ifndef TEST_MANAGER_H
#define TEST_MANAGER_H

#define PASSED 1
#define BYPASS 2

#define u8 unsigned char
typedef u8 (*PFN_test)(void);

void test_manager_init(void);

void test_manager_register_test(PFN_test, char *desc);

u8 test_manager_run_tests(void);

void test_manager_destroy(void);

#endif
