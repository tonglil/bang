package com.zap;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentStatePagerAdapter;

public class TabPagerAdapter extends FragmentStatePagerAdapter {
    public TabPagerAdapter(FragmentManager fm) {
        super(fm);
        // TODO Auto-generated constructor stub
    }

    @Override
    public Fragment getItem(int i) {
        switch (i) {
            case 0:
                // Fragement for Android Tab
                return new PlayerStats();
            case 1:
                // Fragment for Ios Tab
                return new PlayerCards();
            case 2:
                // Fragment for Windows Tab
                return new PlayerDone();
        }
        return null;
    }

    @Override
    public int getCount() {
        return 3; // No of Tabs
    }
}