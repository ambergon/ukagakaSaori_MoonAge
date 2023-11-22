
#include <iostream>
#include <ctime>
#include <cmath>


//曰く、最大2.2日のずれが生じるそうな。
//目的は創作における現実との大体の一致だから私は構わないが、正確なモノを期待する人は使うべきではないだろう。
//
//
//また、月齢であり、現実で月が見えているかどうかはまた別問題である。

int calculateMoonAge() {

    // 現在の日付を取得
    std::time_t t = std::time(nullptr);
    std::tm currentDate = *std::localtime(&t);

    //基準になる月(新月)を指定。
    std::tm newMoonDate ;
    //newMoonDate.tm_year = 2023 - 1900;
    //newMoonDate.tm_mon  = 1   - 1;
    //newMoonDate.tm_mday = 22;
    //newMoonDate.tm_hour = 5;
    //newMoonDate.tm_min  = 53;
    //newMoonDate.tm_sec  = 00;
    //newMoonDate.tm_isdst = -1;

    //アドカレ新月を基準にする。
    newMoonDate.tm_year = 2023 - 1900;
    newMoonDate.tm_mon  = 12   - 1;
    newMoonDate.tm_mday = 13;
    newMoonDate.tm_hour = 8;
    newMoonDate.tm_min  = 32;
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

    // 月齢の計算
    int moonAge = calculateMoonAge();

    std::cout << "現在の月齢: " << moonAge << "日\n";

    return 0;
}
