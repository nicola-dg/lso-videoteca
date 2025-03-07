package com.lso.client.DTO;

import com.fasterxml.jackson.annotation.JsonProperty;

public class LoanDTO {

    @JsonProperty("id")
    private String id;

    @JsonProperty("film_id")
    private String filmId;

    @JsonProperty("user_id")
    private String userId;

    @JsonProperty("checkout_date")
    private String checkoutDate;

    @JsonProperty("due_date")
    private String dueDate;

    @JsonProperty("return_date")
    private String returnDate;

    @JsonProperty("film_title")
    private String filmTitle;

    // Costruttore vuoto
    public LoanDTO() {
    }

    public String getFilmTitle() {
        return this.filmTitle;
    }

    public void setFilmTitle(String filmTitle) {
        this.filmTitle = filmTitle;
    }

    // Getter e Setter
    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getFilmId() {
        return filmId;
    }

    public void setFilmId(String filmId) {
        this.filmId = filmId;
    }

    public String getUserId() {
        return userId;
    }

    public void setUserId(String userId) {
        this.userId = userId;
    }

    public String getCheckoutDate() {
        return checkoutDate;
    }

    public void setCheckoutDate(String checkoutDate) {
        this.checkoutDate = checkoutDate;
    }

    public String getDueDate() {
        return dueDate;
    }

    public void setDueDate(String dueDate) {
        this.dueDate = dueDate;
    }

    public String getReturnDate() {
        return returnDate;
    }

    public void setReturnDate(String returnDate) {
        this.returnDate = returnDate;
    }

    // Metodo toString per debug e logging
    @Override
    public String toString() {
        return "LoanDTO{" +
                "id='" + id + '\'' +
                ", filmId='" + filmId + '\'' +
                ", userId='" + userId + '\'' +
                ", checkoutDate='" + checkoutDate + '\'' +
                ", dueDate='" + dueDate + '\'' +
                ", returnDate='" + returnDate + '\'' +
                ", filmTitle='" + filmTitle + '\'' +
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
        if (filmId != null) {
            if (!first)
                json.append(",");
            json.append("\\\"film_id\\\":").append(escapeString(filmId));
            first = false;
        }
        if (userId != null) {
            if (!first)
                json.append(",");
            json.append("\\\"user_id\\\":").append(escapeString(userId));
            first = false;
        }
        if (checkoutDate != null) {
            if (!first)
                json.append(",");
            json.append("\\\"checkout_date\\\":").append(escapeString(checkoutDate));
            first = false;
        }
        if (dueDate != null) {
            if (!first)
                json.append(",");
            json.append("\\\"due_date\\\":").append(escapeString(dueDate));
            first = false;
        }
        if (returnDate != null) {
            if (!first)
                json.append(",");
            json.append("\\\"return_date\\\":").append(escapeString(returnDate));
        }
        if (filmTitle != null) {
            if (!first)
                json.append(",");
            json.append("\\\"film_title\\\":").append(escapeString(filmTitle));
        }

        json.append("}");
        return json.toString();
    }

    private String escapeString(String s) {
        return "\\\"" + s + "\\\"";
    }

}
