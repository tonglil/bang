package com.zap;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentStatePagerAdapter;

public class PlayerTabPagerAdapter extends FragmentStatePagerAdapter {
    public PlayerTabPagerAdapter(FragmentManager fm) {
        super(fm);
        // TODO Auto-generated constructor stub
    }

    @Override
    public Fragment getItem(int i) {
        switch (i) {
            case 0:
                return new PlayerStats();
            case 1:
                return new PlayerCards();
            case 2:
                return new PlayerDone();
        }
        return null;
    }

    @Override
    public int getCount() {
        return 3; // No of Tabs
    }
}