package com.lso.client.controller;

import com.lso.client.model.LoginForm;
import com.lso.client.model.RegistrationForm;
import com.lso.client.service.RequestService;
import com.lso.client.service.ResponseService.Response;
import com.lso.client.service.SocketClient;
import com.lso.client.types.Method;

import jakarta.servlet.http.HttpSession;
import jakarta.validation.Valid;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.*;

import java.io.IOException;

@Controller
public class AuthController {
    private RequestService requestService;
    private SocketClient socket;

    public AuthController(RequestService requestService, SocketClient socket) {
        this.requestService = requestService;
        this.socket=socket;
    }

    @GetMapping("/login")
    public String showLoginForm(Model model) {
        model.addAttribute("loginForm", new LoginForm());
        return "login";
    }

    @PostMapping("/login")
    public String login(@ModelAttribute LoginForm form, Model model, HttpSession session) { 
            Response res = requestService.sendRequest(requestService.createRequest().setMethod(Method.GET)
                    .setPath("/user").setPayload("{\\\"random\\\":\\\"payload\\\"}"));
            if (res.getStatusCode().equals("200")) {
                // String userId = response.split(";")[1];
                // session.setAttribute("userId", userId);
                // session.setAttribute("userSocket", userSocket); // Salva la connessione nella
                // sessione
                System.out.println("success");
                return "redirect:/movies";
            } else {
                socket.close();
                model.addAttribute("error", "Credenziali errate");
                return "login";
            }
    }

    @GetMapping("/register")
    public String showRegisterForm(Model model) {
        model.addAttribute("registrationForm", new RegistrationForm());
        return "registration";
    }

    // @PostMapping("/register")
    // public String register(@Valid @ModelAttribute RegistrationForm form, BindingResult result, Model model) {
    //     if (result.hasErrors())
    //         return "registration";

    //     try {
    //         SocketClient tempSocket = new SocketClient();
    //         String response = tempSocket.sendRequest(
    //                 "REGISTER;" + form.getUsername() + ";" + form.getPassword() + ";" + form.getEmail());
    //         if ("SUCCESS".equals(response)) {
    //             return "redirect:/login?registered";
    //         } else {
    //             model.addAttribute("error", response);
    //             return "registration";
    //         }
    //     } catch (IOException e) {
    //         model.addAttribute("error", "Errore di connessione");
    //         return "registration";
    //     }
    // }

    @GetMapping("/logout")
    public String logout(HttpSession session) {
        SocketClient userSocket = (SocketClient) session.getAttribute("userSocket");
        if (userSocket != null)
            userSocket.close();
        session.invalidate();
        return "redirect:/login";
    }
}