package com.lso.client.service;

import java.io.*;
import java.net.Socket;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

import org.springframework.stereotype.Service;

@Service
public class SocketClient {
    private Socket socket;
    private PrintWriter out;
    private BufferedReader in;
    private final BlockingQueue<String> responseQueue = new LinkedBlockingQueue<>();
    private volatile boolean isRunning;

    private static final String SERVER_HOST = "localhost";
    private static final int SERVER_PORT = 3000;

    public SocketClient() throws IOException {
        this.socket = new Socket(SERVER_HOST, SERVER_PORT);
        this.out = new PrintWriter(socket.getOutputStream(), true);
        this.in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        this.isRunning = true;

        // Thread per leggere le risposte del server
        new Thread(() -> {
            try {
                while (isRunning) {
                    String response = in.readLine();
                    if (response != null) {
                        responseQueue.put(response);
                    } else {
                        close();
                    }
                }
            } catch (IOException | InterruptedException e) {
                close();
            }
        }).start();
    }

    public String sendRequest(String request) throws IOException {
        out.println(request);
        try {
            return responseQueue.take(); // Blocca finché non riceve una risposta
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            throw new IOException("Richiesta interrotta");
        }
    }

    public void close() {
        isRunning = false;
        try {
            if (in != null)
                in.close();
            if (out != null)
                out.close();
            if (socket != null)
                socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}