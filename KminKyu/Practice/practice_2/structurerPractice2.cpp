#include<iostream>

using namespace std;

struct DateType {
    int year, month, day;
};

int getDayOfWeek(const DateType &d);
int isValidDate(const DateType &d);
string getDateString(const DateType &d);
void readDate(DateType &d);

int main() {

    const string DAY_OF_WEEK[] = {"일", "월", "화", "수", "목", "금", "토"};

    DateType date;

    readDate(date);
}

void readDate(DateType &d) {
    cout << "년월일을 입력하세요 : ";
    cin >> d.year >> d.month >> d.day;
}

string getDateString(DateType d) {
    string str = to_string(d.year) + "년"
                + to_string(d.month) + "월"
                + to_string(d.day) + "일";
    return str;
}