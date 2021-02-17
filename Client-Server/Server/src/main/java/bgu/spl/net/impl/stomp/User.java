package bgu.spl.net.impl.stomp;

//import jdk.vm.ci.amd64.AMD64;

import java.util.HashMap;
import java.util.concurrent.atomic.AtomicInteger;

public class User {
    private String username;
    private String passcode;
    private boolean loggedIn = false;
    private HashMap<String, Integer> topics = new HashMap<>();
    private int connectionId;

    public User(String username, String passcode, int connectionId) {
        this.username = username;
        this.passcode = passcode;
        this.connectionId = connectionId;
    }
    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getPasscode() {
        return passcode;
    }

    public void setPasscode(String passcode) {
        this.passcode = passcode;
    }

    public void setLoggedIn(boolean loggedIn) {
        this.loggedIn = loggedIn;
    }

    public boolean isLoggedIn() {
        return loggedIn;
    }

    public HashMap<String, Integer> getTopics() {
        return topics;
    }

    public void setTopics(HashMap<String, Integer> topics) {
        this.topics = topics;
    }

    public Integer getUserTopicId(String topicName){return topics.get(topicName);}// what do i do if i get wrong topic??

    public void addTopicToUser(String topicName, Integer userTopicId) {
        topics.put(topicName,userTopicId);
    }

    public String unsubscribeFromTopic(int topicId){
        for (String topic: topics.keySet()){
            if (topics.get(topic).equals(topicId)) {
                topics.remove(topic);
                return topic;
            }
        }
        return null;
    }

    public int getConnectionId() {
        return connectionId;
    }

    public boolean login(int connectionId) {
        loggedIn = true;
        return this.connectionId == connectionId;
    }

    public boolean logout(int connectionId) {
        loggedIn = false;
//        connectionId = -1;
        return this.connectionId == connectionId;
    }

}
