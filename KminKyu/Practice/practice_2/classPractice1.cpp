#include<iostream>
#include<cstdlib>

using namespace std;

class DayOfYear {
public:
    DayOfYear(int monthValue, int dayValue);
    DayOfYear(int monthValue);
    DayOfYear();
    void input();
    void output();
    int getMonthNumber();
    int getDay();
private:
    int month, day;
    void testDate();
};
DayOfYear::DayOfYear(int monthValue, int dayValue) : month(monthValue), day(dayValue) {
    testDate();
}
DayOfYear::DayOfYear(int monthValue) : month(monthValue), day(1) {
    testDate();
}
DayOfYear::DayOfYear() : month(1), day(1) {}

void DayOfYear::testDate() {
    if ((month < 1) || (month > 12)) {
        cout << "illegal month value!\n";
        exit(1);
    }
    if ((day < 1) || (day > 31)) {
        cout << "illegal day value!\n";
        exit(1);
    }
}
void DayOfYear::input()
{
    cout << "월과 일을 입력하세요: ";
    cin >> month >> day;
    testDate();
}

void DayOfYear::output()
{
    cout << month << "월 " << day << "일";
}

int DayOfYear::getMonthNumber()
{
    return month;
}

int DayOfYear::getDay()
{
    return day;
}

int main() {
    DayOfYear date1(2,21), date2(5), date3;
    cout << "초기화된 날짜들 : \n";
    date1.output(); cout << endl;
    date2.output(); cout << endl;
    date3.output(); cout << endl;

    date1 = DayOfYear(10,31);
    cout << "10월 31일의 재설정된 날짜는 :\n";
    date1.output(); cout << endl;
    return 0;
}