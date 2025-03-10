package com.lso.client.DTO;

import com.fasterxml.jackson.annotation.JsonProperty;

public class MessageDTO {

    @JsonProperty("id")
    private String id;

    @JsonProperty("user_id")
    private String userId;

    @JsonProperty("text")
    private String text;

    @JsonProperty("checkout_date")
    private String checkoutDate;

    // Costruttore vuoto
    public MessageDTO() {
    }

    // Getter e Setter
    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getUserId() {
        return userId;
    }

    public void setUserId(String userId) {
        this.userId = userId;
    }

    public String getText() {
        return this.text;
    }

    public void setText(String text) {
        this.text = text;
    }

    public String getCheckoutDate() {
        return checkoutDate;
    }

    public void setCheckoutDate(String checkoutDate) {
        this.checkoutDate = checkoutDate;
    }

    // Metodo toString per debug e logging
    @Override
    public String toString() {
        return "LoanDTO{" +
                "id='" + id + '\'' +
                ", userId='" + userId + '\'' +
                ", text='" + text + '\'' +
                ", checkoutDate='" + checkoutDate + '\'' +
                '}';
    }

    // Metodo per convertire in JSON manualmente
    public String toJSON() {
        StringBuilder json = new StringBuilder("{");
        boolean first = true;

        if (id != null) {
            json.append("\\\"id\\\":").append(escapeString(id));
            first = false;
        }
        if (userId != null) {
            if (!first)
                json.append(",");
            json.append("\\\"user_id\\\":").append(escapeString(userId));
            first = false;
        }
        if (text != null) {
            if (!first)
                json.append(",");
            json.append("\\\"text\\\":").append(escapeString(text));
        }
        if (checkoutDate != null) {
            if (!first)
                json.append(",");
            json.append("\\\"checkout_date\\\":").append(escapeString(checkoutDate));
            first = false;
        }

        json.append("}");
        return json.toString();
    }

    private String escapeString(String s) {
        return "\\\"" + s + "\\\"";
    }

}
