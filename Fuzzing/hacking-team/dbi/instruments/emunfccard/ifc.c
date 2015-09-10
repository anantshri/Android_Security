/*
 *  Collin's Binary Instrumentation Tool/Framework for Android
 *  Collin Mulliner <collin[at]mulliner.org>
 *  http://www.mulliner.org/android/
 *
 *  (c) 2012
 *
 *  License: GPL v2
 *
 */

#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>       
#include <unistd.h>
#include <dlfcn.h>
#include <poll.h>

#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "phNfcStatus.h"
#include "phNfcTypes.h"
#include "phLibNfc.h"
#include "phFriNfc_NdefMap.h"
#include "phDal4Nfc_messageQueueLib.h"

#include "util.h"
#include "libt.h"

#include "hijacks.h"

#define ANDROID_4

pthread_t thread_control;

phLibNfc_RemoteDevList_t *rdevl;
phNfc_sRemoteDevInformation_t *di = NULL;

unsigned char *fake_card;
int fake_card_len;

void ifc_call_cb()
{
	struct special_phLibNfc_RemoteDev_NtfRegister_t *d = hook_phLibNfc_RemoteDev_NtfRegister.data;
	
	if (d->orig_cb != NULL) {
		log("%s callback pointer is filled\n", __func__)
		
		phLibNfc_NtfRegister_RspCb_t cb = d->orig_cb;
		
		log("di = %x rdevl = %x\n", di, rdevl)
		
		d->RemoteDevList->psRemoteDevInfo->SessionOpened = di->SessionOpened;
		d->RemoteDevList->psRemoteDevInfo->RemDevType = di->RemDevType;
		d->RemoteDevList->hTargetDev = rdevl->hTargetDev;
		log("before memcpy\n")
		memcpy((char*)&d->RemoteDevList->psRemoteDevInfo->RemoteDevInfo.Iso14443A_Info, 
	(char*)&d->RemoteDevList->psRemoteDevInfo->RemoteDevInfo.Iso14443A_Info, 
	sizeof(struct phNfc_sIso14443AInfo_t));
	
		#ifdef ANDROID_4_dis
		uint8_t uNofRemoteDev = 2;
	    NFCSTATUS  Status = 0x36; //NFCSTATUS_MULTIPLE_PROTOCOLS
		#else
		#warning " ----------------- build for android < 4 --------------------- "
		uint8_t uNofRemoteDev = 1;
	    NFCSTATUS  Status = 0; //NFCSTATUS_SUCCESS;
		#endif
		
		log("%s calling phLibNfc_RemoteDev_NtfRegister callback\n", __func__)
		my_cb_phLibNfc_RemoteDev_NtfRegister(d->pContext, rdevl, uNofRemoteDev, Status);
	}
}

extern int coms;

void ifc_thread(void *data)
{
	log("thread start\n")
	status_wait = 0;
	
	for (;;) {
		if (coms == -1) {
			start_coms();
			continue;
		}
			
		log("%s sleeping... %d\n", __func__, status_wait)
		
		struct pollfd pfd[1];
		pfd[0].fd = coms;
         pfd[0].events = POLLIN|POLLPRI|POLLERR;
		int pfn = poll(pfd, 1, 10000);
		//log("pfn %d\n", pfn)
		
		if (pfd[0].revents & POLLIN == POLLIN || pfd[0].revents & POLLPRI == POLLPRI) {
			char cmdp[4096] = {0};
			int rd = read(coms, cmdp, sizeof(cmdp));
		
			if (rd <= 0) {
				log("coms read error\n")
				close(coms);
				coms = -1;
				continue;
			}
			
			log("coms rd = %d\n", rd);
			int i;
			for (i = 0; i < rd; i++) {
				if (i%16 == 0) log("\n")
				log("%0.2x", cmdp[i]);
			}
			log("\n");
			
			fake_card = malloc(rd);
			memcpy(fake_card, cmdp, rd);
			fake_card_len = rd;
			
			log("fake card state 0->1 (via pty)\n")
			fake_card_state = 1;
			// trigger call to ifc_call_cb from other thread
			wrapper.msg.eMsgType = 0xffffff00;
			ptr_phDal4Nfc_msgsnd = msgsend;
			ptr_phDal4Nfc_msgsnd(global_msqid, (struct msgbuf *)&wrapper, sizeof(phLibNfc_Message_t), 0);
			
		}
		if (pfd[0].revents & POLLERR == POLLERR) {
			log("pollerr\n")
			close(coms);
			coms = -1;
			continue;
		}
		if (pfd[0].revents & POLLHUP == POLLHUP) {
			log("pollhup\n")		
		}
	}
}

int ifc_start()
{
	pthread_create(&thread_control, NULL, ifc_thread, NULL);
}
