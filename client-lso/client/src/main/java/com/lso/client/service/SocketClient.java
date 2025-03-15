package com.lso.client.service;

import java.io.*;
import java.net.*;
import java.util.concurrent.*;

import org.springframework.stereotype.Service;

@Service
public class SocketClient {
    private static final String SERVER_HOST = "server"; // Modifica con l'indirizzo del tuo server
    private static final int SERVER_PORT = 3000; // Modifica con la porta del server
    private Socket socket;
    private PrintWriter out;
    private BufferedReader in;
    private boolean isRunning;
    private BlockingQueue<String> responseQueue = new LinkedBlockingQueue<>();

    public SocketClient() throws IOException {
        int retries = 0;
        int maxRetries = 10; // Numero massimo di tentativi
        int retryDelay = 2000; // Ritardo iniziale in millisecondi (2 secondi)

        while (retries < maxRetries) {
            try {
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

                return; // Se la connessione è riuscita, esci dal ciclo

            } catch (IOException e) {
                retries++;
                if (retries >= maxRetries) {
                    throw new IOException("Impossibile connettersi al server dopo " + maxRetries + " tentativi.");
                }
                System.out
                        .println("Tentativo di connessione fallito. Riprovo tra " + retryDelay / 1000 + " secondi...");
                try {
                    Thread.sleep(retryDelay); // Ritardo tra i tentativi
                    retryDelay *= 2; // Aumenta il ritardo per il prossimo tentativo
                } catch (InterruptedException ie) {
                    Thread.currentThread().interrupt();
                    throw new IOException("Interruzione durante il ritardo tra i tentativi.");
                }
            }
        }
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
