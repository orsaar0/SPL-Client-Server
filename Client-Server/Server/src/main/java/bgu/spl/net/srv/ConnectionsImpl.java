package bgu.spl.net.srv;

import bgu.spl.net.impl.stomp.frames.Frame;

import java.util.concurrent.ConcurrentHashMap;

public class ConnectionsImpl<T> implements Connections<T> {
    private ConcurrentHashMap<Integer, ConnectionHandler<T>> connectionHandlerHashMap = new ConcurrentHashMap<>();

    @Override
    public boolean send(int connectionId, T msg) {
        if (msg instanceof Frame) {
            System.out.println("connected and send");
            Frame to_send = (Frame) msg;
            connectionHandlerHashMap.get(connectionId).send((T) to_send);
            return true;
        }
        return false;
    }

    @Override
    public void send(String channel, T msg) {

    }

    @Override
    public void disconnect(int connectionId) {
        connectionHandlerHashMap.remove(connectionId);
    }

    @Override
    public int connect(ConnectionHandler<T> handler, int curID) {
        connectionHandlerHashMap.put(curID, handler);
        return curID;
    }
}
