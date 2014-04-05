package com.zap.main;

import java.util.ArrayList;

import android.util.Log;

public class DE2Message {
    volatile private static Boolean readyToContinue = false;
    volatile private static Boolean readyToSend = true;
    volatile private static Boolean doingToSelf = true;

    public static Boolean getReadyToContinue(Boolean once) {
        if (once) {
            Log.i("colin", "readyToContinue: " + readyToContinue);
        }
        return readyToContinue;
    }

    public static void setReadyToContinue(Boolean readyToContinue) {
        Log.i("colin", "readyToContinue: " + DE2Message.readyToContinue + "->" + readyToContinue);
        DE2Message.readyToContinue = readyToContinue;
    }

    public static Boolean getReadyToSend(Boolean once) {
        if (once) {
            Log.i("colin", "readyToSend: " + readyToSend);
        }
        return readyToSend;
    }

    public static void setReadyToSend(Boolean readyToSend) {
        Log.i("colin", "readyToSend: " + DE2Message.readyToSend + "->" + readyToSend);
        DE2Message.readyToSend = readyToSend;
    }

    public static Boolean getDoingToSelf(Boolean once) {
        if (once) {
            Log.i("colin", "doingToSelf: " + doingToSelf);
        }
        return doingToSelf;
    }

    public static void setDoingToSelf(Boolean doingToSelf) {
        Log.i("colin", "doingToSelf: " + DE2Message.doingToSelf + "->" + doingToSelf);
        DE2Message.doingToSelf = doingToSelf;
    }

    public static boolean isDoingToSelf() {
        return doingToSelf;
    }

    public static void setDoingToSelf(boolean doingToSelf) {
        DE2Message.doingToSelf = doingToSelf;
        if (doingToSelf) {
            DE2Message.readyToSend = true;
        }
    }

    private static int type;
    private static int fromId;
    private static int toId;
    private static int count;
    private static ArrayList<ArrayList<Integer>> r_pinfo;
    private static ArrayList<ArrayList<Integer>> r_cinfo;

    private static ArrayList<Integer> card_choices;

    public static ArrayList<Integer> getCard_choices() {
        return card_choices;
    }

    public static void setCard_choices(ArrayList<Integer> card_choices) {
        DE2Message.card_choices = card_choices;
    }

    private static DE2Message instance;

    private DE2Message(boolean ready, int type, int fromId, int toId, int count, ArrayList<ArrayList<Integer>> r_pinfo, ArrayList<ArrayList<Integer>> r_cinfo) {
        // DE2Message.setReadyToContinue(ready);
        DE2Message.type = type;
        DE2Message.fromId = fromId;
        DE2Message.toId = toId;
        DE2Message.count = count;
        DE2Message.r_pinfo = r_pinfo;
        DE2Message.r_cinfo = r_cinfo;
    }

    public static synchronized DE2Message getInstance() {
        if (instance == null) {
            instance = new DE2Message(false, 0, 0, 0, 0, new ArrayList<ArrayList<Integer>>(), new ArrayList<ArrayList<Integer>>());
        }
        return instance;
    }

    public static void setMessage(boolean ready, int type, int fromId, int toId, int count, ArrayList<ArrayList<Integer>> r_pinfo, ArrayList<ArrayList<Integer>> r_cinfo) {
        // DE2Message.setReadyToContinue(ready);
        DE2Message.type = type;
        DE2Message.fromId = fromId;
        DE2Message.toId = toId;
        DE2Message.count = count;
        DE2Message.r_pinfo = r_pinfo;
        DE2Message.r_cinfo = r_cinfo;
    }

    public static int getType() {
        return type;
    }

    public static void setType(int type) {
        DE2Message.type = type;
    }

    public static int getFromId() {
        return fromId;
    }

    public static void setFromId(int fromId) {
        DE2Message.fromId = fromId;
    }

    public static int getToId() {
        return toId;
    }

    public static void setToId(int toId) {
        DE2Message.toId = toId;
    }

    public static int getCount() {
        return count;
    }

    public static void setCount(int count) {
        DE2Message.count = count;
    }

    public static ArrayList<ArrayList<Integer>> getR_pinfo() {
        return r_pinfo;
    }

    public static void setR_pinfo(ArrayList<ArrayList<Integer>> r_pinfo) {
        DE2Message.r_pinfo = r_pinfo;
    }

    public static ArrayList<ArrayList<Integer>> getR_cinfo() {
        return r_cinfo;
    }

    public static void setR_cinfo(ArrayList<ArrayList<Integer>> r_cinfo) {
        DE2Message.r_cinfo = r_cinfo;
    }

    public static void setInstance(DE2Message instance) {
        DE2Message.instance = instance;
    }
}
