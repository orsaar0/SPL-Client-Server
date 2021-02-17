package bgu.spl.net.impl.stomp.frames.in;

import bgu.spl.net.impl.stomp.StompMessagingProtocolImp;
import bgu.spl.net.impl.stomp.User;
import bgu.spl.net.impl.stomp.frames.Frame;
import bgu.spl.net.impl.stomp.frames.in.FrameIn;
import bgu.spl.net.impl.stomp.frames.out.RECEIPT;

public class SUBSCRIBE extends FrameIn {
    @Override
    public void play(StompMessagingProtocolImp protocol) {
        String topic = this.get("destination");
        int user_topic_id = Integer.parseInt(this.get("id"));
        int user_id = protocol.getConnectionId();
        String receipt_id = this.get("receipt");

        // user isn't login
        if (!protocol.getDb().getUserByConnectionId(user_id).isLoggedIn()) {
            sendErrorWithReceipt(protocol, receipt_id, user_id);
            return;
        }

        // add the client to all the relevant maps
        protocol.getDb().addTopicIfAbsent(topic);
        User activeUser = protocol.getDb().getUserByConnectionId(user_id);
        activeUser.addTopicToUser(topic, user_topic_id);
        protocol.getDb().addUserToTopic(topic, user_id);

        // send a receipt
        RECEIPT receipt = new RECEIPT();
        receipt.addHeader("receipt-id", receipt_id);

        protocol.getConnections().send(protocol.getConnectionId(), receipt);
    }
}
