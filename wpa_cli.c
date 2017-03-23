/*
 * WPA Supplicant - command line interface for wpa_supplicant daemon
 * Copyright (c) 2004-2011, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#include "utils/includes.h"
#define CONFIG_CTRL_IFACE
#define CONFIG_CTRL_IFACE_UNIX
#ifdef CONFIG_CTRL_IFACE

#ifdef CONFIG_CTRL_IFACE_UNIX
#include <dirent.h>
#endif /* CONFIG_CTRL_IFACE_UNIX */

#include "common/wpa_ctrl.h"
#include "utils/common.h"
#include "utils/eloop.h"
#include "utils/edit.h"
#include "common/version.h"

#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

static struct wpa_ctrl *ctrl_conn;
static struct wpa_ctrl *mon_conn;
static int wpa_cli_quit = 0;
static int wpa_cli_attached = 0;
static int wpa_cli_connected = 0;
static int wpa_cli_last_id = 0;
#ifndef CONFIG_CTRL_IFACE_DIR
#define CONFIG_CTRL_IFACE_DIR "/var/run/wpa_supplicant"
#endif /* CONFIG_CTRL_IFACE_DIR */
static const char *ctrl_iface_dir = CONFIG_CTRL_IFACE_DIR;
static char *ctrl_ifname = NULL;
static const char *pid_file = NULL;
static const char *action_file = NULL;
//static int ping_interval = 5;
static int interactive = 0;
char s_r[4096]={0};

static void wpa_cli_mon_receive(int sock, void *eloop_ctx, void *sock_ctx);


static int str_starts(const char *src, const char *match)
{
	return os_strncmp(src, match, os_strlen(match)) == 0;
}


static int wpa_cli_show_event(const char *event)
{
	const char *start;

	start = os_strchr(event, '>');
	if (start == NULL)
		return 1;

	start++;
	/*
	 * Skip BSS added/removed events since they can be relatively frequent
	 * and are likely of not much use for an interactive user.
	 */
	if (str_starts(start, WPA_EVENT_BSS_ADDED) ||
	    str_starts(start, WPA_EVENT_BSS_REMOVED))
		return 0;

	return 1;
}


static int wpa_cli_open_connection(const char *ifname, int attach)
{
#if defined(CONFIG_CTRL_IFACE_UDP) || defined(CONFIG_CTRL_IFACE_NAMED_PIPE)
	ctrl_conn = wpa_ctrl_open(ifname);
	if (ctrl_conn == NULL)
		return -1;

	if (attach && interactive)
		mon_conn = wpa_ctrl_open(ifname);
	else
		mon_conn = NULL;
#else /* CONFIG_CTRL_IFACE_UDP || CONFIG_CTRL_IFACE_NAMED_PIPE */
	char *cfile = NULL;
	int flen, res;

	if (ifname == NULL)
		return -1;

	if (cfile == NULL) {
		flen = os_strlen(ctrl_iface_dir) + os_strlen(ifname) + 2;
		cfile = os_malloc(flen);
		if (cfile == NULL)
			return -1;
		res = os_snprintf(cfile, flen, "%s/%s", ctrl_iface_dir,
				  ifname);
		if (res < 0 || res >= flen) {
			os_free(cfile);
			return -1;
		}
	}

	ctrl_conn = wpa_ctrl_open(cfile);
	if (ctrl_conn == NULL) {
		os_free(cfile);
		return -1;
	}

	if (attach && interactive)
		mon_conn = wpa_ctrl_open(cfile);
	else
		mon_conn = NULL;
	os_free(cfile);
#endif /* CONFIG_CTRL_IFACE_UDP || CONFIG_CTRL_IFACE_NAMED_PIPE */

	if (mon_conn) {
		if (wpa_ctrl_attach(mon_conn) == 0) {
			wpa_cli_attached = 1;
			if (interactive)
				eloop_register_read_sock(
					wpa_ctrl_get_fd(mon_conn),
					wpa_cli_mon_receive, NULL, NULL);
		} else {
			printf("Warning: Failed to attach to "
			       "wpa_supplicant.\n");
			return -1;
		}
	}

	return 0;
}


