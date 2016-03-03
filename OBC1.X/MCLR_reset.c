/******************************************************************************************
 * MCLR_reset.h
 * ver1.00
 * Tetsuya Kaku
 *=========================================================================================
 * MCLRリセットソースファイル
 *
 *=========================================================================================
 * ・ver1.00 || 2015/03/04 || Tetsuya Kaku
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
#include "MCLR_reset.h"



/*=====================================================
 * @brief
 *     MCLRリセットの初期化
 * @param
 *     void:
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void MCLR_init(void)
{
    uint8_t *nin;
    nin = 0x09;     // PORTEのレジスタアドレスをポインタ変数に格納

    /* MCLRリセットに使用する各I/Oピンを出力に設定 */
    OBC2_MCLR = 0;
    COM_MCLR  = 0;
    POW_MCLR  = 0;

    /* 信号をHighにしてリセットOff */
    *nin = 0b00000111;
}


/*=====================================================
 * @brief
 *     MCLRリセットを実行
 * @param
 *     resetted_mcu:リセットされるMCUを選択
 *                  ヘッダファイルのマクロを使用
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void MCLR_reset(uint8_t resetted_mcu)
{  
    uint8_t *nin;
    nin = 0x09;

    /* リセット実行 */
    *nin = 0x00;      // これだと全てリセットされてしまう。。。
    __delay_ms(10);
    *nin = resetted_mcu;
}
