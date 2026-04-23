/*
 * @Author: car12
 * @Date: 2024-05-28 11:21:42
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-08-29 17:44:20 ulog_tag_lvl  BD 3
 * @Description: 定位数据的获取和解析
 */
#include <rtthread.h>
#define DBG_TAG "BD"
#define DBG_LVL BD_LOG_LVL
#include <rtdbg.h>

#ifdef BRD_GPS_BD

#define DBG_TAG "BD"

#define BD_LOG_E LOG_E
#define BD_LOG_V LOG_I
#define BD_LOG_W LOG_W
#define BD_LOG_D LOG_D
#define BD_LOG_RAW ulog_raw
#define BD_LOG_HEX(STR, ...) ulog_hex(DBG_TAG STR, 128, __VA_ARGS__)

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <gps_bd.h>
#include <time.h>
#include <ulog.h>

static bd_data_t bd_data;
static bd_event_callback event_callback = RT_NULL;

static void bd_print_gga(const bd_gga_data_t *data)
{
	BD_LOG_V("----------------------------gga------------------------");
	BD_LOG_RAW(" time 	 = %02d:%02d:%02d.%d\r\n", data->hour, data->min, data->sec, data->ms);
	BD_LOG_RAW(" latitude 	 = %f dir = %c\r\n", data->latitude, data->latitude_dir);
	BD_LOG_RAW(" longitude 	 = %f dir = %c\r\n", data->longitude, data->longitude_dir);
	BD_LOG_RAW(" status 	 = %d star_num = %d\r\n", data->status, data->star_num);
	BD_LOG_RAW(" hoop 	 = %f elevation = %f %c\r\n", data->hoop, data->elevation, data->elevation_unit);
	BD_LOG_V("----------------------------gga------------------------");
}
static int parse_gga_data(const char *data, bd_gga_data_t *gga)
{
	// 解析位置数据
	// BD_LOG_V("%s", data->str_data);
	gga->error = sscanf(data, "$BDGGA,%02d%02d%02d.%d,%f,%c,%f,%c,%d,%d,%f,%f,%c",
						&gga->hour, &gga->min, &gga->sec, &gga->ms,
						&gga->latitude, &gga->latitude_dir,
						&gga->longitude, &gga->longitude_dir,
						&gga->status, &gga->star_num,
						&gga->hoop, &gga->elevation, &gga->elevation_unit);
	if (gga->error != 13)
	{
		BD_LOG_V("parse num = %d [%s]", gga->error, data);
		return 1;
	}
	else
	{
		return 0;
	}
}

static void bd_print_zda(const bd_zda_data_t *data)
{
	BD_LOG_V("----------------------------zda------------------------");
	BD_LOG_RAW(" date = %d/%d/%d\r\n", data->year, data->mon, data->day);
	BD_LOG_RAW(" time 	 = %02d:%02d:%02d.%d\r\n", data->hour, data->min, data->sec, data->ms);
	BD_LOG_RAW(" mode 	 = %d local = %d\r\n", data->mode, data->local);
	BD_LOG_V("----------------------------zda------------------------");
}
static int parse_zda_data(const char *str, bd_zda_data_t *zda)
{
	// 解析位置数据
	zda->error = sscanf(str, "$BDZDA,%d,%02d%02d%02d.%d,%d,%d,%d,%d",
						&zda->mode,
						&zda->hour, &zda->min, &zda->sec, &zda->ms,
						&zda->day, &zda->mon, &zda->year, &zda->local);
	if (zda->error != 9)
	{
		BD_LOG_W("parse num = %d %s", zda->error, str);
		return 1;
	}
	else
	{
		return 0;
	}
}

