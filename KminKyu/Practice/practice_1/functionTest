#include<iostream>
using namespace std;

int getFinalPrice(int numberParameter, int priceParameter, int saleRateParameter);

int main() {
    int price, saleRate, number, bill;
    cout << "개당 가격 : ";
    cin >> price;

    cout << "구매 갯수: ";
    cin >> number;

    cout << "할인율 : ";
    cin >> saleRate;

    bill = getFinalPrice(price, number, saleRate);
    cout << "총 금액 : " << bill << endl;
    return 0;
}

int getFinalPrice(int priceParameter, int numberParameter, int saleRateParameter) {
    int finalPrice;
    finalPrice = priceParameter * numberParameter * (100 - saleRateParameter) / 100;
    return finalPrice;
}