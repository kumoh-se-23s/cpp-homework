#include <iostream>
#include "DDay.h"
#include "DDaySystem.h"
#include "Utils.h"
using namespace std;

void DDaySystem::run() {
    Utils utils = Utils();
    DDay dday = DDay(0);
    bool isRun = true;

    while (isRun) {
        char input[INPUT_CHAR_ARR_LEN]{};

        cout << "<< " << dday.getDate();
        cout << dday;

        if(dday.getDDay() > 0) {
            cout << dday.plusDDay() << endl;
        } 
        else {
            cout << dday.minusDDay() << endl;
        }

        cout << ">> Move date{yyyymmdd, Tomorrow(T/t), Yesterday(Y/y)}, Set D-day(+/-int), or Quit(Q/q) : ";

        cin >> input;

        if(!utils.isValidInput(input)) {
             cout << "*** ERROR" << endl;  
        } 
        else {
            if(utils.isDDay(input, INPUT_CHAR_ARR_LEN)) {
                int inputDDay = utils.charArrParser(input, INPUT_CHAR_ARR_LEN);
                dday.setDDay(inputDDay);
            } 
            else {
                switch (toupper(input[0])) {

                    case 'T': {
                        dday.moveTomorrow();
                        break;
                    }

                    case  'Y': {
                        dday.moveYesterday();
                        break;
                    }

                    case 'Q': {
                        isRun = !isRun;
                        cout << "=== END ===" << endl;

                        break;
                    }
    
                    default: {
                        int inputDate = utils.charArrParser(input, INPUT_CHAR_ARR_LEN);

                        int inputYear = inputDate / 10000;
                        int inputMonth = (inputDate % 10000) / 100;
                        int inputDay = inputDate % 100;

                        if(!dday.setDate(inputYear,inputMonth,inputDay)) {
                            cout << "*** ERROR" << endl;    
                        } 
                        break;
                    }
                    break;
                }
            } 
        }
    }
}