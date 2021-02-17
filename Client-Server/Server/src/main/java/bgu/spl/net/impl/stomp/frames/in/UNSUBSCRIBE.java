package bgu.spl.net.impl.stomp.frames.in;

import bgu.spl.net.impl.stomp.StompMessagingProtocolImp;
import bgu.spl.net.impl.stomp.User;
import bgu.spl.net.impl.stomp.frames.out.RECEIPT;

import java.util.Queue;

public class UNSUBSCRIBE extends FrameIn {
    @Override
    public void play(StompMessagingProtocolImp protocol) {

        int user_topic_id = Integer.parseInt(this.get("id"));
        int user_id = protocol.getConnectionId();
        User activeUser = protocol.getDb().getUserByConnectionId(user_id);
        String receipt_id = this.get("receipt");

        // user isn't login
        if (!protocol.getDb().getUserByConnectionId(user_id).isLoggedIn()) {
            sendErrorWithReceipt(protocol, receipt_id, user_id);
            return;
        }

        // remove the topic from the user->topics
        String topic_name = activeUser.unsubscribeFromTopic(user_topic_id);

        // remove the user id from the q in db->topicsMap
        if (topic_name != null) {
            protocol.getDb().getTopicsMap().get(topic_name).remove(user_id);
        }
        // send a receipt
        RECEIPT receipt = new RECEIPT();
        receipt.addHeader("receipt-id", receipt_id);
        protocol.getConnections().send(user_id, receipt);
    }
}
