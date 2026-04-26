void swap(int *a, int *b);
void print_array(int *arr, int size);

int firstMissingPositive(int* nums, int numsSize) {
    int endIdx = numsSize;

    for(int i=0; i<endIdx; i++) {
        int curr = *(nums + i);
        if(curr <= 0) {
            for(endIdx; endIdx>0; endIdx--) {
                int endItem = *(nums + endIdx - 1);
                if(endItem > 0) break;
            }
            if(i < endIdx) {
                swap((nums + i), (nums + endIdx - 1));
            }
        }
    }

    // print_array(nums, endIdx);
    int i=0;
    while(i<endIdx) {
        int curr = *(nums + i);
        if(curr <= endIdx) {
            swap((nums + i), (nums + curr - 1));
            if(*(nums + i) != curr) {
                continue;
            }
        }
        i++;
    }

    // print_array(nums, endIdx);
    for(int i=0; i<endIdx; i++) {
        int curr = *(nums + i);
        if(i+1 != curr) {
            return i+1;
        }
    }
    return endIdx + 1;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
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
