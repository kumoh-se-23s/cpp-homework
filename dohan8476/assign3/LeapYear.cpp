#include <iostream>

using namespace std;


int main(){
    int year = 1, month, day, serial;
    
    cin >> serial;

    //반복 시행 1회 줄이려고 -1로 처리
    int temp = serial - 1;

    //400년(146097일) 주기 
    year += (temp / 146097) * 400;
    temp %= 146097;

    //100년(36524일) 주기
    //반복문으로 처리안하면 위 400년 조건에서 temp == 146096로 오류발생
    for(;temp - 36524 > 0; temp -= 36524, year += 100)

    //4년 (1461일) 주기
    year += (temp / 1461) * 4;
    temp %= 1461;

    //1년
    //100년과 같은 이유
    for(;temp - 365 > 0 ; temp -= 365, ++year)
    
    int nomalYearDay[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int leapYearDay[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    temp += 1;
    //뺀 값 다시 정상적으로 돌려두기

    //월 구하기
    bool isLeap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;

    for(int i = 0; i < 12 ; i++){
        month = i + 1;
        if(isLeap){
            if(temp - leapYearDay[i] <= 0){
            break;
        }
        temp -= leapYearDay[i];
        }

        else{
            if(temp - nomalYearDay[i] <= 0){
            break;
        }
        temp -= nomalYearDay[i];
        }
    }

    //자연스레 나머지는 day
    day = temp;

    cout << year << "년" << month << "월" << day << "일";
}

