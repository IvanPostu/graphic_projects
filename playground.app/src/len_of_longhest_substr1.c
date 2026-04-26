int max(int a, int b) {
    return a > b ? a : b;
}

int set(uint64_t* buff, unsigned char idx) {
    unsigned char buff_idx = idx / 64;
    unsigned char buff_element_idx = idx % 64;

    uint64_t before = *(buff_idx + buff);
    *(buff_idx + buff) = *(buff_idx + buff) | ((uint64_t)1 << buff_element_idx);

    return *(buff_idx + buff) != before ? 1 : 0;
}

void unset(uint64_t* buff, unsigned char idx) {
    unsigned char buff_idx = idx / 64;
    unsigned char buff_element_idx = idx % 64;

    uint64_t before = *(buff_idx + buff);
    *(buff_idx + buff) = *(buff_idx + buff) & (~((uint64_t)1 << buff_element_idx));
}

int lengthOfLongestSubstring(char* s) {
    int longest_len = 0;
    int lp = 0;
    int rp = 0;
    char c = *(s);
    int i = 0;

    uint64_t buff[4] = {0, 0, 0, 0};

    while (c != '\0') {
        if (set(buff, c)) {
            rp++;
            longest_len = max(longest_len, rp - lp);
        } else {
            unset(buff, *(lp + s));
            lp++;
            continue;
        }
        c = *(++i + s);
    }
   
    return longest_len;
}
