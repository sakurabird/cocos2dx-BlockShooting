//
//  BlockSprite.h
//  BrockShooting
//
//  Created by sakura on 2014/04/22.
//
//

#ifndef __BrockShooting__BlockSprite__
#define __BrockShooting__BlockSprite__

#include "cocos2d.h"
#include "Config.h"

#define PNG_BLOCK "block.png"

#define BLOCK_COLUMN 15
#define BLOCK_ROW 25

class BlockSprite : public cocos2d::CCSprite
{
protected:
    //ブロックの番号をあらわす変数＆アクセサ
    CC_SYNTHESIZE(int, m_number, Number);

public:

    BlockSprite();
    virtual ~BlockSprite();

    static BlockSprite* createWithBlockSize(float width, float height, int number);

    virtual bool initWithBlockSize(float width, float height, int number);
};


#endif /* defined(__BrockShooting__BlockSprite__) */