static void wpa_cli_close_connection(void)
{
	if (ctrl_conn == NULL)
		return;

	if (wpa_cli_attached) {
		wpa_ctrl_detach(interactive ? mon_conn : ctrl_conn);
		wpa_cli_attached = 0;
	}
	wpa_ctrl_close(ctrl_conn);
	ctrl_conn = NULL;
	if (mon_conn) {
		eloop_unregister_read_sock(wpa_ctrl_get_fd(mon_conn));
		wpa_ctrl_close(mon_conn);
		mon_conn = NULL;
	}
}


static void wpa_cli_msg_cb(char *msg, size_t len)
{
	printf("%s\n", msg);
}


static int _wpa_ctrl_command(struct wpa_ctrl *ctrl, char *cmd, int print)
{
	//Modified by Kurt; enlarge command string for scan_result
	char buf[4096];
	size_t len;
	int ret;

	if (ctrl_conn == NULL) {
		printf("Not connected to wpa_supplicant - command dropped.\n");
		return -1;
	}
	len = sizeof(buf) - 1;
	ret = wpa_ctrl_request(ctrl, cmd, os_strlen(cmd), buf, &len,
			       wpa_cli_msg_cb);
	if (ret == -2) {
		printf("'%s' command timed out.\n", cmd);
		return -2;
	} else if (ret < 0) {
		printf("'%s' command failed.\n", cmd);
		return -1;
	}
	if (print) {
		buf[len] = '\0';
	//	printf("%s", buf);  //ycl
		if (interactive && len > 0 && buf[len - 1] != '\n')
			printf("\n");
	}
        memset(s_r,0,sizeof(s_r));
        memcpy(s_r,buf,strlen(buf));
	return 0;
}


static int wpa_ctrl_command(struct wpa_ctrl *ctrl, char *cmd)
{
	return _wpa_ctrl_command(ctrl, cmd, 1);
}

static int str_match(const char *a, const char *b)
{
	return os_strncmp(a, b, os_strlen(b)) == 0;
}


static int wpa_cli_exec(const char *program, const char *arg1,
			const char *arg2)
{
	char *cmd;
	size_t len;
	int res;
	int ret = 0;

	len = os_strlen(program) + os_strlen(arg1) + os_strlen(arg2) + 3;
	cmd = os_malloc(len);
	if (cmd == NULL)
		return -1;
	res = os_snprintf(cmd, len, "%s %s %s", program, arg1, arg2);
	if (res < 0 || (size_t) res >= len) {
		os_free(cmd);
		return -1;
	}
	cmd[len - 1] = '\0';
#ifndef _WIN32_WCE
	if (system(cmd) < 0)
		ret = -1;
#endif /* _WIN32_WCE */
	os_free(cmd);

	return ret;
}


