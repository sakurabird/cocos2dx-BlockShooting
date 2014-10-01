/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/

package com.sakurafish.cocos2dx.brockshooting;

import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.FrameLayout;

import jp.live_aid.aid.AdController;
import jp.maru.mrd.IconCell;
import jp.maru.mrd.IconLoader;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

public class BrockShooting extends Cocos2dxActivity {
    static final String TAG = "BrockShooting";
    // astrskアイコン型広告
    private static final String MEDIA_CODE1 = "ast019423ynx6vly2vp7"; // Android上
    private static final String MEDIA_CODE2 = "ast01942680qmshrnacr"; // Android下
    private IconLoader<Integer> _iconLoader = null;
    private IconLoader<Integer> _iconLoader2 = null;
    private View mAsterView1, mAsterView2;

    // AID
    private static final String AID_CODE = "com.sakurafish.c"; // 広告枠:(Android全画面)
    private AdController mAidAdController; // AID

    static {
        System.loadLibrary("cocos2dcpp");
    }

    // native
    public static native void nativeEnd();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Global.activity = this;

        // アスタ
        setUpAster();

        // AID
        if (mAidAdController == null) {
            mAidAdController = new AdController(AID_CODE, this);
            mAidAdController.setCreativeStyle(AdController.CreativeStyle.POPUP_IMAGE);// 画像ポップアップ型広告
        }
    }

    @Override
    public Cocos2dxGLSurfaceView onCreateView() {
        Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
        // BrockShooting should create stencil buffer
        glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

        return glSurfaceView;
    }

    @Override
    protected void onResume() {
        startLoadingAstr();
        mAidAdController.startPreloading();

        super.onResume();
    }

    @Override
    public void onPause() {
        stopLoadingAstr();
        mAidAdController.stopPreloading();

        super.onPause();
    }

    private void setUpAster() {
        FrameLayout.LayoutParams adParams = new FrameLayout.LayoutParams(
                LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
        adParams.gravity = (Gravity.TOP | Gravity.CENTER);
        FrameLayout.LayoutParams adParams2 = new FrameLayout.LayoutParams(
                LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
        adParams2.gravity = (Gravity.BOTTOM | Gravity.CENTER);

        mAsterView1 = this.getLayoutInflater().inflate(R.layout.astrsk1, null);
        addContentView(mAsterView1, adParams);
        setUpIconLoader();
        mAsterView2 = this.getLayoutInflater().inflate(R.layout.astrsk2, null);
        addContentView(mAsterView2, adParams2);
        setUpIconLoader2();
    }

    private void startLoadingAstr() {
        if (_iconLoader != null) {
            _iconLoader.startLoading(); // start loading icon-AD
        }
        if (_iconLoader2 != null) {
            _iconLoader2.startLoading(); // start loading icon-AD
        }
    }

    private void stopLoadingAstr() {
        if (_iconLoader != null) {
            _iconLoader.stopLoading(); // stop loading ad
        }
        if (_iconLoader2 != null) {
            _iconLoader2.stopLoading(); // stop loading ad
        }
    }

    protected void setUpIconLoader()
    {
        if (_iconLoader != null) {
            return;
        }
        try {
            _iconLoader = new IconLoader<Integer>(MEDIA_CODE1, this);

            ViewGroup vGrp = (ViewGroup) findViewById(R.id.iconCellLayout1);

            int i, c = vGrp.getChildCount();
            for (i = 0; i < c; i++)
            {
                View view = vGrp.getChildAt(i);
                if (view instanceof IconCell) {
                    ((IconCell) view).addToIconLoader(_iconLoader);
                    ((IconCell) view).setTitleColor(0xffffffff);
                }
            }
            _iconLoader.setRefreshInterval(15);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    protected void setUpIconLoader2()
    {
        if (_iconLoader2 != null) {
            return;
        }
        try {
            _iconLoader2 = new IconLoader<Integer>(MEDIA_CODE2, this);

            ViewGroup vGrp = (ViewGroup) findViewById(R.id.iconCellLayout2);

            int i, c = vGrp.getChildCount();
            for (i = 0; i < c; i++)
            {
                View view = vGrp.getChildAt(i);
                if (view instanceof IconCell) {
                    ((IconCell) view).addToIconLoader(_iconLoader2);
                    ((IconCell) view).setTitleColor(0xffffffff);
                }
            }
            _iconLoader2.setRefreshInterval(15);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void showAd(int n) {
        Utils.logDebug("showAd " + n);
        switch (n) {
            case 1:
                showAstIcons(true);
                break;
            case 2:
                showAstIcons(false);
                break;

            default:
                break;
        }
    }

    /**
     * アスタ広告表示
     */
    private void showAstIcons(final boolean show) {
        if (show) {
            startLoadingAstr();
        } else {
            stopLoadingAstr();
        }
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mAsterView1.setVisibility(show ? View.VISIBLE : View.GONE);
                mAsterView2.setVisibility(show ? View.VISIBLE : View.GONE);
            }
        });
    }

    /**
     * AID広告表示
     */
    public void showAID() {
        Log.v(TAG, "showAID");
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mAidAdController.showDialog(AdController.DialogType.ON_EXIT); // AID
            }
        });
    }
}
