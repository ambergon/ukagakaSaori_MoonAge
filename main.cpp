
//ルール
//半角数字に変換されること。
//各種引数がすべて合って











#include <windows.h>

#include <iostream>
#include <ctime>
#include <cmath>
#include <string>

#include <regex>

//#define Debug
using namespace std;

//{{{
// 汎用関数:stringをintに
int ZenToHan( string str ){
    //shiftjis
    str = regex_replace( str , regex( "�O" ) ,"0" );
    str = regex_replace( str , regex( "�P" ) ,"1" );
    str = regex_replace( str , regex( "�Q" ) ,"2" );
    str = regex_replace( str , regex( "�R" ) ,"3" );
    str = regex_replace( str , regex( "�S" ) ,"4" );
    str = regex_replace( str , regex( "�T" ) ,"5" );
    str = regex_replace( str , regex( "�U" ) ,"6" );
    str = regex_replace( str , regex( "�V" ) ,"7" );
    str = regex_replace( str , regex( "�W" ) ,"8" );
    str = regex_replace( str , regex( "�X" ) ,"9" );
    str = regex_replace( str , regex( "�@" ) ,"" );
    str = regex_replace( str , regex( " " ) ,"" );
    int res = atoi( str.c_str() );

    return res ;
}
//}}}
//{{{
// 月齢計算---------------------------------------------------------
void nowMoonAge( double& MoonAgeDoublePointer ) {

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
    double MoonAgeDouble = fmod(daysSinceNewMoon, 29.53058867);
    if (MoonAgeDouble < 0) {
        MoonAgeDouble += 29.53058867;
    }
    //printf( "A : %lf\n" , MoonAgeDouble );
    MoonAgeDoublePointer=MoonAgeDouble;
}
//}}}
//{{{
// 指定日月齢計算---------------------------------------------------
// 引数 : year/mon/day/hour/min/sec
void calcMoonAge( double& MoonAgeDoublePointer , int year , int month , int day , int hour , int min , int sec) {

    //基準になる月(新月)を指定。
    std::tm setDate;
    setDate.tm_year = year  - 1900;
    setDate.tm_mon  = month - 1;
    setDate.tm_mday = day  ;
    setDate.tm_hour = hour ;
    setDate.tm_min  = min  ;
    setDate.tm_sec  = sec  ;
    setDate.tm_isdst = -1;

    //アドカレ新月を基準にする。
    std::tm newMoonDate ;
    newMoonDate.tm_year = 2023 - 1900;
    newMoonDate.tm_mon  = 12   - 1;
    newMoonDate.tm_mday = 13;
    newMoonDate.tm_hour = 8;
    newMoonDate.tm_min  = 32;
    newMoonDate.tm_sec  = 00;
    newMoonDate.tm_isdst = -1;


    // 現在の日付から新月までの日数を計算
    std::time_t currentDateSeconds = std::mktime(const_cast<std::tm*>(&setDate));
    std::time_t newMoonDateSeconds = std::mktime(const_cast<std::tm*>(&newMoonDate));

    double secondsPerDay = 60 * 60 * 24;
    double daysSinceNewMoon = difftime(currentDateSeconds, newMoonDateSeconds) / secondsPerDay;

    // 月齢の計算
    double MoonAgeDouble = fmod(daysSinceNewMoon, 29.53058867);
    if (MoonAgeDouble < 0) {
        MoonAgeDouble += 29.53058867;
    }
    //printf( "A : %lf\n" , MoonAgeDouble );
    MoonAgeDoublePointer=MoonAgeDouble;
}
//}}}
//{{{
// チェック用main関数-----------------------------------------------
int main() {

    // 月齢の計算
    int MoonAgeInt ;
    double MoonAgeDouble ;

    nowMoonAge( MoonAgeDouble );
    MoonAgeInt = static_cast<int>(MoonAgeDouble);

    string strMoonAgeInt = to_string( MoonAgeInt );
    printf( "%s\n" , strMoonAgeInt.c_str() );

    string strMoonAgeDouble = to_string( MoonAgeDouble );
    printf( "%s\n" , strMoonAgeDouble.c_str() );

    return 0;
}
//}}}


FILE* ConsoleWindow;
typedef void* HGLOBAL;
char* resBuf;
char* SaoriDirectory;


//{{{
//Load 処理--------------------------------------------------------
extern "C" __declspec(dllexport) bool __cdecl load(HGLOBAL h, long len){
    //hにはdllまでのLogFilePathが入っている。
    //lenはアドレスの長さ。\0の分は入っていない。

    //loadの長さが不明の時点で下手にfreeするのは危険な気がするな。
#ifdef Debug
    AllocConsole();
    //標準出力(stdout)を新しいコンソールに向ける
    freopen_s(&ConsoleWindow, "CONOUT$", "w+", stdout);
    //標準エラー出力(stderr)を新しいコンソールに向ける
    freopen_s(&ConsoleWindow, "CONOUT$", "w+", stderr);
    //文字コードをutf-8に変更。
    //satoriはshift-jis決め打ちである。
    //system( "chcp 65001 > /nul" );
#endif

    SaoriDirectory = (char*)malloc(sizeof(char) * (len + 1 ));
    memset( SaoriDirectory , '\0' , ( len + 1 ));
    memcpy( SaoriDirectory , (char*)h , (int)len  );
    GlobalFree( h );

    printf( "load Saori MoonAge\n" );
    return true;
}
//}}}
//{{{
//Unload 処理------------------------------------------------------
extern "C" __declspec(dllexport) bool __cdecl unload(void){
    printf( "unload Saori MoonAge\n" );

    free( SaoriDirectory );

#ifdef Debug
    FreeConsole();
#endif

    return true;
}
//}}}



