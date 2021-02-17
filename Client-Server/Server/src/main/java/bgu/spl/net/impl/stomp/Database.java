package bgu.spl.net.impl.stomp;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicInteger;

public class Database {

    ConcurrentHashMap<String, User> usersMap = new ConcurrentHashMap<>();
    ConcurrentHashMap<String, ConcurrentLinkedQueue<Integer>> topicsMap = new ConcurrentHashMap<>();
    AtomicInteger msgId = new AtomicInteger(-1);

    public User getUserByName(String name) {
        return usersMap.get(name);
    }

    public User getUserByConnectionId(int id) {
        for (User user1 : usersMap.values()) {
            if (user1.getConnectionId() == id)
                return user1;
        }
        return null;
    }

    public void addUserToTopic(String topicName, Integer userId){
        topicsMap.get(topicName).add(userId);
    }

    public User addUserIfAbsent(String name,String pass, int userId){
        return usersMap.putIfAbsent(name,new User(name,pass, userId));
    }
    public Object addTopicIfAbsent(String topicName){
        return topicsMap.putIfAbsent(topicName,new ConcurrentLinkedQueue<>());
    }
    public ConcurrentHashMap<String, ConcurrentLinkedQueue<Integer>> getTopicsMap() {
        return topicsMap;
    }

    public ConcurrentHashMap<String, User> getUsersMap() {
        return usersMap;
    }

    public Integer nextMsgId(){
        return msgId.incrementAndGet();
    }
}