static void wpa_cli_action_process(const char *msg)
{
	const char *pos;
	char *copy = NULL, *id, *pos2;

	pos = msg;
	if (*pos == '<') {
		/* skip priority */
		pos = os_strchr(pos, '>');
		if (pos)
			pos++;
		else
			pos = msg;
	}

	if (str_match(pos, WPA_EVENT_CONNECTED)) {
		int new_id = -1;
		os_unsetenv("WPA_ID");
		os_unsetenv("WPA_ID_STR");
		os_unsetenv("WPA_CTRL_DIR");

		pos = os_strstr(pos, "[id=");
		if (pos)
			copy = os_strdup(pos + 4);

		if (copy) {
			pos2 = id = copy;
			while (*pos2 && *pos2 != ' ')
				pos2++;
			*pos2++ = '\0';
			new_id = atoi(id);
			os_setenv("WPA_ID", id, 1);
			while (*pos2 && *pos2 != '=')
				pos2++;
			if (*pos2 == '=')
				pos2++;
			id = pos2;
			while (*pos2 && *pos2 != ']')
				pos2++;
			*pos2 = '\0';
			os_setenv("WPA_ID_STR", id, 1);
			os_free(copy);
		}

		os_setenv("WPA_CTRL_DIR", ctrl_iface_dir, 1);

		if (!wpa_cli_connected || new_id != wpa_cli_last_id) {
			wpa_cli_connected = 1;
			wpa_cli_last_id = new_id;
			wpa_cli_exec(action_file, ctrl_ifname, "CONNECTED");
		}
	} else if (str_match(pos, WPA_EVENT_DISCONNECTED)) {
		if (wpa_cli_connected) {
			wpa_cli_connected = 0;
			wpa_cli_exec(action_file, ctrl_ifname, "DISCONNECTED");
		}
	} else if (str_match(pos, P2P_EVENT_GROUP_STARTED)) {
		wpa_cli_exec(action_file, ctrl_ifname, pos);
	} else if (str_match(pos, P2P_EVENT_GROUP_REMOVED)) {
		wpa_cli_exec(action_file, ctrl_ifname, pos);
	} else if (str_match(pos, P2P_EVENT_CROSS_CONNECT_ENABLE)) {
		wpa_cli_exec(action_file, ctrl_ifname, pos);
	} else if (str_match(pos, P2P_EVENT_CROSS_CONNECT_DISABLE)) {
		wpa_cli_exec(action_file, ctrl_ifname, pos);
	} else if (str_match(pos, WPS_EVENT_SUCCESS)) {
		wpa_cli_exec(action_file, ctrl_ifname, pos);
	} else if (str_match(pos, WPS_EVENT_FAIL)) {
		wpa_cli_exec(action_file, ctrl_ifname, pos);
	} else if (str_match(pos, WPA_EVENT_TERMINATING)) {
		printf("wpa_supplicant is terminating - stop monitoring\n");
		wpa_cli_quit = 1;
	}
}

static void wpa_cli_reconnect(void)
{
	wpa_cli_close_connection();
	wpa_cli_open_connection(ctrl_ifname, 1);
}


static void wpa_cli_recv_pending(struct wpa_ctrl *ctrl, int action_monitor)
{
	if (ctrl_conn == NULL) {
		wpa_cli_reconnect();
		return;
	}
	while (wpa_ctrl_pending(ctrl) > 0) {
		char buf[256];
		size_t len = sizeof(buf) - 1;
		if (wpa_ctrl_recv(ctrl, buf, &len) == 0) {
			buf[len] = '\0';
			if (action_monitor)
				wpa_cli_action_process(buf);
			else {
				if (wpa_cli_show_event(buf)) {
					edit_clear_line();
					printf("\r%s\n", buf);
					edit_redraw();
				}
			}
		} else {
			printf("Could not read pending message.\n");
			break;
		}
	}

	if (wpa_ctrl_pending(ctrl) < 0) {
		printf("Connection to wpa_supplicant lost - trying to "
		       "reconnect\n");
		wpa_cli_reconnect();
	}
}

#define max_args 10



static void wpa_cli_mon_receive(int sock, void *eloop_ctx, void *sock_ctx)
{
	wpa_cli_recv_pending(mon_conn, 0);
}


static void wpa_cli_cleanup(void)
{
	wpa_cli_close_connection();
	if (pid_file)
		os_daemonize_terminate(pid_file);

	os_program_deinit();
}

static void wpa_cli_terminate(int sig)
{
	wpa_cli_cleanup();
	exit(0);
}


static char * wpa_cli_get_default_ifname(void)
{
	char *ifname = NULL;

#ifdef CONFIG_CTRL_IFACE_UNIX
	struct dirent *dent;
	DIR *dir = opendir(ctrl_iface_dir);
	if (!dir) {
		return NULL;
	}
	while ((dent = readdir(dir))) {
		if (os_strcmp(dent->d_name, ".") == 0 ||
		    os_strcmp(dent->d_name, "..") == 0)
			continue;
//		printf("Selected interface '%s'\n", dent->d_name);
		ifname = os_strdup(dent->d_name);
		break;
	}
	closedir(dir);
#endif /* CONFIG_CTRL_IFACE_UNIX */

	return ifname;
}




