package io.fiverocks.android;

import android.app.Activity;
import android.os.Bundle;

/**
 * Sample activity integrated with the 5Rocks SDK.
 * <p>
 * The values of the string resources of "fiverocks_app_id" and
 * "fivrocks_app_key" are used for the 5Rocks SDK credentials.
 */
public class FiveRocksActivity extends Activity {

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    FiveRocks.init(this, //
        getText(R.string.fiverocks_app_id).toString(), //
        getText(R.string.fiverocks_app_key).toString());
  }

  @Override
  protected void onStart() {
    super.onStart();
    FiveRocks.onActivityStart(this);
  }

  @Override
  protected void onStop() {
    FiveRocks.onActivityStop(this);
    super.onStop();
  }
}
