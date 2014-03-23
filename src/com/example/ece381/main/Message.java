package com.example.ece381.main;

import java.util.ArrayList;

public class Message {
    public static boolean ready;
    public static int type;
    public static int fromId;
    public static int toId;
    public static int count;
    public static ArrayList<ArrayList<Integer>> r_pinfo;
    public static ArrayList<ArrayList<Integer>> r_cinfo;

    private static Message instance;

    private Message(boolean ready, int type, int fromId, int toId, int count, ArrayList<ArrayList<Integer>> r_pinfo, ArrayList<ArrayList<Integer>> r_cinfo) {
        Message.ready = ready;
        Message.type = type;
        Message.fromId = fromId;
        Message.toId = toId;
        Message.count = count;
        Message.r_pinfo = r_pinfo;
        Message.r_cinfo = r_cinfo;
    }

    public static synchronized Message getInstance() {
        if (instance == null) {
            instance = new Message(false, 0, 0, 0, 0, new ArrayList<ArrayList<Integer>>(), new ArrayList<ArrayList<Integer>>());
        }
        return instance;
    }

    public void setMessage(boolean ready, int type, int fromId, int toId, int count, ArrayList<ArrayList<Integer>> r_pinfo, ArrayList<ArrayList<Integer>> r_cinfo) {
        Message.ready = ready;
        Message.type = type;
        Message.fromId = fromId;
        Message.toId = toId;
        Message.count = count;
        Message.r_pinfo = r_pinfo;
        Message.r_cinfo = r_cinfo;
    }
}
