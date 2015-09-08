/*
 * cgiOneWire.c
 *
 *  Created on: 03.09.2015
 *      Author: X-SzczyptaM
 */


#include <string.h>
#include <osapi.h>
#include "user_interface.h"
#include "mem.h"
#include "httpd.h"
#include "cgi.h"
#include "io.h"
#include "espmissingincludes.h"
#include "config.h"


#include "ds1820.h"
//Template code for the DHT 22 page.
void  cgiOneWire(HttpdConnData *connData) {
	ControllerConfig *pApp =pControllerConfig_Descriptor();
	int len;
	int r;
	int len_1;
	int len_send;
	char set_point_A_boiler[64];
	char set_point_B[64];
	char ctrl_elemnt_buff[32];
	char sensor_position;
	char set_point_value_A;
	char set_point_value_B;
	char ctrl_elemnt;
	char buff[32];

	if (connData->conn == NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}
	len = httpdFindArg(connData->post->buff, "set_point_A_boiler", set_point_A,
			sizeof(set_point_A));
	len_1= httpdFindArg(connData->post->buff, "set_point_B_boiler", set_point_B,
	
	if (len != 0 && len_1!=0) {
		set_point_value_A = atoi(set_point_A);
		set_point_value_B = atoi(set_point_B);
		r = set_Point_ds18b20(set_point_value_A ,set_point_value_B,0);
		os_printf("\r\nCGI set_point_A & B %d%d\r\n",len ,len_1);
		len_send=os_sprintf(buff, "]\n}\n}\n");
	}
	
	len = httpdFindArg(connData->post->buff, "set_point_A_stove", set_point_A,
			sizeof(set_point_A));
	len_1= httpdFindArg(connData->post->buff, "set_point_B_stove", set_point_B,
	
	
	if (len != 0 && len_1!=0) {
		set_point_value_A = atoi(set_point_A);
		set_point_value_B = atoi(set_point_B);
		r = set_Point_ds18b20(set_point_value_A ,set_point_value_B,0);
		os_printf("\r\nCGI set_point_A & B %d%d\r\n",len ,len_1);
		len_send=os_sprintf(buff, "]\n}\n}\n");
	}
	httpdSend(connData, buff, len_send);
//	espconn_sent(connData->conn, (uint8 *)buff, os_strlen(buff));
}
void  cgiOneWire_cfg(HttpdConnData *connData) {
	ControllerConfig *pApp =pControllerConfig_Descriptor();
	int len;
	int r;
	int len_send;
	char sensor[128];
	char buff[32];

	if (connData->conn == NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len = httpdFindArg(connData->post->buff, "sensor", sensor, sizeof(sensor));

	if (len != 0) {
		sensor_position = atoi(sensor);
		os_printf("\r\nCGI sensor_present %d\r\n", len);
		r = search_ds18b20(sensor_position);
		len_send=os_sprintf(buff, "]\n}\n}\n");
	}
	
	httpdSend(connData, buff, len_send);
//	espconn_sent(connData->conn, (uint8 *)buff, os_strlen(buff));
}
