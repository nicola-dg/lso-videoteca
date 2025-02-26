package com.lso.client.controller;

import com.lso.client.DTO.UserDTO;
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

    // @GetMapping("/login")
    // public String showLoginForm(Model model) {
    //     model.addAttribute("loginForm", new LoginForm());
    //     return "login"; 
    // }

    // @PostMapping("/login")
    // public String login(@ModelAttribute LoginForm form, Model model, HttpSession session) { 
    //         Response res = requestService.sendRequest(requestService.createRequest().setMethod(Method.GET)
    //                 .setPath("/user").setPayload("{\\\"username\\\":\\\"" + form.getUsername() + "\\\"}"));
    //         if (res.getStatusCode().equals("200")) {
    //             System.out.println("success");
    //             return "redirect:/movies";
    //         } else {
    //             socket.close();
    //             model.addAttribute("error", "Credenziali errate");
    //             return "login";
    //         }
    // }

    @GetMapping("/register")
    public String showRegisterForm(Model model) {
        model.addAttribute("registrationForm", new RegistrationForm());
        return "registration";
    }

    @PostMapping("/register")
    public String register(@Valid @ModelAttribute RegistrationForm form, BindingResult result, Model model) {
        if (result.hasErrors()){
            return "registration";
        }
        UserDTO userDTO = new UserDTO();
        userDTO.setEmail(form.getEmail());
        userDTO.setName(form.getNome());
        userDTO.setSurname(form.getCognome());
        userDTO.setPassword(form.getPassword());
        userDTO.setUsername(form.getUsername());

        Response res = requestService.sendRequest(requestService.createRequest().setMethod(Method.POST).setPath("/user").setPayload(userDTO.toJSON()));
        if (res.getStatusCode().equals("200")) {
            System.out.println("register success");
            return "redirect:/login?registered";
        } else {
            model.addAttribute("error", res);
            return "registration";
        }
    }

    // @GetMapping("/logout")
    // public String logout(HttpSession session) {
    //     SocketClient userSocket = (SocketClient) session.getAttribute("userSocket");
    //     if (userSocket != null)
    //         userSocket.close();
    //     session.invalidate();
    //     return "redirect:/login";
    // }
}