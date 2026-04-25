void swap(int* a, int* b);
void print_array(int* arr, int size);

int is_legit(int num, int len) {
    if (num < 1 || num > len)
        return 0;
    return 1;
}

int firstMissingPositive(int* nums, int numsSize) {
    int i = 0;
    while (i < numsSize) {
        int curr = *(nums + i);
        if (is_legit(curr, numsSize)) {
            swap((nums + i), (nums + curr - 1));
            if (*(nums + i) != curr) {
                continue;
            }
        }
        i++;
    }

    for (int i = 0; i < numsSize; i++) {
        int curr = *(nums + i);
        if (i + 1 != curr) {
            return i + 1;
        }
    }
    return numsSize + 1;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void print_array(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) {
            printf(" ");
        }
    }
    printf("\n");
}
