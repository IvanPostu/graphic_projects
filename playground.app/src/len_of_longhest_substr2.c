#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define UPPER_BOUND 50001

#define u16 unsigned short int

void init_arr(unsigned short int* arr, int len) {
    for (int i = 0; i < len; i++) {
        *(arr + i) = UPPER_BOUND;
    }
}

int lengthOfLongestSubstring(char* s) {
    u16 map[255];
    init_arr(map, 255);

    u16 left = 0;
    u16 max_len = 0;

    for (u16 right = 0; *(right + s) != '\0'; right++) {
        char c = *(right + s);
        unsigned char c_idx = (unsigned char)c;

        if (map[c_idx] != UPPER_BOUND && map[c_idx] >= left) {
            left = map[c_idx] + 1;
        }

        map[c_idx] = right;
        u16 cur_window = right - left + 1;
        max_len = MAX(max_len, cur_window);
    }

    return max_len;
}
