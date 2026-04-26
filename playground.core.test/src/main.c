#include "expect.h"
#include "test_manager.h"
#include <stdio.h>

int sum(int a, int b) { return a + b; }

static u8 test_1_plus_2_is_3(void) {
  int result = sum(1, 2);
  expect_should_be(result, 3);
  return PASSED;
}

int main(void) {
  test_manager_init();

  test_manager_register_test(test_1_plus_2_is_3, "Dummy test sum logic");

  printf("Starting tests...");

  // Execute tests
  int result = test_manager_run_tests();

  test_manager_destroy();
  return result;
}
