package com.lso.client.service;

import java.util.List;

import org.springframework.stereotype.Service;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.lso.client.service.ResponseService.Response;
import com.lso.client.types.Method;

import jakarta.websocket.server.ServerEndpoint;

@Service
public class RequestService {

    SocketClient socketClient;
    ResponseService responseService;

    public class Request {
        Method method;
        String path;
        String payload;

        public Method getMethod() {
            return this.method;
        }

        public Request setMethod(Method method) {
            this.method = method;
            return this;
        }

        public String getPath() {
            return this.path;
        }

        public Request setPath(String path) {
            this.path = path;
            return this;
        }

        public String getPayload() {
            return this.payload;
        }

        public Request setPayload(String payload) {
            this.payload = payload;
            return this;
        }

        @Override
        public String toString() {
            return "{\"method\":\"" + method.toString() + "\",\"path\":\"" + path + "\",\"payload\":\"" + payload
                    + "\"}";
        }
    }

    public RequestService(SocketClient socketClient, ResponseService responseService){
        this.socketClient=socketClient;
        this.responseService=responseService;
    }

    public Request createRequest() {
        return new Request();
    }

    public Response sendRequest(Request req){
        try {
            return responseService.parseResponse(socketClient.sendRequest(req.toString()));
        } catch (Exception e) {
            System.out.println("Eccezione durante l'invio della richiesta");
            return null;
        }
    }

}
