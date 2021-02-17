package bgu.spl.net.impl.stomp.frames.in;

import bgu.spl.net.impl.stomp.StompMessagingProtocolImp;
import bgu.spl.net.impl.stomp.User;
import bgu.spl.net.impl.stomp.frames.Frame;
import bgu.spl.net.impl.stomp.frames.out.ERROR;

import java.util.Map;
import java.util.concurrent.ConcurrentLinkedQueue;

public abstract class FrameIn extends Frame {

    public abstract void play(StompMessagingProtocolImp protocol);

    public void sendErrorWithReceipt(StompMessagingProtocolImp protocol, String receipt_id, int user_id){
        ERROR error = new ERROR();
        error.addHeader("receipt-id", receipt_id);
        error.addHeader("message: ", "you are not login.\npress any button to exit");
        protocol.getConnections().send(user_id, error);
        protocol.setShouldTerminate(true);
    }

    public void sendError(StompMessagingProtocolImp protocol,int user_id, String details){
        ERROR error = new ERROR();
        error.addHeader("message", details);
        protocol.getConnections().send(user_id, error);
        protocol.setShouldTerminate(true);
    }

    public boolean containHeader(String header){
        return this.toString().contains(header);
    }

    public void disconnectActions(StompMessagingProtocolImp protocol){

        int user_id = protocol.getConnectionId();
        // user logged in -> we logout
        // remove the user from db->topics
        Map<String, ConcurrentLinkedQueue<Integer>> map = protocol.getDb().getTopicsMap();
        for (String gen : map.keySet()) {
            map.get(gen).remove(user_id);
        }

        // disconnect from connections
        protocol.getConnections().disconnect(user_id);

    }
}