int hushan_wifi(char *cmd,char *result)
{
//	int warning_displayed = 0;
//	int c;
	int daemonize = 0;
	int ret = 0;
	const char *global = NULL;
        ctrl_iface_dir="/var/run/wpa_supplicant";
	if (os_program_init())
		return -1;
	if (eloop_init())
		return -1;
	signal(SIGINT, wpa_cli_terminate);
	signal(SIGTERM, wpa_cli_terminate);
	if (ctrl_ifname == NULL)
	{
		ctrl_ifname = wpa_cli_get_default_ifname();
        
        if (!global &&wpa_cli_open_connection(ctrl_ifname, 0) < 0) 
        {
	     perror("Failed to connect to wpa_supplicant -wpa_ctrl_open");
	     return -1;
        }
	}

	if (daemonize && os_daemonize(pid_file))
		return -1;
	//ret = wpa_request(ctrl_conn, num, cmd);
//	printf("cmd=%s\n",cmd);
	wpa_ctrl_command(ctrl_conn, cmd);
	os_free(ctrl_ifname);
	eloop_destroy();
	wpa_cli_cleanup();
    strncpy(result,s_r,strlen(s_r));
    ctrl_ifname = NULL;
	return ret;



}

//int wifi_init();


# if 0
int reset_config()
{

    if(client.total_topic>0)
    {
	   client.is_pc=1;
	}
    if(client.seat==0)
    {
    	client.seat=3;
    }
    if(get_len(client.ssid)<2)
    {
    	 cpy_to_volatile(client.psk,
    				  (unsigned char *)"hushanwuxian",strlen("hushanwuxian"));
         cpy_to_volatile(client.ssid,
    				  (unsigned char *)"HUSHAN_5G_1",strlen("HUSHAN_5G_1"));
    }
	 client.volume=110;   //默认采集音量值
	 client.date=0x7fffffff;
	 client.wifi_status=0;
	 client.mic_onoff=0;
	 client.horn_onoff=0;
	 client.vport=0;
	 client.did=0;
	 memset_volatile(client.dip,0,sizeof(client.dip));
	 client.port=0;
	 client.mode=0;
	 client.elec_value=0;
	 client.mport=0;
	 client.voice=2;
	 client.speaker=2;
	 heart_count=0;
	 led_blue_on();
	 change_brightness(7);
	 return 0;
//	 volume_control();
}


int check_wifi()
{

   char command[256]={0};
   FILE *fp=NULL;
   get_wifi_info_if(client.mac,client.ip);
 //  printf("get ok\n");
//   if(client.wifi_status==0)
   if(fsm.wifi_state==0)
   {
	    printf("no wifi device\n");
        memset(command,0,sizeof(command));
        snprintf(command,sizeof(command),
        	"echo 1 > /sys/class/leds/led-power/brightness");
        fp=popen(command,"r");
        if(fp==NULL)
        {
              printf("fail to set wifi power\n");
        }
        pclose(fp);
        printf("pull low\n");
        sleep(1);
        memset(command,0,sizeof(command));
        snprintf(command,sizeof(command),
            "echo 0 > /sys/class/leds/led-power/brightness");
        fp=popen(command,"r");
        if(fp==NULL)
        {
             printf("fail to set wifi power1\n");
        }
        pclose(fp);
        printf("pull high\n");
        sleep(3);
       memset(command,0,sizeof(command));
       snprintf(command,sizeof(command),
          "wpa_supplicant -Dwext -iwlan0 -c/tmp/wpa_1_8.conf -B");
      fp=popen(command,"r");
      if(fp==NULL)
      {
         printf("fail to init wifi server\n");
      }
      sleep(2);
      pclose(fp);
      char s_buf[66]={0};
      char p_buf[66]={0};
      sprintf(s_buf,"\"%s\"",client.ssid);
      sprintf(p_buf,"\"%s\"",client.psk);
      char ssid[64]={0};
      char psk[64]={0};
      char *scan="AP_SCAN 1";
      char *add="ADD_NETWORK";
   //   char *mgmt="SET_NETWORK 0 key_mgmt NONE";
      char *mgmt="SET_NETWORK 0 key_mgmt WPA-PSK";
      char *pairwise="SET_NETWORK 0 pairwise CCMP";
      char *select="SELECT_NETWORK 0";
      strcat(ssid,"SET_NETWORK 0 ssid ");
      strcat(ssid,s_buf);
     // printf("ssid=%s\n",ssid);
      strcat(psk,"SET_NETWORK 0 psk ");
      strcat(psk,p_buf);
      char str1[512]={0};
      hushan_wifi(scan,str1);
      hushan_wifi(add,str1);
    //  printf("str1=%s\n",str1);
      hushan_wifi(ssid,str1);
      hushan_wifi(mgmt,str1);
      hushan_wifi(pairwise,str1);
      hushan_wifi(psk,str1);
      hushan_wifi(select,str1);
  }

  return 0;

}