//曰く、最大2.2日のずれが生じるそうな。
//目的は創作における現実との大体の一致だから私は構わないが、正確なモノを期待する人は使うべきではないだろう。
//また、月齢であり、現実で月が見えているかどうかはまた別問題である。
extern "C" __declspec(dllexport) HGLOBAL __cdecl request(HGLOBAL h, long *len){
    char req[*len+1];
    memset( req , '\0' , *len+1 );
    memcpy( req , (char*)h , *len );
    GlobalFree( h );
//#ifdef Debug
//    printf( "%s----\n" , req );
//#endif


    bool NOTIFY = false;
    resBuf = NULL;

    char* Sender    = NULL;

    char* Argument0 = NULL;
    char* Argument1 = NULL;
    char* Argument2 = NULL;
    char* Argument3 = NULL;
    char* Argument4 = NULL;
    char* Argument5 = NULL;

    char  sepLine[]    = "\r\n";
    char  sepLR[] = ": ";
    char* tp ;
    tp = strtok( req , sepLine );
    while( tp != NULL ){
        if ( tp == "GET Version SAORI/1.0" ){
            tp = strtok( NULL , sepLine );
            continue;
        }

        //左右分割を試みる。
        char* checkR = strstr( tp , sepLR );
        if ( checkR == NULL ){
            tp = strtok( NULL , sepLine );
            continue;
        }


        int Lsize = strlen( tp ) - strlen( checkR ) ;
        char L[ Lsize + 1 ] ;
        memset( L , '\0' , Lsize + 1 );
        memcpy( L , tp , Lsize);

        int Rsize = strlen( tp ) - ( Lsize + strlen( sepLR ) );
        
        if ( strcmp( L , "Sender" ) == 0 ) {
            Sender = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Sender , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Argument0" ) == 0 ) {
            Argument0 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Argument0 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Argument1" ) == 0 ) {
            Argument1 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Argument1 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Argument2" ) == 0 ) {
            Argument2 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Argument2 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Argument3" ) == 0 ) {
            Argument3 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Argument3 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Argument4" ) == 0 ) {
            Argument4 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Argument4 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Argument5" ) == 0 ) {
            Argument5 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Argument5 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        
        //} else if ( strcomp( L , "" ) == 0 ) {
        }
        tp = strtok( NULL , sepLine );
    }




    // 月齢の計算
    int MoonAgeInt ;
    double MoonAgeDouble ;
    if ( Argument0 != NULL && Argument1 != NULL && Argument2 != NULL && Argument3 != NULL && Argument4 != NULL && Argument5 != NULL ){

        int Arg0 = ZenToHan( Argument0 );
        int Arg1 = ZenToHan( Argument1 );
        int Arg2 = ZenToHan( Argument2 );
        int Arg3 = ZenToHan( Argument3 );
        int Arg4 = ZenToHan( Argument4 );
        int Arg5 = ZenToHan( Argument5 );
        calcMoonAge( MoonAgeDouble , Arg0 , Arg1 , Arg2 , Arg3 , Arg4 , Arg5 );

    } else {
        nowMoonAge( MoonAgeDouble );
    }

    MoonAgeInt = static_cast<int>(MoonAgeDouble);

    string strMoonAgeInt = to_string( MoonAgeInt );
    printf( "%s\n" , strMoonAgeInt.c_str() );

    string strMoonAgeDouble = to_string( MoonAgeDouble );
    printf( "%s\n" , strMoonAgeDouble.c_str() );


    string Res;
    Res = "SAORI/1.0 200 OK\r\nValue0: " + strMoonAgeInt + "\r\nValue1: " + strMoonAgeDouble + "\r\n\r\n";
    //Res = "SAORI/1.0 200 OK\r\nResult: " + + "\r\n\r\n";

    int i = strlen( Res.c_str() );
    char* res_buf;
    res_buf = (char*)calloc( i + 1 , sizeof(char) );
    memcpy( res_buf , Res.c_str() , i );

    resBuf = res_buf;



    //Saoriにおいて、Charsetはshift-jis決め打ちである。
    //返すものがなかった時
    if ( resBuf == NULL ){
        //char res_buf[] = "SAORI/1.0 204 No Content\r\nCharset: UTF-8\r\n\r\n";
        char res_buf[] = "SAORI/1.0 204 No Content\r\n\r\n";
        resBuf = res_buf;
    }
    if ( Sender != NULL ){ free( Sender ); }

    if ( Argument0 != NULL ){ free( Argument0 ); }
    if ( Argument1 != NULL ){ free( Argument1 ); }
    if ( Argument2 != NULL ){ free( Argument2 ); }
    if ( Argument3 != NULL ){ free( Argument3 ); }
    if ( Argument4 != NULL ){ free( Argument4 ); }
    if ( Argument5 != NULL ){ free( Argument5 ); }
    
    //pluginは2.0で返す。
    //char res_buf[] = "SAORI/1.0 204 No Content";
    *len = strlen(resBuf);
    HGLOBAL ret = GlobalAlloc(GPTR, (SIZE_T)(*len));
    memcpy(ret, (void*)(resBuf), *len);
    return ret;
}
























