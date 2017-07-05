#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "sys/wait.h"
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/cdefs.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

typedef struct conf{
	char ssid[33];
	char psk[33];
}w_conf;

typedef struct result_node{
	int len;
	char buf[512];
	struct result_node *next;
}result_node;

typedef struct result_node *presult;

w_conf info;
int enter_result(presult p,char *buf);
int out_result(presult p,char *buf);
int wifi_scan();
int read_info(w_conf *p);
int write_info(w_conf *p);
int connect_wifi();
char *get_string(unsigned char *src,char *dst);
int  hs_strcpy(char *src,char *des);
int  out_string(char *src,unsigned char *des);
int add_ip();

extern int hushan_wifi(char *cmd,char *result);

int main(int argc,char *argv[])
{
    memset(&info,0,sizeof(info));
	int ret=read_info(&info);
    if(ret!=0)
    {
    	 strncpy(info.psk,"hushanwuxian",strlen("hushanwuxian"));
  //  	 strncpy(info.ssid,"mt7612e-2077",strlen("mt7612e-2077"));
         strncpy(info.ssid,"HUSHAN_5G_1",strlen("HUSHAN_5G_1"));
         write_info(&info);
    }
    if(strlen(info.ssid)<=1)
    {
    	 strncpy(info.psk,"hushanwuxian",strlen("hushanwuxian"));
  //  	 strncpy(info.ssid,"mt7612e-2077",strlen("mt7612e-2077"));
    	 strncpy(info.ssid,"HUSHAN_5G_1",strlen("HUSHAN_5G_1"));
    	 write_info(&info);
    }
 //   printf("ssid=%s\n",info.ssid);
 //   printf("psk=%s\n",info.psk);
    connect_wifi();
//    add_ip();
    system("udhcpc -iwlan0");
    return 0;

}

presult creat_head()
{
	presult temp=(presult)(malloc(sizeof(result_node)));
	temp->len=0;
	temp->next=NULL;
	return temp;
}

int enter_result(presult p,char *buf)
{
	presult temp=(presult)(malloc(sizeof(result_node)));
	hs_strcpy(buf,p->buf);
	presult head=p;
	head->len++;
	while(p->next)
	{
		p=p->next;
	}
    temp->next=p->next;
    p->next=temp;
    return 0;
}

int out_result(presult p,char *buf)
{
	presult fp=NULL;
	if(p->next)
	{
		fp=p->next;
		hs_strcpy(fp->buf,buf);
		p->next=fp->next;
		free(fp);
	}
    return 0;
}



int connect_wifi()
{

	int encode_mode = -1;
	char command[256] = { 0 };
	char str1[2048] = { 0 };
	int ret = -1;
	int fd = -1;
	char *path = "/sys/class/leds/led-power/brightness";
	fd = open(path, O_RDWR);
	if (fd < 0)
	{
		printf("open %s error\n", path);
	}
	ret = write(fd, "1", 1);
	if (ret < 0)
	{
		printf("pull low error\n");
	}
	usleep(100000);
	ret = write(fd, "0", 1);
	if (ret < 0)
	{
		printf("pull high error\n");
	}

	FILE *fp = NULL;
	usleep(500000);
	memset(command, 0, sizeof(command));
	snprintf(command, sizeof(command),
			"wpa_supplicant -Dwext -iwlan0 -c/tmp/wpa_1_8.conf -B");
	fp = popen(command, "r");
	if (fp == NULL)
	{
		printf("fail to init wifi server\n");
	}
	sleep(2);
	pclose(fp);
	char s_buf[66] = { 0 };
	char p_buf[66] = { 0 };
	sprintf(s_buf, "\"%s\"", info.ssid);
	sprintf(p_buf, "\"%s\"", info.psk);
	char ssid[64] = { 0 };
	char psk[64] = { 0 };
//	char *scan = "AP_SCAN 1";
	char *add = "ADD_NETWORK";
	char *mgmt = NULL;
	char *pairwise = NULL;

	encode_mode = wifi_scan();
	while ((encode_mode != 1) && (encode_mode != 3))
	{
		encode_mode = wifi_scan();
		usleep(10000);
	}
	if (encode_mode == 1)
	{
		mgmt = "SET_NETWORK 0 key_mgmt WPA-PSK";
		pairwise = "SET_NETWORK 0 pairwise CCMP";

	} else if (encode_mode == 3)
	{
		mgmt = "SET_NETWORK 0 key_mgmt NONE";
	} else
	{
		printf("get encode: %d\n", encode_mode);
	}
	char *select = "SELECT_NETWORK 0";
	strcat(ssid, "SET_NETWORK 0 ssid ");
	strcat(ssid, s_buf);
	strcat(psk, "SET_NETWORK 0 psk ");
	strcat(psk, p_buf);
//	hushan_wifi(scan, str1);
	hushan_wifi(add, str1);
	hushan_wifi(ssid, str1);
	hushan_wifi(mgmt, str1);
	hushan_wifi(pairwise, str1);
	hushan_wifi(psk, str1);
	hushan_wifi(select, str1);
	return 0;
}





