#ifndef __GPS_BD_H__
#define __GPS_BD_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define TCI_DATA_SIZE 256 // 缓存的数量
#define MSG_MAX_LEN 256	  // 每一条数据的大小

// 北斗数据编码格式
typedef enum
{
	GBK = 1,	   /*汉字*/
	ASICC = 2,	   /*ASICC编码*/
	GBK_ASICC = 3, /*汉字和ASICC编码混合编码*/
	CPS_GBK = 4,   /*压缩汉字*/
	CPS_ASICC = 5, /*压缩ASICC编码*/

} bd_coding_t;
// 北斗频点，频率通道
typedef enum
{
	LF0 = 1, /*TCQ 小于120个汉字*/
	LF1 = 2, /*TCQ 小于1000个汉字*/
	LF2 = 3, /*TCQ 小于1000个汉字*/
	LF3 = 4, /*TCQ 小于1000个汉字*/
	LF4 = 5, /*TCQ 小于40个汉字*/

} bd_fre_t;

typedef enum
{
	BD_F_TCQ_OK = 1U << 0, /**发送数据 */
	BD_F_CIV_OK = 1U << 1, /**民用定位使能 */
	BD_F_MSS_OK = 1U << 2, /**设置频点完成 */
	BD_F_RMC = 1U << 3,	   /**获取位置OK */
	BD_F_TCI = 1U << 4,	   /**接收数据 */
	BD_F_SIM_ID = 1U << 5,	   /**sim 卡ID */
	BD_PWI_START_NUM = 1U << 6,	   /**搜到的星星数量 */

} BD_FLAG_T;
typedef struct
{
	int hour;
	int min;
	int sec;
	int ms;
	float latitude;		// 纬度
	char latitude_dir;	// 纬度方向
	float longitude;	// 经度
	char longitude_dir; // 经度方向
	int status;
	float hoop;			 //
	float elevation;	 // 海拔高度
	char elevation_unit; // 海拔高度单位
	int star_num;		 // 卫星数量
	int error;			 // 解析结果
	int error_cnt;		 // 解析结果次数
} bd_gga_data_t;

typedef struct
{
	int mode;
	int hour;
	int min;
	int sec;
	int ms;
	int day;
	int mon;
	int year;
	int local;
	int error; // 解析结果

} bd_zda_data_t;
typedef struct
{

	int hour;
	int min;
	int sec;
	int ms;
	char send_type[8];
	char result;
	int fail_reason;
	int remain_sec;
	int error; // 解析结果

} bd_fki_data_t;

/**
 * @brief 终端接收报文结构
 *
 */
typedef struct
{
	int send_id;
	int recv_id;
	int fre;
	int hour;
	int min;
	int sec;
	int ms;
	int conding;
	uint8_t data[TCI_DATA_SIZE];
	char data_len;
	int error; // 解析结果

} bd_tci_data_t;

/**
 * @brief 载噪比
 *
 */

typedef struct
{

	int hour;
	int min;
	int sec;
	int ms;
	int sig_num;/*解析出的星数量*/
	int sig_db_ary[100];/**星星信号强度 */
	int error; // 解析结果

} bd_pwi_t;

/**
 * @brief BDRMC报文结构
 *
 */
typedef struct
{

	int hour;
	int min;
	int sec;
	int ms;
	char pos_is_ok;		// A:有效定位，V无效定位
	float latitude;		// 纬度
	char latitude_dir;	// 纬度方向 N北半球；S南半球
	float longitude;	// 经度
	char longitude_dir; // 经度方向 E东经；W西经
	float speed;		// 对地速度
	float dir;			// 地面航向
	int day;
	int mon;
	int year;
	uint32_t tick;
	int error; // 解析结果

} bd_rmc_data_t;


/**
 * @brief 中断发送报文结构
 *
 */
typedef struct
{
	int recv_id;
	int fre;
	int requst;
	int conding;
	char data[TCI_DATA_SIZE];
	int send_time;
} bd_tcq_data_t;

typedef struct
{
	char *str_data;
	bd_gga_data_t gga;
	bd_zda_data_t zda;
	bd_fki_data_t fki;
	bd_tci_data_t tci;
	bd_rmc_data_t rmc;
	bd_pwi_t pwi;
	rt_uint32_t sim_id;
	rt_uint32_t star_cov_num;//可以发送报文的卫星数据
	rt_uint32_t star_b1i_num;//可以定位卫星数据
	rt_uint32_t star_b3i_num;//可以定位卫星数据
	uint8_t bd_error;
	BD_FLAG_T flag;
} bd_data_t;

/**
 * @brief 解析北斗数据同时检查校验数据
 *
 * @param  rx_buffer
 * @param  rx_length
 */
void parse_check_bd_data(const char *rx_buffer, const rt_uint32_t rx_length);

/**
 * @brief 创建发送报文
 *
 * @param data 发送报文数据
 * @param send_str 发送报文
 * @return int
 * $CCTCQ,4238473,2,1,3,123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890,0*7B
 */
int create_tcq_data_pack(char *outbuff, const int outbuff_len, const char *send_str, uint32_t recv_id);

/**
 * @brief Get the beidou data object
 *
 * @return const bd_data_t*
 */
const bd_data_t *get_const_bd_data(void);
/**
 * @brief 北斗的事件处理函数
 *
 */
typedef void (*bd_event_callback)(uint32_t event, void *user);
void register_bd_event_callback(bd_event_callback callback);
#endif
