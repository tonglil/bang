package com.zap;

import java.util.ArrayList;

import android.app.ActionBar;
import android.app.ActionBar.Tab;
import android.app.FragmentTransaction;
import android.content.Context;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;

public class PlayerActivity extends FragmentActivity {

    ViewPager viewPager;
    TabsAdapter tabsAdapter;

    String tabStats;

    public void setTabStats(String data) {
        tabStats = data;
    }

    public String getTabStats() {
        return tabStats;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        viewPager = new ViewPager(this);
        viewPager.setId(R.id.pager);
        setContentView(viewPager);

        final ActionBar bar = getActionBar();
        bar.setNavigationMode(ActionBar.NAVIGATION_MODE_TABS);
        bar.setDisplayOptions(0, ActionBar.DISPLAY_SHOW_TITLE);

        tabsAdapter = new TabsAdapter(this, viewPager);
        tabsAdapter.addTab(bar.newTab().setText("Stats"), PlayerStats.class, null);
        tabsAdapter.addTab(bar.newTab().setText("Cards"), PlayerCards.class, null);
        tabsAdapter.addTab(bar.newTab().setText("Done"), PlayerDone.class, null);

        if (savedInstanceState != null) {
            bar.setSelectedNavigationItem(savedInstanceState.getInt("tab", 0));
        }
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        //super.onSaveInstanceState(outState);
        outState.putInt("tab", getActionBar().getSelectedNavigationIndex());
    }

    public static class TabsAdapter extends FragmentPagerAdapter implements ActionBar.TabListener, ViewPager.OnPageChangeListener {

        private final Context context;
        private final ActionBar actionBar;
        private final ViewPager viewPager;
        private final ArrayList<TabInfo> tabs = new ArrayList<TabInfo>();

        static final class TabInfo {

            private final Class<?> classes;
            private final Bundle args;

            TabInfo(Class<?> _class, Bundle _args) {
                classes = _class;
                args = _args;
            }

        }

        public TabsAdapter(FragmentActivity activity, ViewPager pager) {
            super(activity.getSupportFragmentManager());
            context = activity;
            actionBar = activity.getActionBar();
            viewPager = pager;
            viewPager.setAdapter(this);
            viewPager.setOnPageChangeListener(this);
        }

        public void addTab(ActionBar.Tab tab, Class<?> classes, Bundle args) {
            TabInfo info = new TabInfo(classes, args);
            tab.setTag(info);
            tab.setTabListener(this);
            tabs.add(info);
            actionBar.addTab(tab);
            notifyDataSetChanged();
        }

        @Override
        public void onPageScrollStateChanged(int state) {

        }

        @Override
        public void onPageScrolled(int position, float positionOffset, int positionoffsetPixels) {

        }

        @Override
        public void onPageSelected(int position) {
            actionBar.setSelectedNavigationItem(position);
        }

        @Override
        public void onTabReselected(Tab tab, FragmentTransaction transaction) {

        }

        @Override
        public void onTabSelected(Tab tab, FragmentTransaction transaction) {
            Object tag = tab.getTag();
            for (int i = 0; i < tabs.size(); i++) {
                if (tabs.get(i) == tag) {
                    viewPager.setCurrentItem(i);
                }
            }
        }

        @Override
        public void onTabUnselected(Tab tab, FragmentTransaction transaction) {

        }

        @Override
        public Fragment getItem(int position) {
            TabInfo info = tabs.get(position);
            return Fragment.instantiate(context, info.classes.getName(), info.args);
        }

        @Override
        public int getCount() {
            return tabs.size();
        }

    }

}
