/******************************************************************************************
 * plati_temp.c
 * ver1.00
 * Tetsuya Kaku
 *=========================================================================================
 * 白金薄膜温度センサデータ取得ソース・ファイル
 *
 *=========================================================================================
 * ・ver1.00 || 2016/01/14 || Tetsuya Kaku
 *   初版
 *=========================================================================================
 * PIC16F877A
 * MPLAB X IDE(v3.10/Win)
 * XC8 (v1.35/Win)
 *=========================================================================================
 * Created by fabsat Project(Tokai university Satellite Project[TSP])
 *****************************************************************************************/
#include <xc.h>
#include <pic16f877a.h>
#include "pic_clock.h"
#include "AD_value.h"
#include "plati_temp.h"


/*-----------------------------------------------------
 * @breif
 *     AD変換した値から温度データに変換する関数
 * @param
 *     なし
 * @return
 *     白金薄膜温度データ
 * @note
 *     plati_temp.c内で使用する関数なのでstatic関数
 *---------------------------------------------------*/
static double get_pt(void)
{  
    double r1, r2, t1, t2;
    double a = 0.0039083;
    double b = -0.0000005775;
   
    //温度算出の計算
    r1 = (get_adcon() / (5.0 - get_adcon())) * 2400.0;       //5.0Vを2.4kΩで分圧しているため
    t1 = (r1 / 100.0 - 1.0) / a;
    r2 = ((t1 * b + a) * t1 +1.0) * 100.0;   // t1 * b + aでOK??
    t2 = (r1 / 100.0 - r2 / 100.0) / a + t1;

    return t2;
}


/*=====================================================
 * @brief
 *     温度データ取得関数
 * @param
 *     なし
 * @return
 *     平均温度データ
 * @note
 *     温度を取得する関数
 *===================================================*/
double get_temp(void)
{
    double t_average = 0.0;
    double t_rowdata = 0.0;
    int i;
    
    //温度データの平均化
    for(i = 0; i <= 300; i++){
        t_rowdata += get_pt();
    }
    
    t_average = t_rowdata / 300.0;

    return t_average;
}

