package com.example.ece381.main;

import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;

import com.example.ece381.MyApplication;

public class Comm {
    private static MyApplication app;

    public static MyApplication getApp() {
        return app;
    }

    public static void setApp(MyApplication app) {
        Comm.app = app;
    }

    // Called when the user wants to send a message
    public static void sendMessage(String message) {
        // while (!DE2Message.isReadyToSend())
        // ;
        // DE2Message.setReadyToSend(false);
        // Get the message from the box

        String msg = message;
        byte[] bmsg = hstba(msg);

        // Create an array of bytes. First byte will be the
        // message length, and the next ones will be the message
        byte buf[] = new byte[bmsg.length + 1];

        buf[0] = (byte) bmsg.length;
        System.arraycopy(bmsg, 0, buf, 1, bmsg.length);

        // Now send through the output stream of the socket

        OutputStream out;
        try {
            out = app.sock.getOutputStream();
            try {
                out.write(buf, 0, bmsg.length + 1);
            } catch (IOException e) {
                e.printStackTrace();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // bytesToHexString
    final protected static char[] hexArray = "0123456789abcdef".toCharArray();

    public static String bth(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        for (int j = 0; j < bytes.length; j++) {
            int v = bytes[j] & 0xFF;
            hexChars[j * 2] = hexArray[v >>> 4];
            hexChars[j * 2 + 1] = hexArray[v & 0x0F];
        }
        return new String(hexChars);
    }

    // hexStringToByteArray

    public static byte[] hstba(String s) {
        int len = s.length();
        byte[] data = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4) + Character.digit(s.charAt(i + 1), 16));
        }
        return data;
    }

    // intToHexString

    public static String iths(int intg) {
        StringBuilder sb = new StringBuilder();
        sb.append(Integer.toHexString(intg));
        if (sb.length() < 2) {
            sb.insert(0, '0'); // pad with leading zero if needed
        }
        String hex = sb.toString();
        return hex;
    }

    public static void tellDE2CardsInHand(int pid, int ncards, ArrayList<Card> cards) {
        String msg = "11" + iths(pid) + iths(ncards);

        for (Card c : cards) {
            msg = msg + iths(c.cid);
        }

        sendMessage(msg);

        return;
    }

    public static void tellDE2BlueCardsInFront(int pid, int ncards, ArrayList<Card> cards) {
        String msg = "12" + iths(pid) + iths(ncards);

        for (Card c : cards) {
            msg = msg + iths(c.cid);
        }

        sendMessage(msg);

        return;
    }

    public static void tellDE2UserUsedSelf(int pid, String type) {
        int itype = 0;
        if (type.equals("BEER")) {
            itype = 0x01;
        } else if (type.equals("GATLING")) {
            itype = 0x02;
        } else if (type.equals("ALIENS")) {
            itype = 0x03;
        } else if (type.equals("GENERAL_STORE")) {
            itype = 0x04;
        } else if (type.equals("SALOON")) {
            itype = 0x05;
        }
        String msg = "13" + iths(pid) + iths(itype);

        sendMessage(msg);

        return;
    }

    public static void tellDE2UserUsedOther(int pid, int pid1, String type) {
        int itype = 0;
        if (type.equals("ZAP")) {
            itype = 0x01;
        } else if (type.equals("PANIC")) {
            itype = 0x02;
        } else if (type.equals("CAT_BALOU")) {
            itype = 0x03;
        } else if (type.equals("DUEL")) {
            itype = 0x04;
        } else if (type.equals("JAIL")) {
            itype = 0x05;
        }

        String msg = "14" + iths(pid) + iths(pid1) + iths(itype);

        sendMessage(msg);

        return;
    }

    public static void tellDE2UserEndedTurn(int pid) {
        String msg = "15" + iths(pid);

        sendMessage(msg);

        return;
    }

    public static void tellDE2UserNeedsXCards(int pid, int ncards) {
        String msg = "16" + iths(pid) + iths(ncards);

        sendMessage(msg);

        return;
    }

    public static void tellDE2UserUpdateLives(int pid, int lives) {
        String msg = "17" + iths(pid) + iths(lives);

        sendMessage(msg);

        return;
    }

    public static void tellDE2UserPickedCard(int pid, Card c) {
        String msg = "18" + iths(pid) + iths(c.cid);

        sendMessage(msg);

        return;
    }

    public static void tellDE2UserTransferCard(int pid, Card c) {
        String msg = "19" + iths(pid) + iths(c.cid);

        sendMessage(msg);

        return;
    }

    public static void tellDE2OK(int pid) {
        String msg = "1a" + iths(pid);

        sendMessage(msg);

        return;
    }

