package com.zap;

import android.app.ActionBar;
import android.app.FragmentTransaction;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.support.v4.view.ViewPager;

public class PlayerActivity extends FragmentActivity {
    ViewPager viewPager;
    PlayerTabPagerAdapter tabAdapter;
    ActionBar actionBar;

    private void setupTabs() {
        tabAdapter = new PlayerTabPagerAdapter(getSupportFragmentManager());
        viewPager = (ViewPager) findViewById(R.id.pager);

        viewPager.setOnPageChangeListener(new ViewPager.SimpleOnPageChangeListener() {
            @Override
            public void onPageSelected(int position) {
                actionBar = getActionBar();
                actionBar.setSelectedNavigationItem(position);
            }
        });

        viewPager.setAdapter(tabAdapter);
    }

    // Enable Tabs on Action Bar
    private void setupActionBar() {
        actionBar = getActionBar();

        actionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_TABS);
        ActionBar.TabListener tabListener = new ActionBar.TabListener() {
            @Override
            public void onTabReselected(android.app.ActionBar.Tab tab, FragmentTransaction ft) {
                // TODO Auto-generated method stub
            }

            @Override
            public void onTabSelected(ActionBar.Tab tab, FragmentTransaction ft) {
                viewPager.setCurrentItem(tab.getPosition());
            }

            @Override
            public void onTabUnselected(android.app.ActionBar.Tab tab, FragmentTransaction ft) {
                // TODO Auto-generated method stub
            }
        };

        // Add New Tabs
        actionBar.addTab(actionBar.newTab().setText("Stats").setTabListener(tabListener));
        actionBar.addTab(actionBar.newTab().setText("Cards").setTabListener(tabListener));
        actionBar.addTab(actionBar.newTab().setText("Done").setTabListener(tabListener));
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_player);
        setupTabs();
        setupActionBar();
    }
}