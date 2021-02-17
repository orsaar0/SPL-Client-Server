package bgu.spl.net.impl.stomp.frames.in;

import bgu.spl.net.impl.stomp.StompMessagingProtocolImp;
import bgu.spl.net.impl.stomp.frames.out.MESSAGE;

import java.util.concurrent.ConcurrentLinkedQueue;

public class SEND extends FrameIn {
    @Override
    public void play(StompMessagingProtocolImp protocol) {
        int user_id = protocol.getConnectionId();

        // if one or more headers are missing
        if (!this.containHeader("destination")){
            sendError(protocol, user_id, " one header or more are missing");
            return;
        }

        // user isn't login
        if (!protocol.getDb().getUserByConnectionId(user_id).isLoggedIn()) {
            sendError(protocol, user_id, "you are not login. please login first");
        }

        MESSAGE msg = new MESSAGE();

        String genre = this.get("destination");
        // put the topic if absent
        protocol.getDb().getTopicsMap().putIfAbsent(genre, new ConcurrentLinkedQueue<>());
        // if the user subscribed to the topic -> add his id to the MESSAGE
        if (protocol.getDb().getTopicsMap().get(genre).contains(user_id)) {
            String user_topic_id = protocol.getDb().getUserByConnectionId(user_id).getUserTopicId(this.get("destination")).toString();
            msg.addHeader("subscription", user_topic_id);
        }
        // the user isnt subcribed -> subscription is -1
        else{
            msg.addHeader("subscription", "-1");
        }

        msg.addHeader("Message-id", protocol.getDb().nextMsgId().toString());
        msg.addHeader("destination", genre);
        msg.addHeader("frameBody", this.get("frameBody"));

        // send a MESSAGE to all relevant users subscribed to that topic
        for (Integer user : protocol.getDb().getTopicsMap().get(genre)) {
            protocol.getConnections().send(user, msg);
        }
    }
}

