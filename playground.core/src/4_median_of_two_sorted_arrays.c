#include "4_median_of_two_sorted_arrays.h"
#include <limits.h>
#include <stdio.h>

void print_array(int *arr, int size);
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

double findMedianSortedArrays(int *nums1, int nums1Size, int *nums2,
                              int nums2Size) {
  if (nums1Size > nums2Size) {
    return findMedianSortedArrays(nums2, nums2Size, nums1, nums1Size);
  }
  int low = 0, high = nums1Size;

  while (low <= high) {
    int partition1 = (low + high) / 2;
    int partition2 = (nums1Size + nums2Size + 1) / 2 - partition1;
    int maxL1 = partition1 == 0 ? INT_MIN : *(nums1 + partition1 - 1);
    int minR1 = partition1 == nums1Size ? INT_MAX : *(nums1 + partition1);
    int maxL2 = partition2 == 0 ? INT_MIN : *(nums2 + partition2 - 1);
    int minR2 = partition2 == nums2Size ? INT_MAX : *(nums2 + partition2);

    if (maxL1 <= minR2 && maxL2 <= minR1) {
      if ((nums1Size + nums2Size) % 2 == 1) {
        return MAX(maxL1, maxL2);
      }
      return (MAX(maxL1, maxL2) + MIN(minR1, minR2)) / 2.0;
    } else if (maxL1 > minR2) {
      high = partition1 - 1;
    } else {
      low = partition1 + 1;
    }
  }
  return -1.0;
}

void print_array(int *arr, int size) {
  for (int i = 0; i < size; i++) {
    printf("%d", arr[i]);
    if (i < size - 1) {
      printf(" ");
    }
  }
  printf("\n");
}
