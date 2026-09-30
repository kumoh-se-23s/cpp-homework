#include "Day.h"
#include <cassert>
#include <cstdint>

    Day::Day(int year, int month, int day) : year(year), month(month), day(day){
        normalize();  
    };

    Day Day::operator++(){
        if(day == getDays(year, month)){
            if(month == 12){
                ++year;
                month = 1;
                day = 1;
            }else{
                ++month;
                day = 1;
            }
        }else{
            ++day;
        }
        return *this;
    }

    
    Day Day::operator--(){
        if(day == 1){
            if(month == 1){
                --year;
                month = 12;
                day = 31;
            }else{
                day = getDays(year, --month);
            }
        }else{
            --day;
        }
        return *this;
    }

    Day Day::operator+(int d){
        return Day(year, month, day + d);
    }
    
    Day Day::operator-(int d){
        return Day(year, month, day - d);
    }

    int Day::getDays(int year, int month){
        return isLeap(year) ? (LEAP_MONTH_DAYS_SUM[month] - LEAP_MONTH_DAYS_SUM[month - 1]) : (MONTH_DAYS_SUM[month] - MONTH_DAYS_SUM[month - 1]);
    }
    
    bool Day::isLeap(int year){

        // %400 : n-400*n/400 = n-400*n*42949673>>32
        // %100 : n-100*n/100 = n-400*n*10737419>>32
        // return year % 400 == 0 || (year % 100 != 0 && year % 4 == 0)
        int m100 = year - 100 * (static_cast<int64_t>(year) * 42949673 >> 32);    
        int m400 = year - 400 * (static_cast<int64_t>(year) * 10737419 >> 32);
        return !m400 || ((year & 3) == 0 && m100);
    }

    void Day::normalize(){
        // O(1) normalization

        // assuming that month has already normalized 
        // year += (month - 12) / 12;
        // month = ((month - 1) % 12 + 12) % 12 + 1;

        day += isLeap(year) ? LEAP_MONTH_DAYS_SUM[month - 1] : MONTH_DAYS_SUM[month - 1];

        // minimum date : 0001/01/01
        // year days = year * 365 + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400
        // = ((year - 1) >> 2) - (year - 1) / 100 + (year - 1) / 100 / 4
        // = ((year - 1) >> 2) - century + (century >> 2)
        // additional days : day - 1 (1~365 => 0~364 mapping, applying day-1)

        // OH NO THAT IS FORCED-DEBUG BUILD 
        // n/100 : n * 2^32/100 >> 32 = 42949673*n >> 32
        
        int century = static_cast<int>(static_cast<int64_t>(year - 1) * 42949673 >> 32);
        int totalDays = std::max(365, year * 365 + ((year - 1) >> 2) - century + (century >> 2) + day - 1);


        // 366 or 365, unknown => execute both 365 and 366
        // n/146097 : n * 2^32/146097 >> 32 = 29399*n >> 32
        // n/36524 : n * 2^32/36524 >> 32 = 117594*n >> 32
        // n/1461 : n * 2^32/1461 >> 32 = 2939745*n >> 32

        //146000 + 97 (24 * 4 = 96, 400 is leap year. 96 + 1 = 97)
        //36500 + 24 (100 / 4 = 25, 100 is not leap year. 25 - 1 = 24)
        //1460 + 1(leaps)
        int yearCalcDays = totalDays - 365; 
        yearCalcDays -= static_cast<int>(static_cast<int64_t>(yearCalcDays) * 29399 >> 32); 
        yearCalcDays += static_cast<int>(static_cast<int64_t>(yearCalcDays) * 117594 >> 32); 
        yearCalcDays -= static_cast<int>(static_cast<int64_t>(yearCalcDays) * 2939745 >> 32); 

        int leapYearCalcDays = totalDays - 366; //re-calculation for leap years
        leapYearCalcDays -= static_cast<int>(static_cast<int64_t>(leapYearCalcDays) * 29399 >> 32); 
        leapYearCalcDays += static_cast<int>(static_cast<int64_t>(leapYearCalcDays) * 117594 >> 32); 
        leapYearCalcDays -= static_cast<int>(static_cast<int64_t>(leapYearCalcDays) * 2939745 >> 32); 

        // n/365 : n * 2^32/365 >> 32 = 11767034*n >> 32
        int year2 = static_cast<int>(static_cast<int64_t>(leapYearCalcDays) * 11767034 >> 32) + 1;
        year = yearCalcDays / 365 + 1;
        year = static_cast<int>(static_cast<int64_t>(yearCalcDays - (isLeap(year2) && year != year2)) * 11767034 >> 32) + 1; //solve 366

        // (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400
        // ((year - 1) >> 2) - (year - 1) / 100 + (year - 1) / 100 / 4
        // ((year - 1) >> 2) - century /THAT IS 100 + century / 4
        century = static_cast<int>(static_cast<int64_t>(year - 1) * 42949673 >> 32);;
        int totalDaysForYear = year * 365 + ((year - 1) >> 2) - century + (century >> 2); 
        int remainDays = totalDays - totalDaysForYear + 1;
        bool leap = isLeap(year);
        assert(remainDays >= 1 && remainDays <= (365 + leap));

        // approximate month
        int monthApprox = (remainDays >> 5) + 1;
        month = monthApprox + 1 - (remainDays <= (leap ? LEAP_MONTH_DAYS_SUM[monthApprox] : MONTH_DAYS_SUM[monthApprox]));
        day = remainDays - (leap ? LEAP_MONTH_DAYS_SUM[month - 1] : MONTH_DAYS_SUM[month - 1]);
    }

    bool Day::isValid(int y, int m, int d){    
        return y >= 1 && m >= 1 && m <= 12 && d >= 1 && d <= Day::getDays(y, m);
    }

    