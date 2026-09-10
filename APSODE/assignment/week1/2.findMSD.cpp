#include <iostream>

int main() {
    int loop_amount;
    std::cin >> loop_amount;

    for(int loop = 0; loop < loop_amount; ++loop) {
        int input_number;
        int pos = 1;

        scanf_s("%d", &input_number);

        while(true) {
            int temp = input_number / 10;
            
            if (temp == 0) {
                break;
            }
            
            input_number = temp;
            ++pos;
        }
        
        printf("%d @ [%d]", input_number, pos);
    }

    return 0;
}
