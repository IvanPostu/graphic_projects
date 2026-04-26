#include "test_manager.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct test_entry {
  PFN_test func;
  char *desc;
} test_entry;

typedef struct linked_list_node {
  test_entry *data;
  struct linked_list_node *next;
} linked_list_node;

static linked_list_node origin_head = {.data = NULL, .next = NULL};
static linked_list_node *tests_tail = &origin_head;
static unsigned int tests_count = 0;

void test_manager_init(void) {}

void test_manager_register_test(u8 (*PFN_test)(void), char *desc) {
  test_entry *e = (test_entry *)malloc(sizeof(test_entry));
  e->func = PFN_test;
  e->desc = desc;

  linked_list_node *node = (linked_list_node *)malloc(sizeof(linked_list_node));
  node->data = e;
  node->next = NULL;

  tests_tail->next = node;
  tests_tail = tests_tail->next;
  tests_count++;
}

u8 test_manager_run_tests(void) {
  u8 passed = 0;
  u8 failed = 0;
  u8 skipped = 0;

  linked_list_node *curr = origin_head.next;
  while (curr != NULL) {
    u8 result = curr->data->func();
    if (result == PASSED) {
      ++passed;
      printf("[PASSED]: %s\n", curr->data->desc);
    } else if (result == BYPASS) {
      ++skipped;
      printf("[SKIPPED]: %s\n", curr->data->desc);
    } else {
      ++failed;
      printf("[FAILED]: %s\n", curr->data->desc);
    }
    curr = curr->next;
  }

  char status[256];
  if (failed) {
    snprintf(status, sizeof(status), "*** %d FAILED ***", failed);
  } else {
    snprintf(status, sizeof(status), "SUCCESS");
  }

  char final_output[256];
  snprintf(final_output, sizeof(final_output),
           "Results: %d passed, %d failed, %d skipped.", passed, failed,
           skipped);

  printf("%s\n%s\n", status, final_output);
  return failed;
}

void test_manager_destroy() {
  linked_list_node *curr = origin_head.next;
  while (curr != NULL) {
    free(curr->data);
    linked_list_node *next = curr->next;
    free(curr);
    curr = next;
  }
  origin_head.next = NULL;
}