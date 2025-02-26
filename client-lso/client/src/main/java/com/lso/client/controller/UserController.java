package com.lso.client.controller;

import com.lso.client.service.SocketClient;
import org.springframework.stereotype.Controller;
import jakarta.servlet.http.HttpSession;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

import java.io.IOException;

@Controller
@RequestMapping("/user")
public class UserController {

    @GetMapping("/{id}")
    public String getUserData(@PathVariable String id, Model model,HttpSession session) {
        SocketClient userSocket = (SocketClient) session.getAttribute("userSocket") ;
        if (userSocket == null) return "redirect:/login";
        try{
            String response = userSocket.sendRequest("GET_USER," + id);
            model.addAttribute("user", response);
            return "userProfile";
        }catch (IOException e){
            model.addAttribute("error", "Errore di connessione");
            return "main";
        }
    }

    @GetMapping("/{id}/messages")
    public String getUserMessages(@PathVariable String id, Model model, HttpSession session) {
        SocketClient userSocket = (SocketClient) session.getAttribute("userSocket") ;
        if (userSocket == null) return "redirect:/login";

        try{
            String response = userSocket.sendRequest("GET_MESSAGES," + id);
            model.addAttribute("messages", response);
            return "messagesPage";
        }catch (IOException e){
            model.addAttribute("error", "Errore di connessione");
            return "main";
        }

    }

    @PostMapping("/password")
    public String changePassword(@RequestParam String userId, @RequestParam String newPassword, HttpSession session) {
        SocketClient userSocket = (SocketClient) session.getAttribute("userSocket");
        if (userSocket == null) return "redirect:/login";

        try {
            userSocket.sendRequest("CHANGE_PASSWORD," + userId + "," + newPassword);
            return "redirect:/user-profile/"+userId;
        } catch (IOException e) {
            return "redirect:/user-profile";
        }
    }
}