static void bd_print_fki(const bd_fki_data_t *data)
{
	BD_LOG_V("----------------------------zda------------------------");
	BD_LOG_RAW(" time 	 = %02d:%02d:%02d.%d\r\n", data->hour, data->min, data->sec, data->ms);
	BD_LOG_RAW(" type 	 = %s \r\n", data->send_type);
	BD_LOG_RAW(" result 	 = %d \r\n", data->result);
	BD_LOG_RAW(" fail_reason 	 = %d \r\n", data->fail_reason);
	BD_LOG_RAW(" remain_sec 	 = %d \r\n", data->remain_sec);
	BD_LOG_V("----------------------------zda------------------------");
}
static int parse_fki_data(const char *str, bd_fki_data_t *fki)
{
	// 解析位置数据
	// BD_LOG_V("%s", data->str_data);
	fki->error = sscanf(str, "$BDFKI,%02d%02d%02d.%d,%c%c%c,%c,%d,%d",
						&fki->hour, &fki->min, &fki->sec, &fki->ms,
						fki->send_type, fki->send_type + 1, fki->send_type + 2,
						&fki->result, &fki->fail_reason, &fki->remain_sec);
	fki->send_type[3] = 0;
	if (fki->error != 9)
	{
		BD_LOG_W("parse num = %d %s", fki->error, str);
		return 1;
	}
	else
	{
		return 0;
	}
}

static void bd_print_cno(const uint32_t star_b3i_num, const uint32_t star_b1i_num)
{
	BD_LOG_V("-------star num b3i = %d,b1i =%d------", star_b3i_num, star_b1i_num);
}
/**
 * @brief
$BDCNO,B1I,23,1,35,2,41,3,42,60,47,6,41,59,39,7,40,9,38,10,37,11,37,25,38,20,39,37,44,16,42,32,43,23,47,28,38,43,38,39,44,40,44,41,32,56,44,49,38*7D
$BDCNO,B3I,11,6,34,60,34,37,32,56,33,16,32,39,35,23,40,40,36,32,35,11,32,59,32*4F

 *
 * @param  str解析定位的卫星数量
 * @param  star_num
 * @return int
 */
static int parse_cno_data(const char *str, uint32_t *star_b3i_num, uint32_t *star_b1i_num)
{
	// 解析位置数据
	RT_ASSERT(str);
	RT_ASSERT(star_b3i_num);
	RT_ASSERT(star_b1i_num);
	if (RT_NULL != rt_strstr(str, "B1I"))
	{
		sscanf(str, "$BDCNO,B1I,%d", star_b1i_num);
	}
	else if (RT_NULL != rt_strstr(str, "B3I"))
	{
		sscanf(str, "$BDCNO,B3I,%d", star_b3i_num);
	}
	return 0;
}

static void bd_print_cov(const uint32_t star_cov_num)
{
	BD_LOG_V("---------star_cov_num =%d----------------", star_cov_num);
}
/**
 * @brief
$BDCOV,1036,286176.000,00*66
 *
 * @param  str解析短报文的卫星数量
 * @param  star_num
 * @return int
 */
static int parse_cov_data(const char *str, uint32_t *star_cov_num)
{
	// 解析位置数据
	RT_ASSERT(str);
	RT_ASSERT(star_cov_num);
	uint32_t week_cnt, week_sec;

	sscanf(str, "$BDCOV,%d,%d,%d", &week_cnt, &week_sec, star_cov_num);

	return 0;
}

static void bd_print_tci(const bd_tci_data_t *data)
{
	BD_LOG_V("----------------------------tci------------------------");
	BD_LOG_RAW(" time 	 = %02d:%02d:%02d.%d\r\n", data->hour, data->min, data->sec, data->ms);
	BD_LOG_RAW(" send_id 	 = %d \r\n", data->send_id);
	BD_LOG_RAW(" recv_id 	 = %d \r\n", data->recv_id);
	BD_LOG_RAW(" conding 	 = %d \r\n", data->conding);
	BD_LOG_RAW(" len 	 = %d \r\n", data->data_len);
	BD_LOG_HEX("data ", data->data, data->data_len);
	BD_LOG_V("----------------------------tci------------------------");
}

