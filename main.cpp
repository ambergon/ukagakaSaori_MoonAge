
#include <windows.h>

#include <iostream>
#include <ctime>
#include <cmath>
//stringstream
#include <iomanip>
#include <string>


//#define Debug
using namespace std;
//{{{
//月齢計算----------------------------------------------------------
void calcMoonAge( double& MoonAgeDoublePointer ) {

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
//チェック用main関数
int main() {

    // 月齢の計算
    int MoonAgeInt ;
    double MoonAgeDouble ;

    calcMoonAge( MoonAgeDouble );
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
//Load 処理---------------------------------------------------------
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
//Unload 処理-------------------------------------------------------
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

    //{{{
    //char* Argument0 = NULL;
    //char* Argument1 = NULL;
    //char* Argument2 = NULL;
    //char* Argument3 = NULL;
    //char* Argument4 = NULL;
    //char* Argument5 = NULL;
    //char* Argument6 = NULL;
    //char* Argument7 = NULL;
    //}}}

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
        //char R[ Rsize + 1 ] ;
        //memset( R , '\0' , Rsize + 1 );
        //memcpy( R , &tp[ Lsize + strlen( sepLR ) ] , Rsize);

        //printf( "%s\n" , L );
        //printf( "%s\n" , R );
        
        if ( strcmp( L , "Sender" ) == 0 ) {
            Sender = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Sender , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        //今回引数は受けない。
        //{{{
        //} else if ( strcmp( L , "Argument0" ) == 0 ) {
        //    Argument0 = (char*)calloc( Rsize + 1 , sizeof(char) );
        //    memcpy( Argument0 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        //} else if ( strcmp( L , "Argument1" ) == 0 ) {
        //    Argument1 = (char*)calloc( Rsize + 1 , sizeof(char) );
        //    memcpy( Argument1 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        //} else if ( strcmp( L , "Argument2" ) == 0 ) {
        //    Argument2 = (char*)calloc( Rsize + 1 , sizeof(char) );
        //    memcpy( Argument2 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        //} else if ( strcmp( L , "Argument3" ) == 0 ) {
        //    Argument3 = (char*)calloc( Rsize + 1 , sizeof(char) );
        //    memcpy( Argument3 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        //} else if ( strcmp( L , "Argument4" ) == 0 ) {
        //    Argument4 = (char*)calloc( Rsize + 1 , sizeof(char) );
        //    memcpy( Argument4 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        //} else if ( strcmp( L , "Argument5" ) == 0 ) {
        //    Argument5 = (char*)calloc( Rsize + 1 , sizeof(char) );
        //    memcpy( Argument5 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        //} else if ( strcmp( L , "Argument6" ) == 0 ) {
        //    Argument6 = (char*)calloc( Rsize + 1 , sizeof(char) );
        //    memcpy( Argument6 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        //} else if ( strcmp( L , "Argument7" ) == 0 ) {
        //    Argument7 = (char*)calloc( Rsize + 1 , sizeof(char) );
        //    memcpy( Argument7 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );
        //}}}
        
        //} else if ( strcomp( L , "" ) == 0 ) {
        }
        tp = strtok( NULL , sepLine );
    }


    // 月齢の計算
    int MoonAgeInt ;
    double MoonAgeDouble ;

    calcMoonAge( MoonAgeDouble );
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

    //{{{
    //if ( Argument0 != NULL ){ free( Argument0 ); }
    //if ( Argument1 != NULL ){ free( Argument1 ); }
    //if ( Argument2 != NULL ){ free( Argument2 ); }
    //if ( Argument3 != NULL ){ free( Argument3 ); }
    //if ( Argument4 != NULL ){ free( Argument4 ); }
    //if ( Argument5 != NULL ){ free( Argument5 ); }
    //if ( Argument6 != NULL ){ free( Argument6 ); }
    //if ( Argument7 != NULL ){ free( Argument7 ); }
    //}}}
    
    //pluginは2.0で返す。
    //char res_buf[] = "SAORI/1.0 204 No Content";
    *len = strlen(resBuf);
    HGLOBAL ret = GlobalAlloc(GPTR, (SIZE_T)(*len));
    memcpy(ret, (void*)(resBuf), *len);
    return ret;
}
