int wifi_scan()
{
	char str1[4096] = { 0 };
	unsigned char temp_str[512] = { 0 };
	char *p = NULL;
	char *scan = "SCAN";
	char *scan_results = "SCAN_RESULTS";
	int mode=-1;
	int rest_len = 0;

	hushan_wifi(scan, str1);
	hushan_wifi(scan_results, str1);
//	printf("str1=%s\n",str1);
	rest_len = out_string(str1, temp_str);
	while (rest_len > 0)
	{
		p = get_string(temp_str, info.ssid);
		if (p != NULL)
		{
	//		printf("the ssid find : %s\n", temp_str);
			p = get_string(temp_str, "WPA");
			if (p != NULL)
			{
				printf("encode use wpa/wpa2\n");
				mode=1;
				break;
			}
			p = get_string(temp_str, "WEP");
			if (p != NULL)
			{
				printf("encode use wep\n");
				mode=2;
				break;
			}
			p = get_string(temp_str, "ESS");
			if (p != NULL)
			{
				printf("encode use open\n");
				mode=3;
				break;
			} else
			{
				printf("error\n");
			}
		}
		memset(temp_str, 0, sizeof(temp_str));
		rest_len = out_string(str1, temp_str);
		//	break;
	}
	return mode;
}

int read_info(w_conf *p)
{
	memset(p,0,sizeof(info));
	FILE *fp=NULL;
	fp=fopen("wifi.dat","r");
	if(fp==NULL)
	{
		 printf("wifi config file not found \n");
		 return -1;
	}
	fread(p,sizeof(info),1,fp);
	printf("read ok\n");
	fclose(fp);
	return 0;

}

int write_info(w_conf *p)
{

	FILE *fp=NULL;
	fp=fopen("wifi.dat","w+");
	if(fp==NULL)
    {
		printf("write_wifi conf error\n");
        return -1;
	}
	fwrite(p,sizeof(w_conf),1,fp);
	fclose(fp);
	return 0;
}

int add_ip()
{

   char result_buf[256], command[32];
   snprintf(command,sizeof(command),"udhcpc -iwlan0");
   FILE *fp=NULL;
   fp=popen(command,"r");
   if(fp==NULL)
   {
         printf("can not get the state of wlan0\n");
         return -2;
   }
   fread(result_buf,sizeof(char),sizeof(result_buf),fp);
//   printf("result=%s\n",result_buf);
   pclose(fp);
//   status|=0x01;
   return 0;
}

char *get_string(unsigned char *src,char *dst)
{
   char *p;
   int i,j;
   for(i=0;src[i]!='\0';i++)
   {
       if(src[i]!=dst[0])
       {
           continue;
       }
       j=0;
       while(dst[j]!='\0'&&src[i+j]!='\0')
       {
            j++;
            if(dst[j]!=src[i+j])
            {
                break;
            }
       }
       if(dst[j]=='\0')
       {
            p=(char*)(src+i);
            return p;
       }
   }
   return NULL;
}

int  hs_strcpy(char *src,char *des)
{
	int i=0;
	while(src[i]!='\n'&&src[i]!='\0')
	{
		des[i]=src[i];
	//	printf("%x",src[i]);
		i++;
	}
	des[i]='\0';
	return i;
}




int out_string(char *src, unsigned char *des)
{
    int len=strlen(src);
	int i = 0, j = 0;
	char temp_str[4096] = { 0 };
	while (src[i] != '\n' && src[i] != '\0')
	{
		des[i] = src[i];
		i++;
	}
	des[i] = '\0';
	i++;
	while (src[i] != '\0')
	{
		temp_str[j] = src[i];
		j++;
		i++;
	}
	temp_str[j]='\0';
	memset(src,0,sizeof(src));
	while(j>=0)
	{
		src[j]=temp_str[j];
		j--;
	}
	return len;

}

