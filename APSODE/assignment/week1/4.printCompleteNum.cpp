#include <iostream>


int main() {
    int loop_amount;
    std::cin >> loop_amount;

    for(int loop = 0; loop < loop_amount; ++loop) {
        int dan_amount;
        int times_amount;
        int gugudan_block_width;

        scanf_s("%d %d %d", &dan_amount, &times_amount, &gugudan_block_width);
        
        for (int block_layer_lev = 0; block_layer_lev < gugudan_block_width; ++block_layer_lev) {
            int current_dan_start = (block_layer_lev * gugudan_block_width) + 1;
            int current_dan_end = current_dan_start + gugudan_block_width;

            for(int current_times = 1; current_times <= times_amount; ++current_times) {
                for(int current_dan = current_dan_start; current_dan < current_dan_end && current_dan <= dan_amount; ++current_dan) {
                    int current_result = current_dan * current_times;
                    printf("%3d * %3d = %3d    ", current_dan, current_times, current_result);
                }
                printf("\n\n");
            }
        }

    }

    return 0;
}
