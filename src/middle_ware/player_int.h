#ifndef _PLAYER_INT_H_
#define _PLAYER_INT_H_

#define CEDARC_DEBUG 0

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <errno.h>
#include <sys/select.h>
#include <tplayer.h>

#define MSG_USRPLAYEXIT 103
#define TUNNEL_NUM 		1

enum {
	TUNNEL_VIDEO = 0,
	TUNNEL_AUDIO = 0,
	TUNNEL_CLICK = 0,
} tunnel_type_t;

typedef struct PLAYER_CONTEXT_T {
    TPlayer *mTPlayer;
    int mSeekable;
    int mError;
    int mVideoFrameNum;
    bool mPreparedFlag;
    bool mLoopFlag;
    bool mSetLoop;
    bool mCompleteFlag;
    char mUrl[512];
	int mTunnelId;
	int mVolume;
    MediaInfo *mMediaInfo;
    sem_t mPreparedSem;
} player_context_t;

int tplayer_init(TplayerVideoRotateType rotate);
int tplayer_exit(void);
int tplayer_play_url(int id, const char *parth);
int tplayer_play(int id);
int tplayer_setvolume(int id, int volume);
int tplayer_getvolume(int id);
int tplayer_pause(int id);
int tplayer_seekto(int id, int nSeekTimeMs);
int tplayer_stop(int id);
int tplayer_setlooping(int id, bool bLoop);
int tplayer_loop(int id, int bloop);
int tplayer_setscaledown(int id, TplayerVideoScaleDownType nHorizonScaleDown, \
        TplayerVideoScaleDownType nVerticalScaleDown);
int tplayer_setrotate(int id, TplayerVideoRotateType rotate);
MediaInfo * tplayer_getmediainfo(int id);
int tplayer_getduration(int id, int *msec);
int tplayer_getcurrentpos(int id, int *msec);
int tplayer_getcompletestate(int id);
int tplayer_setdisplayrect(int id, int x, int y, unsigned int width,
        unsigned int height);

#endif
