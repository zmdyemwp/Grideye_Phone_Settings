package com.evenwell.grideye.Peripheral;

import android.content.pm.PackageManager;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;

import com.evenwell.grideye.R;
import com.evenwell.grideye.Peripheral.util.FusionConst;

public class PermissionsCheckActivity extends AppCompatActivity {
    private static final int RESULT_CODE_MAIN_PERMISSION = 100;
    private boolean allPermissionGranted;
    private int checkedPermissionCount;
    private String[] permissions;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_permissions_check);

        permissions =
                getIntent().getStringArrayExtra(FusionConst.PermissionConst.EXTRA_KEY_PERMISSIONS);
        allPermissionGranted = true;
        if (permissions != null) {
            ActivityCompat.requestPermissions(PermissionsCheckActivity.this,
                    permissions, RESULT_CODE_MAIN_PERMISSION);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           String permissions[], int[] grantResults) {
        switch (requestCode) {
            case RESULT_CODE_MAIN_PERMISSION: {
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    doNext();
                } else {
                    // permission denied, boo! Disable the
                    // functionality that depends on this permission.
                    allPermissionGranted = false;
                    setResult(RESULT_CANCELED);
                    finish();
                }
                return;
            }
        }

    }

    private void doNext(){
        checkedPermissionCount++;
        if (checkedPermissionCount == permissions.length){
            if(allPermissionGranted){
                setResult(RESULT_OK);
            }else{
                setResult(RESULT_CANCELED);
            }
            finish();
        }
    }

    @Override
    public void onBackPressed() {
        setResult(RESULT_CANCELED);
        finish();
        super.onBackPressed();
    }
}
