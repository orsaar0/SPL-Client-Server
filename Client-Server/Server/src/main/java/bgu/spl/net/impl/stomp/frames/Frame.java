package bgu.spl.net.impl.stomp.frames;

import java.util.LinkedHashMap;

public abstract class Frame {

    LinkedHashMap<String, String> headers = new LinkedHashMap<>();

    public LinkedHashMap<String, String> getHeaders() {
        return headers;
    }

    public void addHeader(String headerName, String headerValue) {
        headers.put(headerName, headerValue);
    }

    public String get(String headerName) {
        return headers.get(headerName);
    }

    private String tmpString = "";
    @Override
    public String toString() {
        tmpString = getClass().getSimpleName() + "\n";
        headers.forEach((k, v) -> {

            tmpString += k + ":" + v;
            if (!k.equals("frameBody")) {
                tmpString += "\n";
            }

        });
        tmpString = tmpString.replace("frameBody:", "\n");
        return tmpString;
    }

    public void addEndingSymbol (){
        this.addHeader("frameBody", "\0");
    }
}