static int parse_tci_data(const char *str, bd_tci_data_t *tci)
{
	static char buff[512];
	// 解析位置数据
	rt_memset(tci, 0, sizeof(bd_tci_data_t));
	rt_memset(buff, 0, 512);
	BD_LOG_W("bd tci recv %s", str);
	tci->error = sscanf(str, "$BDTCI,%u,%u,%u,%02d%02d%02d,%d,%s*",
						&tci->send_id, &tci->recv_id, &tci->fre,
						&tci->hour, &tci->min, &tci->sec,
						&tci->conding, buff);
	if (tci->error != 8)
	{
		BD_LOG_W("parse num = %d", tci->error);
		return 1;
	}
	else
	{
		// 这里需要移除*和校验
		int str_len = rt_strlen(buff);
		int i = 0;
		for (; i < str_len; i++)
		{
			if (buff[i] == '*')
			{
				break;
			}
		}
		tci->data_len = i;
		if (tci->data_len <= TCI_DATA_SIZE && 0 == (tci->data_len & 0x01))
		{
			// rt_memcpy(tci->data, buff, i);
			tci->data_len = tci->data_len >> 1;
			for (int i = 0; i < tci->data_len; i++)
			{
				rt_sscanf(buff + i * 2, "%02X", tci->data + i);
				tci->data[i] &= 0xff;
			}
			ulog_hex("tci hex", 128, tci->data, tci->data_len);
			return 0;
		}
		return 2;
	}
}
/**
 * @brief 打印输出rmc数据信息，内部包含日期位置数据
 * @param  data             My Param doc
 */
static void bd_print_rmc(const bd_rmc_data_t *data)
{
	BD_LOG_V("----------------------------rmc------------------------");
	BD_LOG_RAW(" date time	 = %02d/%02d/%02d %02d:%02d:%02d.%d\r\n", data->year, data->mon, data->day, data->hour, data->min, data->sec, data->ms);
	BD_LOG_RAW(" latitude 	 = %f dir = %c\r\n", data->latitude, data->latitude_dir);
	BD_LOG_RAW(" longitude 	 = %f dir = %c\r\n", data->longitude, data->longitude_dir);
	BD_LOG_V("----------------------------rmc------------------------");
}

static int parse_rmc_data(const char *str, bd_rmc_data_t *rmc)
{
	// 解析位置数据
	rmc->error = sscanf(str, "$BDRMC,%02d%02d%02d.%03d,%c,%f,%c,%f,%c,%f,%f,%02d%02d%02d",
						&rmc->hour, &rmc->min, &rmc->sec, &rmc->ms, &rmc->pos_is_ok,
						&rmc->latitude, &rmc->latitude_dir, &rmc->longitude, &rmc->longitude_dir,
						&rmc->speed, &rmc->dir,
						&rmc->day, &rmc->mon, &rmc->year); // 3+2+4+4

	// bd_print_rmc(rmc);
	if (rmc->error != 14)
	{
		rmc->tick = (RT_UINT32_MAX >> 1);
		BD_LOG_D("rmc parse num = %d %s ", rmc->error, str);
		return 1;
	}
	else
	{
		rmc->year += 2000;
		rmc->tick = rt_tick_get();
		return 0;
	}
}

static void bd_print_pwi(const bd_pwi_t *data)
{
	BD_LOG_V("----------------------------pwi------------------------");
	BD_LOG_RAW(" date time	 = %02d:%02d:%02d.%d\r\n", data->hour, data->min, data->sec, data->ms);
	BD_LOG_RAW(" sig_num 	 = %f dir = %c\r\n", data->sig_num);
	BD_LOG_RAW("pwi[");
	for (int i = 0; i < data->sig_num - 4 && i < 100; i++)
	{
		BD_LOG_RAW("%d ", data->sig_db_ary[i]);
	}
	BD_LOG_RAW("]\n");
	BD_LOG_V("----------------------------pwi------------------------");
}

