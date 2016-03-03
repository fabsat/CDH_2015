
#ifndef _DATA_SEND_RECEIVE_PROTOCOL_H
#define	_DATA_SEND_RECEIVE_PROTOCOL_H

#include <xc.h>
#include "pic_clock.h"


/* データ転送の相手先 */
typedef enum
{
    OBC2,
    COMMCU,
    POWMCU,
} destination_t;

/* 関数の処理結果のタイプ */
typedef enum
{
    SYS_ERROR   = 0,
    SYS_SUCCESS = 1,
    SYS_TIMEOUT = 2,
} sys_result_t;



/********************************************************************************
 *                          パケット関連の各種定義                                *
 *******************************************************************************/
/* パケットのdata_type一覧 */
typedef enum
{
    CW_TYPE = 0x01,
    DATA_TYPE,
    COMMAND_TYPE,
} data_type_t;



#define COMMAND 0x03
#define DATA    0x02
#define CW      0x01

/* コマンド一覧 */
#define LED_BLINK     (0x01)
#define GET_I2C_TEMP  (0x02)


/* データ終了コマンド一覧 */
typedef enum
{
    DATA_END = 0b00010000,
    DATA_CONTINUE,    
} data_end_command_t;



/********************************************************************************
 *                             CWに関する各種定義                                *
 *******************************************************************************/
/* CWで発信するデータリストのステータス保存用構造体 */
typedef struct
{
    uint8_t power1[2];
    uint8_t power2[2];
    uint8_t power3[2];
    uint8_t power4[2];
    uint8_t power5[2];
    uint8_t temp[2];
    uint8_t obc2:1;
    uint8_t commcu:1;
    uint8_t powmcu:1;
} cw_t;


/* cw_struct_t型変数の初期化用 */
#define CW_DATA_INIT  \
{                     \
	{0x00, 0x00}, \
	{0x00, 0x00}, \
	{0x00, 0x00}, \
	{0x00, 0x00}, \
	{0x00, 0x00}, \
	{0x00, 0x00}, \
	0,            \
	0,            \
	0             \
}


/* Peripheral MCU Notification Pin and I/O Setting Register */
#define OBC2_NOTIFICATION     PORTAbits.RA3
#define COMMCU_NOTIFICATION   PORTAbits.RA4
#define POWMCU_NOTIFICATION   PORTAbits.RA5
#define OBC2_NOTIF_PIN_TRIS   TRISAbits.TRISA3
#define COMMCU_NOTIF_PIN_TRIS TRISAbits.TRISA4
#define POWMCU_NOTIF_PIN_TRIS TIRSAbits.TRISA5


/* Macro for checking TIMEOUT */
#define WAIT_OR_TIMEOUT(destination_notification) \
do                                                \
{                                                 \
    timeout_counter = 1000;                       \
    while(destination_notification == 0)          \
    {                                             \
        if(timeout_counter == 0)                  \
        {                                         \
            return SYS_TIMEOUT;                   \
        }                                         \
        __delay_ms(1);                            \
        timeout_counter--;                        \
    }                                             \
} while(0)



/********************************************************************************
 *                       ライブラリ関数のプロトタイプ宣言                          *
 *******************************************************************************/

/*=====================================================
 * @brief
 *     Payloadに送信データを格納していく
 * @param
 *     p_data      :uint8_t, uint16_t, doubleの
 *                  配列へのポインタ
 *
 *     data_len    :p_dataは何データあるか
 *                  ただし以下のデータ数制限がある
 *                  uint8_tは15データ
 *                  uint16_tは10データ
 *                  doubleは6データ
 *
 *     byte_of_data:格納したいデータ型のバイト数
 *                  uint8_tなら1
 *                  uintt16_tなら2
 *                  doubleなら4
 * @return
 *     Flee space of payload:ペイロードの残り空き容量
 *     0xff:渡されたデータ全ては格納できない
 *          またはbyte_of_dataが適切な値でない
 * @note
 *     最大ペイロードサイズまで格納できる
 *===================================================*/
uint8_t sent_data_set(void *p_data, uint8_t data_len, uint8_t byte_of_data);


/*=====================================================
 * @brief
 *     PayloadへCW用データを格納する
 * @param
 *     p_cw_data:CWデータへのポインタ
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void cw_data_set(cw_t *p_cw_data);


/*=====================================================
 * @brief
 *     指定したサブシステムにデータを送信する(Master用)
 * @param
 *     destination     :送信の相手先
 *     data_type       :payloadに格納したデータのタイプ
 *     data_end_command:データはまだ継続するかどうか
 * @return
 *     void:
 * @note
 *     この関数実行後にsetしたデータ内容は初期化される
 *===================================================*/
void send_data_master(destination_t destination, data_type_t data_type, data_end_command_t data_end_command);



#endif	/* _DATA_SEND_RECEIVE_PROTOCOL_H */

