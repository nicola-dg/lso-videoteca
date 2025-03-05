package com.lso.client.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;

@Controller
public class LandingController {

    @GetMapping("/")
    public String landingPage() {
        return "landing";
    };

    @GetMapping("/admin")
    public String admin() {
        return "admin-dashboard";
    };

    @GetMapping("/cart")
    public String cart() {
        return "cart-checkout";
    };

    @GetMapping("/user")
    public String dashboard() {
        return "user-profile-test";
    };

}