static int parse_pwi_data(const char *str, bd_pwi_t *pwi)
{
	// 解析位置数据
	pwi->sig_num = sscanf(str, "$BDPWI,%02d%02d%02d.%03d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,",
						  &pwi->hour, &pwi->min, &pwi->sec, &pwi->ms,
						  pwi->sig_db_ary + 0, pwi->sig_db_ary + 1, pwi->sig_db_ary + 2, pwi->sig_db_ary + 3,
						  pwi->sig_db_ary + 4, pwi->sig_db_ary + 5, pwi->sig_db_ary + 6, pwi->sig_db_ary + 7,
						  pwi->sig_db_ary + 8, pwi->sig_db_ary + 9, pwi->sig_db_ary + 10, pwi->sig_db_ary + 11,
						  pwi->sig_db_ary + 12, pwi->sig_db_ary + 13, pwi->sig_db_ary + 14, pwi->sig_db_ary + 15,
						  pwi->sig_db_ary + 16, pwi->sig_db_ary + 17, pwi->sig_db_ary + 18, pwi->sig_db_ary + 19,
						  pwi->sig_db_ary + 20, pwi->sig_db_ary + 21, pwi->sig_db_ary + 22, pwi->sig_db_ary + 23,
						  pwi->sig_db_ary + 24, pwi->sig_db_ary + 25, pwi->sig_db_ary + 26, pwi->sig_db_ary + 27,
						  pwi->sig_db_ary + 28, pwi->sig_db_ary + 29, pwi->sig_db_ary + 30, pwi->sig_db_ary + 31,
						  pwi->sig_db_ary + 32, pwi->sig_db_ary + 33, pwi->sig_db_ary + 34, pwi->sig_db_ary + 35,
						  pwi->sig_db_ary + 36, pwi->sig_db_ary + 37, pwi->sig_db_ary + 38, pwi->sig_db_ary + 39); //

	return 1;
}
static void bd_print_sim_id(const uint32_t sim_id)
{
	BD_LOG_V("-------sim id = %d ---------", sim_id);
}
/**
 * @brief =
 *
 * $BDICP,4238473,0,0,3,0,N,2,N,1,0,0,3,1,60,2,0,0,0,0,21,0,0,0*48

 *
 * @param  str
 * @param  simid
 * @return int
 */
static int parse_simid_data(const char *str, rt_uint32_t *simid)
{
	// 解析位置数据
	if (1 == sscanf(str, "$BDICP,%d,", simid))
	{
		bd_print_sim_id(*simid);
		return 0;
	}

	return 1;
}

static uint8_t xor_check(char *str, int len)
{

	uint8_t val = 0;
	for (int i = 0; i < len; i++)
	{
		val ^= str[i];
	}
	return val & 0xff;
}

/**
 * @brief 创建发送报文
 *
 * @param data 发送报文数据
 * @param send_str 发送报文
 * @return int
 * $CCTCQ,4238473,2,1,3,123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890,0*7B
 */
int create_tcq_data_pack(char *outbuff, const int outbuff_len, const char *send_str, uint32_t recv_id)
{
	assert(send_str);
	assert(outbuff);

	// rt_memset(outbuff, 0, 512);
	outbuff[0] = '$';
	rt_snprintf(outbuff + 1, outbuff_len,
				"CCTCQ,%d,%d,%d,%d,%s,0",
				recv_id, 2, 1, 2, send_str);
	int data_len = rt_strlen(outbuff);
	uint8_t check = xor_check(outbuff + 1, data_len);
	rt_sprintf(outbuff + data_len, "*%02X\r\n", check);

	return 0;
}

int bd_printf_info(void)
{
	bd_data_t *data = &bd_data;
	bd_print_gga(&data->gga);
	bd_print_zda(&data->zda);
	bd_print_tci(&data->tci);
	bd_print_rmc(&data->rmc);
	bd_print_fki(&data->fki);
	bd_print_pwi(&data->pwi);
	bd_print_sim_id(data->sim_id);
	bd_print_cno(data->star_b3i_num, data->star_b1i_num);
	bd_print_cov(data->star_cov_num);
	return 0;
}
MSH_CMD_EXPORT(bd_printf_info, print beidou parse result);
/**
 * @brief
 *
 * @param  data        gps数据，
 * @return int
 */
