//
//  Config.h
//  nyangame
//
//  Created by sakura on 2014/04/20.
//
//

#ifndef nyangame_Config_h
#define nyangame_Config_h

#define ccsf(...) CCString::createWithFormat(__VA_ARGS__)->getCString()


#define FONT_WHITE "fonts/fontWhite.fnt"
#define FONT_ORANGE "fonts/fontOrange.fnt"
#define FONT_RED "fonts/fontRed.fnt"
#define FONT_VIOLET "fonts/fontViolet.fnt"
#define FONT_BLUE "fonts/fontBlue.fnt"
#define FONT_GREEN "fonts/fontGreen.fnt"
#define FONT_YELLOW "fonts/fontYellow.fnt"
#define FONT_TITLE "fonts/fonttitle.fnt"
#define FONT_TOUCH "fonts/fontTouch.fnt"
#define FONT_BIG1 "fonts/fontBig1.fnt"
#define FONT_BIG2 "fonts/fontBig2.fnt"


#define PNG_BALL "image/ball.png"
#define PNG_BLOCK "image/block.png"
#define PNG_BLOCK_BLUE "image/block_blue.png"
#define PNG_BLOCK_GREEN "image/block_green.png"
#define PNG_BLOCK_RED "image/block_red.png"
#define PNG_BLOCK_VIOLET "image/block_violet.png"
#define PNG_BLOCK_YELLOW "image/block_yellow.png"
#define PNG_BAR "image/bar.png"
#define PNG_RECT1 "image/rect1.png"
#define PNG_MUSIC_ON "image/button_Music_On.png"
#define PNG_MUSIC_OFF "image/button_Music_Off.png"
#define PNG_SE_ON "image/button_SE_On.png"
#define PNG_SE_OFF "image/button_SE_Off.png"
#define PNG_HOME "image/home.png"
#define PNG_REFRESH "image/refresh.png"
#define PNG_BACK "image/back.png"
#define PNG_P_BLUE "image/p_blue.png"
#define PNG_P_GREEN "image/p_green.png"
#define PNG_P_RED "image/p_red.png"
#define PNG_P_VIOLET "image/p_violet.png"
#define PNG_P_YELLOW "image/p_yellow.png"

#define MP3_BG "sounds/bg.mp3"
#define MP3_BALLPUSH "sounds/ballPush.mp3"
#define MP3_BOUNDBAR "sounds/boundBar.mp3"
#define MP3_BOUNDWALL "sounds/boundWall.mp3"
#define MP3_HITBLOCK "sounds/hitBlock.mp3"
#define MP3_BALLLOST "sounds/ballLost.mp3"
#define MP3_CLEAR "sounds/clear.mp3"
#define MP3_GAMEOVER "sounds/gameover.mp3"
#define MP3_GETITEM1 "sounds/getItem1.mp3"
#define MP3_GETITEM2 "sounds/getItem2.mp3"
#define MP3_GETITEM3 "sounds/getItem3.mp3"
#define MP3_GETITEM4 "sounds/getItem4.mp3"
#define MP3_GETITEM5 "sounds/getItem5.mp3"

#define PNG_BG_MAX 48

#define LEVEL_EASY 1
#define LEVEL_NORMAL 2
#define LEVEL_HARD 3

#define BLOCK_COLUMN 12
#define BLOCK_ROW 16

#define BALL_REMAIN 3
#define BALL_FRICTION 1.005

#define ITEM1_RATE 0.08
#define ITEM2_RATE 0.04
#define ITEM3_RATE 0.04
#define ITEM4_RATE 0.04
#define ITEM5_RATE 0.1

// TODO
enum kTag
{
    kTagBar = 1001,
    kTagBall = 1002,
    kTagBlock = 1003,
    kTagStartLabel = 1004,
    kTagLayer = 1234,
    kTagBackground = 1005,
    kTagBallRemainLabel = 1006,
    kTagBallRemainCount = 1007,
    kTagScoreLabel = 1008,
    kTagScore = 1009,
    kTagItem1,
    kTagItem2,
    kTagItem3,
    kTagItem4,
    kTagItem5,
    kTagItemGetLabel,
    kTagLevel,
    kTagBack,
    kTagRetry,
    kTagRedLabel,
    kTagBlueLabel,
    kTagYellowLabel,
    kTagGreenLabel,
    kTagGrayLabel,
    kTagGameOver,
    kTagHighScoreLabel,
    kTagMusicButtton,
    kTagSEButtton,
    kTagGameOverScoreLavel,
    kTagGameOverHighScoreLavel,
    kTagBaseBlock = 10000,
};

// TODO
enum kZOrder
{
    kZOrderBackground,
    kZOrderEmmit,
    kZOrderLabel,
    kZOrderBlock,
    kZOrderGameOver,
    kZOrderTop = 1,
};

#endif
