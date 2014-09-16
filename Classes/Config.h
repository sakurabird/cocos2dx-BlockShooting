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

// fonts
#define FONT_DISC1 "fonts/fontDisc1.fnt"
#define FONT_WHITE "fonts/fontWhite.fnt"
#define FONT_ORANGE "fonts/fontOrange.fnt"
#define FONT_RED "fonts/fontRed.fnt"
#define FONT_VIOLET "fonts/fontViolet.fnt"
#define FONT_BLUE "fonts/fontBlue.fnt"
#define FONT_GREEN "fonts/fontGreen.fnt"
#define FONT_YELLOW "fonts/fontYellow.fnt"
#define FONT_TITLE "fonts/title.fnt"
#define FONT_TOUCH "fonts/fontTouch.fnt"
#define FONT_BIG1 "fonts/fontBig1.fnt"
#define FONT_BIG2 "fonts/fontBig2.fnt"


// images
#define PNG_BG "image/bg.png"
#define PNG_POPUP_BG "image/popup_bg.png"
#define PNG_POPUP_BG2 "image/popup_bg2.png"
#define PNG_OK "image/ok.png"
#define PNG_CANCEL "image/cancel.png"
#define PNG_BALL "image/ball.png"
#define PNG_BLOCK "image/block.png"
#define PNG_BLOCK_BLUE "image/block_blue.png"
#define PNG_BLOCK_GREEN "image/block_green.png"
#define PNG_BLOCK_RED "image/block_red.png"
#define PNG_BLOCK_VIOLET "image/block_violet.png"
#define PNG_BLOCK_YELLOW "image/block_yellow.png"
#define PNG_BAR "image/bar.png"
#define PNG_RECT1 "image/rect1.png"
#define PNG_ON "image/button_on.png"
#define PNG_OFF "image/button_off.png"
#define PNG_HOME "image/home.png"
#define PNG_REFRESH "image/refresh.png"
#define PNG_BACK "image/back.png"
#define PNG_P_BLUE "image/p_blue.png"
#define PNG_P_GREEN "image/p_green.png"
#define PNG_P_RED "image/p_red.png"
#define PNG_P_VIOLET "image/p_violet.png"
#define PNG_P_YELLOW "image/p_yellow.png"

#define PNG_MINI1_1 "image/mini_anim1_1.png"
#define PNG_MINI1_2 "image/mini_anim1_2.png"
#define PNG_MINI1_3 "image/mini_anim1_3.png"
#define PNG_MINI1_4 "image/mini_anim1_4.png"
#define PNG_MINI2_1 "image/mini_anim2_1.png"
#define PNG_MINI2_2 "image/mini_anim2_2.png"
#define PNG_MINI2_3 "image/mini_anim2_3.png"
#define PNG_MINI2_4 "image/mini_anim2_4.png"
#define PNG_MINI3_1 "image/mini_anim3_1.png"
#define PNG_MINI3_2 "image/mini_anim3_2.png"
#define PNG_MINI3_3 "image/mini_anim3_3.png"
#define PNG_MINI3_4 "image/mini_anim3_4.png"

#define PNG_STAGE1_A "image/stage1_active.png"
#define PNG_STAGE1_D "image/stage1_inactive.png"
#define PNG_STAGE2_A "image/stage2_active.png"
#define PNG_STAGE2_D "image/stage2_inactive.png"
#define PNG_STAGE3_A "image/stage3_active.png"
#define PNG_STAGE3_D "image/stage3_inactive.png"
#define PNG_STAGE4_A "image/stage4_active.png"
#define PNG_STAGE4_D "image/stage4_inactive.png"
#define PNG_STAGE5_A "image/stage5_active.png"
#define PNG_STAGE5_D "image/stage5_inactive.png"
#define PNG_STAGE6_A "image/stage6_active.png"
#define PNG_STAGE6_D "image/stage6_inactive.png"
#define PNG_STAGE7_A "image/stage7_active.png"
#define PNG_STAGE7_D "image/stage7_inactive.png"
#define PNG_STAGE8_A "image/stage8_active.png"
#define PNG_STAGE8_D "image/stage8_inactive.png"
#define PNG_STAGE9_A "image/stage9_active.png"
#define PNG_STAGE9_D "image/stage9_inactive.png"
#define PNG_STAGE10_A "image/stage10_active.png"
#define PNG_STAGE10_D "image/stage10_inactive.png"
#define PNG_STAGE11_A "image/stage11_active.png"
#define PNG_STAGE11_D "image/stage11_inactive.png"
#define PNG_STAGE12_A "image/stage12_active.png"
#define PNG_STAGE12_D "image/stage12_inactive.png"
#define PNG_STAGE13_A "image/stage13_active.png"
#define PNG_STAGE13_D "image/stage13_inactive.png"
#define PNG_STAGE14_A "image/stage14_active.png"
#define PNG_STAGE14_D "image/stage14_inactive.png"
#define PNG_STAGE15_A "image/stage15_active.png"
#define PNG_STAGE15_D "image/stage15_inactive.png"
#define PNG_STAGE16_A "image/stage16_active.png"
#define PNG_STAGE16_D "image/stage16_inactive.png"

// sounds
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

// constants
#define BLOCK_COLUMN 12
#define BLOCK_ROW 16

#define BALL_REMAIN 3
#define BALL_FRICTION 1.005

#define ITEM1_RATE 0.05
#define ITEM2_RATE 0.06
#define ITEM3_RATE 0.04
#define ITEM4_RATE 0.04
#define ITEM5_RATE 0.03

#define ITEM_LIFE_SECONDS 5
#define ITEM1_VELOCITY 10

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
    kTagRedLabel,
    kTagBlueLabel,
    kTagYellowLabel,
    kTagGreenLabel,
    kTagGrayLabel,
    kTagGameOver,
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

// レベルクリア状態
extern int g_LevelState[2][16];

extern cocos2d::CCSize g_visibleSize;


#endif
