#include "player_int.h"

#define CEDARX_UNUSE(param) (void)param
#define ISNULL(x) if(!x){return -1;}
#if 0
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
    MediaInfo *mMediaInfo;
    sem_t mPreparedSem;
} player_context_t;
#endif

static player_context_t gplayer[TUNNEL_NUM];

/* a callback for tplayer. */
static int CallbackForTPlayer(void *pUserData, int msg, int param0,
        void *param1) {
    player_context_t *pPlayer = (player_context_t*) pUserData;

    CEDARX_UNUSE(param1);
    switch (msg) {
    case TPLAYER_NOTIFY_PREPARED: {
        printf("[%d] TPLAYER_NOTIFY_PREPARED,has prepared.\n", pPlayer->mTunnelId);
        pPlayer->mPreparedFlag = 1;
        sem_post(&pPlayer->mPreparedSem);
        break;
    }
    case TPLAYER_NOTIFY_PLAYBACK_COMPLETE: {
        printf("[%d] TPLAYER_NOTIFY_PLAYBACK_COMPLETE\n", pPlayer->mTunnelId);
        pPlayer->mCompleteFlag = 1;
        break;
    }
    case TPLAYER_NOTIFY_SEEK_COMPLETE: {
        printf("[%d] TPLAYER_NOTIFY_SEEK_COMPLETE>>>>info: seek ok.\n", pPlayer->mTunnelId);
        break;
    }
    case TPLAYER_NOTIFY_MEDIA_ERROR: {
        switch (param0) {
        case TPLAYER_MEDIA_ERROR_UNKNOWN: {
            printf("[%d] erro type:TPLAYER_MEDIA_ERROR_UNKNOWN\n", pPlayer->mTunnelId);
            break;
        }
        case TPLAYER_MEDIA_ERROR_UNSUPPORTED: {
            printf("[%d] erro type:TPLAYER_MEDIA_ERROR_UNSUPPORTED\n", pPlayer->mTunnelId);
            break;
        }
        case TPLAYER_MEDIA_ERROR_IO: {
            printf("[%d] erro type:TPLAYER_MEDIA_ERROR_IO\n", pPlayer->mTunnelId);
            break;
        }
        }
        printf("[%d] error: open media source fail.\n", pPlayer->mTunnelId);
        break;
    }
    case TPLAYER_NOTIFY_NOT_SEEKABLE: {
        pPlayer->mSeekable = 0;
        printf("[%d] info: media source is unseekable.\n", pPlayer->mTunnelId);
        break;
    }
    case TPLAYER_NOTIFY_BUFFER_START: {
        printf("[%d] have no enough data to play\n", pPlayer->mTunnelId);
        break;
    }
    case TPLAYER_NOTIFY_BUFFER_END: {
        printf("[%d] have enough data to play again\n", pPlayer->mTunnelId);
        break;
    }
    case TPLAYER_NOTIFY_VIDEO_FRAME: {
        /* printf("[%d] get the decoded video frame\n", pPlayer->mTunnelId); */
        break;
    }
    case TPLAYER_NOTIFY_AUDIO_FRAME: {
        /* printf("[%d] get the decoded audio frame\n", pPlayer->mTunnelId); */
        break;
    }
    case TPLAYER_NOTIFY_SUBTITLE_FRAME: {
        /* printf("[%d] get the decoded subtitle frame\n", pPlayer->mTunnelId); */
        break;
    }
    default: {
        printf("[%d] warning: unknown callback from Tinaplayer.\n", pPlayer->mTunnelId);
        break;
    }
    }
    return 0;
}

int tplayer_init(TplayerVideoRotateType rotate) {
    for (int i = 0; i < TUNNEL_NUM; i++) {
        /* * create a player. */
        gplayer[i].mTPlayer = TPlayerCreate(CEDARX_PLAYER);
        if (gplayer[i].mTPlayer == NULL) {
            printf("[%d] can not create tplayer, quit.\n", i);
            return -1;
        }
        /* * set callback to player. */
        TPlayerSetNotifyCallback(gplayer[i].mTPlayer, CallbackForTPlayer,
                (void*) &gplayer[i]);
        /* set player start status */
        gplayer[i].mError = 0;
        gplayer[i].mSeekable = 1;
        gplayer[i].mPreparedFlag = 0;
        gplayer[i].mLoopFlag = 0;
        gplayer[i].mSetLoop = 0;
        gplayer[i].mMediaInfo = NULL;
        gplayer[i].mCompleteFlag = 0;
        gplayer[i].mTunnelId = i;
        sem_init(&gplayer[i].mPreparedSem, 0, 0);
        TPlayerReset(gplayer[i].mTPlayer);
        TPlayerSetDebugFlag(gplayer[i].mTPlayer, 0);
        TPlayerSetRotate(gplayer[i].mTPlayer, rotate);
        printf("tplayer_init [%d] done\n", i);
    }
    TPlayerSetVideoDisplay(gplayer[TUNNEL_CLICK].mTPlayer, false);
    TPlayerSetVideoDisplay(gplayer[TUNNEL_AUDIO].mTPlayer, false);
    printf("tplayer_init done\n");
    return 0;
}

int tplayer_loop(int id, int bloop) {
    gplayer[id].mSetLoop = bloop;
    return 0;
}

