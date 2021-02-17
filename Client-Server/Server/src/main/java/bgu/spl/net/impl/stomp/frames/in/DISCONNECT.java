package bgu.spl.net.impl.stomp.frames.in;

import bgu.spl.net.impl.stomp.StompMessagingProtocolImp;
import bgu.spl.net.impl.stomp.frames.out.RECEIPT;


public class DISCONNECT extends FrameIn {
    @Override
    public void play(StompMessagingProtocolImp protocol) {

        System.out.println("server go a disconnect request");

        String receipt_id = this.get("receipt");
        int user_id = protocol.getConnectionId();

        // user isn't login
        if (!protocol.getDb().getUserByConnectionId(user_id).isLoggedIn()) {
            sendErrorWithReceipt(protocol, receipt_id, user_id);
            return;
        }

        // send receipt
        RECEIPT receipt = new RECEIPT();
        receipt.addHeader("receipt-id", receipt_id);
        receipt.addHeader("\n", ":::press any button to exit.\nGOODBYE!:::");
        protocol.getConnections().send(user_id, receipt);

        disconnectActions(protocol);

        // turning user's logging = false
        protocol.getDb().getUserByConnectionId(user_id).logout(user_id);

    }

}
