#include <iostream>
#include <cstdint>
#include <cstdio>

uint32_t crc32_table[256];
void init_crc32() {
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t c = i;
        for (int j = 0; j < 8; j++)
            c = (c & 1) ? (0xEDB88320 ^ (c >> 1)) : (c >> 1);
        crc32_table[i] = c;
    }
}

inline uint32_t calc_crc32(const char* buf, size_t len) {
    uint32_t c = 0xFFFFFFFF;
    for (size_t i = 0; i < len; i++)
        c = crc32_table[(c ^ static_cast<uint8_t>(buf[i])) & 0xFF] ^ (c >> 8);
    return c ^ 0xFFFFFFFF;
}

// 숫자의 자릿수 반환 (0~9는 1자리, 10~99는 2자리)
inline int digits(int n) { return (n < 10) ? 1 : 2; }

int main() {
    init_crc32();
    const uint32_t TARGET_EASY  = 0x112AC98C; // Input_Easy.txt
    const uint32_t TARGET_INPUT = 0x457655E5; // Input.txt

    std::cout << "초고속 탐색 시작 (자릿수 합계 = 10 규칙 적용)...\n";
    char buf[64];

    // 도착시간(a2~a5) 탐색: 오름차순 가정 (0 ~ 30)
    for (int a2 = 0; a2 <= 20; ++a2) {
        int d_a2 = digits(a2);
        for (int a3 = a2; a3 <= 25; ++a3) {
            int d_a3 = d_a2 + digits(a3);
            if (d_a3 > 4) continue; // 이미 두 자릿수가 2개 이상이면 스킵

            for (int a4 = a3; a4 <= 30; ++a4) {
                int d_a4 = d_a3 + digits(a4);
                if (d_a4 > 5) continue;

                for (int a5 = a4; a5 <= 35; ++a5) {
                    int d_a = d_a4 + digits(a5);
                    if (d_a > 5) continue; // 도착시간 4개 중 두 자릿수는 최대 1개뿐!

                    // 실행시간(b1~b5) 탐색: 남은 자릿수 합계가 (10 - d_a)가 되어야 함
                    for (int b1 = 1; b1 <= 35; ++b1) {
                        int d_b1 = d_a + digits(b1);
                        if (d_b1 > 6) continue;

                        for (int b2 = 1; b2 <= 35; ++b2) {
                            int d_b2 = d_b1 + digits(b2);
                            if (d_b2 > 7) continue;

                            for (int b3 = 1; b3 <= 35; ++b3) {
                                int d_b3 = d_b2 + digits(b3);
                                if (d_b3 > 8) continue;

                                for (int b4 = 1; b4 <= 35; ++b4) {
                                    int d_b4 = d_b3 + digits(b4);
                                    if (d_b4 > 9) continue;

                                    // 마지막 b5의 자릿수는 (10 - d_b4)로 자동 고정됨!
                                    int needed_digits = 10 - d_b4;
                                    int b5_start = (needed_digits == 1) ? 1 : 10;
                                    int b5_end   = (needed_digits == 1) ? 9 : 40;

                                    for (int b5 = b5_start; b5 <= b5_end; ++b5) {
                                        int len = snprintf(buf, sizeof(buf),
                                            "P1, 0, %d\r\nP2, %d, %d\r\nP3, %d, %d\r\nP4, %d, %d\r\nP5, %d, %d",
                                            b1, a2, b2, a3, b3, a4, b4, a5, b5);
                                        
                                        uint32_t crc = calc_crc32(buf, 49);
                                        if (crc == TARGET_EASY) {
                                            std::cout << "\n[Input_Easy.txt 일치 데이터]\n" << buf << "\n";
                                        }
                                        if (crc == TARGET_INPUT) {
                                            std::cout << "\n[Input.txt 일치 데이터]\n" << buf << "\n";
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    std::cout << "탐색 종료.\n";
    return 0;
}