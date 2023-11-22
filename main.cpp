
#include <iostream>
#include <ctime>
#include <cmath>



int calculateMoonAge(const std::tm& currentDate) {
    //基準になる月を指定。
    std::tm newMoonDate ;

    //newMoonDate.tm_year = 2023 - 1900;
    //newMoonDate.tm_mon  = 1   - 1;
    //newMoonDate.tm_mday = 22;
    //newMoonDate.tm_hour = 5;
    //newMoonDate.tm_min  = 53;
    //newMoonDate.tm_sec  = 00;
    //newMoonDate.tm_isdst = -1;


    newMoonDate.tm_year = 2023 - 1900;
    newMoonDate.tm_mon  = 11   - 1;
    newMoonDate.tm_mday = 13;
    newMoonDate.tm_hour = 18;
    newMoonDate.tm_min  = 28;
    newMoonDate.tm_sec  = 00;
    newMoonDate.tm_isdst = -1;


    // 現在の日付から新月までの日数を計算
    std::time_t currentDateSeconds = std::mktime(const_cast<std::tm*>(&currentDate));
    std::time_t newMoonDateSeconds = std::mktime(const_cast<std::tm*>(&newMoonDate));

    double secondsPerDay = 60 * 60 * 24;
    double daysSinceNewMoon = difftime(currentDateSeconds, newMoonDateSeconds) / secondsPerDay;

    // 月齢の計算
    double moonAge = fmod(daysSinceNewMoon, 29.53058867);
    if (moonAge < 0) {
        moonAge += 29.53058867;
    }
    printf( "%lf\n" , moonAge );

    return static_cast<int>(moonAge);
}

int main() {
    // 現在の日付を取得
    std::time_t t = std::time(nullptr);
    std::tm currentDate = *std::localtime(&t);

    // 月齢の計算
    int moonAge = calculateMoonAge(currentDate);

    std::cout << "現在の月齢: " << moonAge << "日\n";

    return 0;
}
