package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.impl.stomp.frames.Frame;

import java.io.IOException;
import java.lang.reflect.Constructor;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.Collections;
import java.util.LinkedList;

public class StompEncoderDecoder implements MessageEncoderDecoder<Frame> {

//    public static void main(String[] args) {
//        String str1 = "CONNECT\naccept-version:1.2\nhost:stomp.cs.bgu.ac.il\nlogin:bob\npasscode:alice\n\n\0";
//        byte[] all = str1.getBytes();
//        StompEncoderDecoder e = new StompEncoderDecoder();
//        Frame frame = null;
//
//        for (byte b : all) {
//            if (frame != null) {
//                System.out.println("Err: 1241");
//            }
//            frame = e.decodeNextByte(b);
//        }
//        if (frame == null) {
//            System.out.println("Err: 526846");
//        }
//
//        String str2 = new String(e.encode(frame));
//
//        int g = 0;
//    }

    //region decode
    private byte[] bytes = new byte[1 << 10]; //start with 1k
    private int len = 0;

    public Frame decodeNextByte(byte nextByte) {
        if (nextByte == '\0') {
            return popString();
        }

        pushByte(nextByte);
        return null;
    }

    private void pushByte(byte nextByte) {
        if (len >= bytes.length) {
            bytes = Arrays.copyOf(bytes, len * 2);
        }

        bytes[len++] = nextByte;
    }

    private Frame frame;

    @SuppressWarnings("unchecked")
    private Frame popString() {
        //notice that we explicitly requesting that the string will be decoded from UTF-8
        //this is not actually required as it is the default encoding in java.
        frame = null;
        try {
            String msgstr = new String(bytes, 0, len, StandardCharsets.UTF_8);

            msgstr = msgstr.replace("\n\n", "\nframeBody:");
            String[] strings = msgstr.split("\n");
            LinkedList<String> lines = new LinkedList<>();
            Collections.addAll(lines, strings);

            String cmd = lines.removeFirst();
            Class<Frame> type = (Class<Frame>) Class.forName("bgu.spl.net.impl.stomp.frames.in." + cmd);
            Constructor<Frame> constructor = type.getConstructor();
            frame = constructor.newInstance();

            lines.forEach((line) -> {
                String[] pair = line.split(":", 2);
                System.out.println(line);
                String headerName = pair[0];
                String headerValue = pair[1];

                frame.addHeader(headerName, headerValue);
            });
            len = 0;
        } catch (Exception e) {
            frame = null;
        }
        return frame;
    }
    //endregion

    @Override
    public byte[] encode(Frame message) {
        return (message.toString() + "\0").getBytes();
    }
}