int parse_data_type(const char *str, bd_data_t *data)
{
	int result = 0;

	if (rt_strstr(str, "GGA")) /*定位信息*/
	{
		parse_gga_data(str, &data->gga);
	}
	else if (rt_strstr(str, "ZDA")) /*时间、日期、时区信息，*/
	{
		parse_zda_data(str, &data->zda);
	}
	else if (rt_strstr(str, "BDTCI")) /*接收报文*/
	{
		result = parse_tci_data(str, &data->tci);
		if (0 == result && event_callback)
		{
			event_callback(BD_F_TCI, 0);
		}
		else
		{
			LOG_W("TCI parse fail error %d", result);
		}
	}
	else if (rt_strstr(str, "RMC")) /*位置信息*/
	{
		if (!parse_rmc_data(str, &data->rmc)) // 解析OK
		{

			if (event_callback)
			{
				event_callback(BD_F_RMC, 0);
			}
		}
	}
	else if (rt_strstr(str, "FKI")) /*tci数据发送结果*/
	{
		parse_fki_data(str, &data->fki);
	}
	else if (rt_strstr(str, "PWI")) /*卫星再载噪比*/
	{
		parse_pwi_data(str, &data->pwi);

		if (event_callback)
		{
			event_callback(BD_PWI_START_NUM, 0);
		}
	}
	else if (rt_strstr(str, "BDICP")) /*sim ID*/
	{
		parse_simid_data(str, &data->sim_id);
		if (event_callback)
		{
			event_callback(BD_F_SIM_ID, &data->sim_id);
		}
	}
	else if (rt_strstr(str, "COV")) /*报文卫星数量*/
	{
		parse_cov_data(str, &data->star_cov_num);
	}
	else if (rt_strstr(str, "CNO")) /*定位卫星数量*/
	{
		parse_cno_data(str, &data->star_b3i_num, &data->star_b1i_num);
	}

	else
	{
	}

	return 0;
}
/**
 * @brief Get the beidou data object
 *
 * @return const bd_data_t*
 */
const bd_data_t *get_const_bd_data(void)
{
	return &bd_data;
}

/**
 * @brief 解析北斗数据同时检查校验数据
 *
 * @param  rx_buffer
 * @param  rx_length
 */
void parse_check_bd_data(const char *rx_buffer, const rt_uint32_t rx_length)
{
	static char cmd[256];
	int index = 0;

	rt_memset(cmd, 0, 256);

	for (int i = 0; i < rx_length; i++)
	{
		if (rx_buffer[i] == '$')
		{
			cmd[0] = rx_buffer[i];
			index = 1;
		}
		else if (rx_buffer[i] == '\n')
		{

			uint8_t xor_val = 0;
			uint8_t old_ck = 0;
			for (int x = 1; x < index; x++)
			{
				if ('*' == cmd[x])
				{
					char ch_str[3] = {cmd[x + 1], cmd[x + 2], 0};
					rt_sscanf(ch_str, "%02X", &old_ck);
					break;
				}
				xor_val ^= cmd[x];
			}
			if (xor_val == old_ck)
			{
				/**解析北斗数据 */
				parse_data_type(cmd, &bd_data);
			}
			else
			{
				if (rt_strstr(cmd, "TCQ OK!")) /*发送数据OK*/
				{
					bd_data.flag |= BD_F_TCQ_OK;
					if (event_callback)
					{
						event_callback(BD_F_TCQ_OK, 0);
					}
					BD_LOG_V("BD TCQ IS OK");
				}
				else if (rt_strstr(cmd, "CIV OK!")) /*民用定时使能CCCIV*/
				{
					bd_data.flag |= BD_F_CIV_OK;
					if (event_callback)
					{
						event_callback(BD_F_CIV_OK, 0);
					}
					LOG_I("SET CCCIV OK");
				}
				else if (rt_strstr(cmd, "MSS OK!")) /*频点使能CCMSS*/
				{
					bd_data.flag |= BD_F_MSS_OK;
					if (event_callback)
					{
						event_callback(BD_F_MSS_OK, 0);
					}
					LOG_I("SET CCMSS OK");
				}
			}
			// 输出北斗详细信息
			//    BD_LOG_D("ch = %02x  old_ck %02x str=%s", xor_val, old_ck, cmd);
			index = 0;
			rt_memset(cmd, 0, 256);
		}
		else
		{
			cmd[index++] = rx_buffer[i];
		}
	}
}

void register_bd_event_callback(bd_event_callback callback)
{
	event_callback = callback;
}
#endif // BRD_GPS_BD
