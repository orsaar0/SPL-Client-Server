package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.rci.ObjectEncoderDecoder;
import bgu.spl.net.impl.rci.RemoteCommandInvocationProtocol;
import bgu.spl.net.impl.stomp.frames.Frame;
import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        Database database = new Database();

// you can use any server...
//        Server.threadPerClient(
//                7777, //port
//                () -> new StompMessagingProtocolImp(database), //protocol factory
//                StompEncoderDecoder::new //message encoder decoder factory
//        ).serve();

        Server.reactor(
                Runtime.getRuntime().availableProcessors(),
                7777, //port
                () -> new StompMessagingProtocolImp(database), //protocol factory
                StompEncoderDecoder::new //message encoder decoder factory
        ).serve();

    }


}