int tplayer_exit(void) {
    for (int i = TUNNEL_NUM - 1; i >= 0; i--) {
        if (!gplayer[i].mTPlayer) {
            printf("player not init.\n");
            continue;
        }
        printf("[%d] join in\n", i);
        TPlayerReset(gplayer[i].mTPlayer);
        printf("[%d] player reset\n", i);
        TPlayerDestroy(gplayer[i].mTPlayer);
        printf("[%d] player destroy\n", i);
        gplayer[i].mTPlayer = NULL;
        sem_destroy(&gplayer[i].mPreparedSem);
    }
    return 0;
}

int tplayer_play_url(int id, const char *parth) {
    ISNULL(gplayer[id].mTPlayer);
    TPlayerReset(gplayer[id].mTPlayer);
    if (TPlayerSetDataSource(gplayer[id].mTPlayer, parth, NULL) != 0) {
        printf("[%d] TPlayerSetDataSource() return fail.\n", id);
        return -1;
    } else {
        printf("[%d] setDataSource end\n", id);
    }
    gplayer[id].mPreparedFlag = 0;
    if (TPlayerPrepareAsync(gplayer[id].mTPlayer) != 0) {
        printf("[%d] TPlayerPrepareAsync() return fail.\n", id);
        return -1;
    } else {
        printf("[%d] prepare\n", id);
    }
    sem_wait(&gplayer[id].mPreparedSem);
    printf("[%d] prepared ok\n", id);
    return 0;
}

int tplayer_play(int id) {
    ISNULL(gplayer[id].mTPlayer);
    if (!gplayer[id].mPreparedFlag) {
        printf("[%d] not prepared!\n", id);
        return -1;
    }
    if (TPlayerIsPlaying(gplayer[id].mTPlayer)) {
        printf("[%d] already playing!\n", id);
        return -1;
    }
    gplayer[id].mCompleteFlag = 0;
    return TPlayerStart(gplayer[id].mTPlayer);
}

int tplayer_setvolume(int id, int volume) {
    int ret;
    ISNULL(gplayer[id].mTPlayer);
    ret = TPlayerSetVolume(gplayer[id].mTPlayer, volume);
    if(ret == 0) gplayer[id].mVolume = volume;
    return ret;
}

int tplayer_getvolume(int id) {
    ISNULL(gplayer[id].mTPlayer);
    gplayer[id].mVolume = TPlayerGetVolume(gplayer[id].mTPlayer);
    return gplayer[id].mVolume;
}

int tplayer_pause(int id) {
    ISNULL(gplayer[id].mTPlayer);
    if (!TPlayerIsPlaying(gplayer[id].mTPlayer)) {
        printf("[%d] not playing!\n", id);
        return -1;
    }
    return TPlayerPause(gplayer[id].mTPlayer);
}

int tplayer_seekto(int id, int nSeekTimeMs) {
    ISNULL(gplayer[id].mTPlayer);
    if (!gplayer[id].mPreparedFlag) {
        printf("not prepared!\n");
        return -1;
    }

    /*
     if(TPlayerIsPlaying(gplayer[id].mTPlayer)){
     printf("[%d] seekto can not at palying state!\n", id);
     return -1;
     }
     */
    return TPlayerSeekTo(gplayer[id].mTPlayer, nSeekTimeMs);
}

int tplayer_stop(int id) {
    ISNULL(gplayer[id].mTPlayer);
    if (!gplayer[id].mPreparedFlag) {
        printf("[%d] not prepared!\n", id);
        return -1;
    }
    return TPlayerStop(gplayer[id].mTPlayer);
}

int tplayer_setlooping(int id, bool bLoop) {
    ISNULL(gplayer[id].mTPlayer);
    return TPlayerSetLooping(gplayer[id].mTPlayer, bLoop);
}

int tplayer_setscaledown(int id, TplayerVideoScaleDownType nHorizonScaleDown,
        TplayerVideoScaleDownType nVerticalScaleDown) {
    ISNULL(gplayer[id].mTPlayer);
    return TPlayerSetScaleDownRatio(gplayer[id].mTPlayer, nHorizonScaleDown,
            nVerticalScaleDown);
}

int tplayer_setdisplayrect(int id, int x, int y, unsigned int width,
        unsigned int height) {
    ISNULL(gplayer[id].mTPlayer);
    TPlayerSetDisplayRect(gplayer[id].mTPlayer, x, y, width, height);
    return 0;
}

int tplayer_setrotate(int id, TplayerVideoRotateType rotate) {
    ISNULL(gplayer[id].mTPlayer);
    return TPlayerSetRotate(gplayer[id].mTPlayer, rotate);
}

MediaInfo* tplayer_getmediainfo(int id) {
    return TPlayerGetMediaInfo(gplayer[id].mTPlayer);
}

int tplayer_getduration(int id, int *msec) {
    ISNULL(gplayer[id].mTPlayer);
    return TPlayerGetDuration(gplayer[id].mTPlayer, msec);
}

int tplayer_getcurrentpos(int id, int *msec) {
    ISNULL(gplayer[id].mTPlayer);
    return TPlayerGetCurrentPosition(gplayer[id].mTPlayer, msec);
}

int tplayer_getcompletestate(int id) {
    return gplayer[id].mCompleteFlag;
}