    public static void receiveInterpretDE2(byte buf[]) {
        // Message structure:
        // [0] Client_Id
        // [1] S_len
        // [2] Message_type
        int l = 0;
        int length = buf[l++];
        int fromId = buf[l++];
        int m_len = buf[l++];
        int type = buf[l++];
        int toId = fromId;
        int count = 0;
        ArrayList<ArrayList<Integer>> r_pinfo = new ArrayList<ArrayList<Integer>>();
        ArrayList<ArrayList<Integer>> r_cinfo = new ArrayList<ArrayList<Integer>>();
        switch (type) {
        case 0x01: {
            // tell_user_pid_role
            // [3] role
            ArrayList<Integer> pinfo = new ArrayList<Integer>();
            pinfo.add((int) buf[l++]);
            r_pinfo.add(pinfo);
            break;
        }
        case 0x02: {
            // tell_user_all_opponent_range_role
            // [3] pid
            // [4] distance
            // [5] role
            // ...
            // [21] pid
            // [22] distance
            // [23] role
            for (int i = 0; i < 7; i++) {
                ArrayList<Integer> pinfo = new ArrayList<Integer>();
                pinfo.add((int) buf[3 * i + l]);
                pinfo.add((int) buf[3 * i + l + 1]);
                pinfo.add((int) buf[3 * i + l + 2]);
                r_pinfo.add(pinfo);
            }
            break;
        }
        case 0x03: {
            // tell_user_all_opponent_blue_lives
            // [3] pid
            // [4] lives
            // [5] num_blues
            // ...
            // [21] pid
            // [22] lives
            // [23] num_blues
            // [24] rest of array is cid for blue cards
            // ...
            int i;
            for (i = 0; i < 7; i++) {
                ArrayList<Integer> pinfo = new ArrayList<Integer>();
                pinfo.add((int) buf[3 * i + l]);
                pinfo.add((int) buf[3 * i + l + 1]);
                pinfo.add((int) buf[3 * i + l + 2]);
                r_pinfo.add(pinfo);
            }
            int k = 3 * i + l;
            for (i = 0; i < 7; i++) {
                ArrayList<Integer> bcard = new ArrayList<Integer>();
                for (int j = 0; j < r_pinfo.get(i).get(2); j++) {
                    bcard.add((int) buf[k++]);
                }
                r_cinfo.add(bcard);
            }
            break;
        }
        case 0x04: {
            // tell_user_new_card
            // [3] cid
            // Player p = retrievePlayer(pid);
            // p.receiveCard(cid);
            break;
        }
        case 0x05: {
            // tell_user_lost_card
            // [3] cid
            // int cid = (int) buf[l++];
            // Player p = retrievePlayer(pid);
            // p.discardCard(cid);
            break;
        }
        case 0x06:
            // tell_user_their_turn
            // Player p = retrievePlayer(pid);
            // p.startTurn();
            break;
        case 0x07: {
            // tell_user_miss_or_lose_life
            // Player p = retrievePlayer(pid);
            // p.onZap();
            break;
        }
        case 0x08: {
            // tell_user_zap_or_lose_life
            // [3] 0x01 for Aliens, 0x02 for Duel
            // Player p = retrievePlayer(pid);
            // if (buf[l++] == 0x01) {
            // p.onAliens();
            // } else {
            // p.onDuel();
            // }
            break;
        }
        case 0x09: {
            // tell_user_get_life
            // Player p = retrievePlayer(pid);
            // p.onSaloon();
            break;
        }
        case 0x0a:
            // tell_user_ok
            DE2Message.setReadyToSend(true);
            break;
        case 0x0b: {
            // tell_user_blue_player_infront
            // [3] cid
            // int cid = (int) buf[l++];
            // Card c = CardController.getValidCard(cid);
            // Player p = retrievePlayer(pid);
            // p.receiveBlueCard(cid);
            // break;
            break;
        }
        case 0x0c: {
            // tell_user_store
            // [3] ncards
            // [4] array for choice of cards, length depends on ncards
            // ..
            // Player p = retrievePlayer(pid);
            // p.onGeneralStore();
        }
        case 0x0d: {
            // tell_user_panic
            // [3] toId
            // [4] nbcards
            // [5] array of blue cards
            // ...
            // Player p = retrievePlayer(pid);
            // p.onPanic();
        }
        case 0x0e: {
            // tell_user_cat_balou
            // [3] toId
            // [4] nbcards
            // [5] array of blue cards
            // ...
            // p.onCatBalou();
        }
        default:
            break;
        }
        if (type == 0x05 || type == 0x06 || type == 0x07 || type == 0x08 || type == 0x09 || type == 0x0b) {
            DE2Message.setMessage(false, type, fromId, toId, count, r_pinfo, r_cinfo);
        } else {
            DE2Message.setMessage(true, type, fromId, toId, count, r_pinfo, r_cinfo);
        }
        return;
    }
}
