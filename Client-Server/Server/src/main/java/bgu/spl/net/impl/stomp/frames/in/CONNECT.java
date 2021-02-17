package bgu.spl.net.impl.stomp.frames.in;

import bgu.spl.net.impl.stomp.StompMessagingProtocolImp;
import bgu.spl.net.impl.stomp.User;
import bgu.spl.net.impl.stomp.frames.Frame;
import bgu.spl.net.impl.stomp.frames.out.CONNECTED;

public class CONNECT extends FrameIn {

    @Override
    public void play(StompMessagingProtocolImp protocol) {
        // if one or more headers are missing
        if (!this.containHeader("login") || !this.containHeader("passcode") || !this.containHeader("host") || !this.containHeader("accept-version")) {
            sendError(protocol, protocol.getConnectionId(), " one or more headers are missing");
            return;
        }

        String user_name = this.get("login");
        String passcode = this.get("passcode");
        int connectionId = protocol.getConnectionId();

        User nullIfNew = protocol.getDb().addUserIfAbsent(user_name, passcode, connectionId);
        User toActive = protocol.getDb().getUserByName(user_name);

        if (nullIfNew != null) { // some Error
            if (toActive.isLoggedIn()) {
                sendError(protocol, connectionId, "user is already logged in");
                disconnectActions(protocol);
            } else if (toActive.getPasscode().equals(passcode)) {
                sendError(protocol, connectionId, "wrong passcode");
            }

        } else {
            toActive.login(connectionId);
            Frame msg = new CONNECTED();
            msg.addHeader("version", this.get("accept-version"));
            toActive.setLoggedIn(true);
            protocol.getConnections().send(connectionId, msg);
        }

    }
}
