package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.impl.stomp.frames.Frame;
import bgu.spl.net.impl.stomp.frames.in.FrameIn;
import bgu.spl.net.srv.Connections;

public class StompMessagingProtocolImp implements StompMessagingProtocol<Frame> {
    private int connectionId;
    private Connections<Frame> connections;
    private boolean shouldTerminate;
    private Database db;


    /**
     * Used to initiate the current client protocol with it's personal connection ID and the connections implementation
     **/
    public void start(int connectionId, Connections<Frame> connections) {
        this.connectionId = connectionId;
        this.connections = connections;
        shouldTerminate = false;
    }

    public Connections<Frame> getConnections() {
        return connections;
    }

    public void setShouldTerminate(boolean shouldTerminate) {
        this.shouldTerminate = shouldTerminate;
    }

    public Database getDb() {
        return db;
    }

    public StompMessagingProtocolImp(Database db) {
        this.db = db;
    }

    @Override
    public void process(Frame message) {
        if (!(message instanceof FrameIn)) {
            return;
        }
        FrameIn fin = (FrameIn) message;
        fin.play(this);
    }

    /**
     * @return true if the connection should be terminated
     */
    public boolean shouldTerminate() {
        return shouldTerminate;
    }

    public int getConnectionId(){return connectionId;}

}