int wifi_init()
{

   printf("libwifi version: %s\n","1.0.7");
   status=-1;
   int ret=-1;
   set_volume_capture();
   init_mic_play();
   memset(&client,0,sizeof(client));
   memset(&fsm,0,sizeof(fsm));



   info_queue=creat_head();
   int res=sem_init(&que_sem,0,0);
   if(res!=0)
   {
	   printf("que_sem initialization failed \n");
   }
   printf("que_sem init\n");


   ret=read_info(&client);
   if(ret==-1)
   {
	  cpy_to_volatile(client.psk,
			  (unsigned char *)"hushanwuxian",strlen("hushanwuxian"));
	  cpy_to_volatile(client.ssid,
			  (unsigned char *)"HUSHAN_5G_1",strlen("HUSHAN_5G_1"));

//	   	  cpy_to_volatile(client.psk,
//	   			  (unsigned char *)"123123123",strlen("123123123"));
//	   	  cpy_to_volatile(client.ssid,
//	   			  (unsigned char *)"wifihuiyi_test",strlen("wifihuiyi_test"));
//
//	   cpy_to_volatile(client.ssid,
//	         (unsigned char *)"mt7612e-2077",strlen("mt7612e-2077"));
  	  client.seat=3;
  	  memset_volatile(client.ip,0,get_len(client.ip));
	  ret=write_info(&client);
  	  if(ret!=0)
  	  {
  		  printf("write error end!!!!!!!");
  		  return -1;
  	  }
    }
    memset_volatile(client.ip,0,get_len(client.ip));
    reset_config();
    check_wifi();

    get_brightness();

    init_time();
    key_thread();

//    init_horn();
  //  sleep(1);
//    printf("here\n");
    init_mic();
//    init_horn();
    init_udp();
    init_tcp();

    struct m_data temp;
    get_info(&temp);

    return 0;


}


int wifi_reconnect()
{

	char str1[512]={0};
    char *disable="ENABLE_NETWORK 0";
    hushan_wifi(disable,str1);
    client.wifi_status=1;
    client.quilty=100;
    enter_queue(info_queue,10);
    sem_post(&que_sem);
	return 0;

}

int wifi_disconnect()
{
	char str1[512]={0};
	char *disable="DISABLE_NETWORK 0";
	hushan_wifi(disable,str1);
	client.wifi_status=0;
	client.quilty=0;
	enter_queue(info_queue,10);
	sem_post(&que_sem);
    return 0;
}


#endif



#else /* CONFIG_CTRL_IFACE */
int main(int argc, char *argv[])
{
	printf("CONFIG_CTRL_IFACE not defined - wpa_cli disabled\n");
	return -1;
}
#endif /* CONFIG_CTRL_IFACE */
