package com.urlapov.labspring3;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.SerializationFeature;
import com.fasterxml.jackson.datatype.jsr310.JavaTimeModule;
import controllers.CatController;
import dto.CatDto;
import dto.OwnerDto;
import entities.Cat;
import entities.CatColor;
import entities.Owner;
import enums.ServiceCatColor;
import exceptions.CatNotFoundException;
import exceptions.OwnerCatRelationshipException;
import org.aspectj.lang.annotation.Before;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.http.MediaType;
import org.springframework.security.test.context.support.WithMockUser;
import org.springframework.security.test.context.support.WithUserDetails;
import org.springframework.test.annotation.DirtiesContext;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.TestPropertySource;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.setup.MockMvcBuilders;
import org.springframework.web.context.WebApplicationContext;
import request.records.CatFriendCat;
import request.records.CatOwnerLink;
import request.records.Id;

import java.time.LocalDate;
import java.util.ArrayList;
import java.util.Arrays;


import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.springframework.security.test.web.servlet.setup.SecurityMockMvcConfigurers.springSecurity;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.*;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.content;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;


@SpringBootTest(
    webEnvironment = SpringBootTest.WebEnvironment.MOCK,
    classes = LabSpring3Application.class)
@DirtiesContext(classMode = DirtiesContext.ClassMode.BEFORE_EACH_TEST_METHOD)
@TestPropertySource(locations = "classpath:test-application.properties")
@AutoConfigureMockMvc
@WithMockUser(username = "Nafany", authorities = {"USER"})
class LabSpring3ApplicationTests {

    @Autowired
    private MockMvc mvc;

    @Test
    public void contextLoads() {
    }

    @Test
    public void createCatAsUser_ExpectForbidden() throws Exception {
        LocalDate moment = LocalDate.now();

        mvc.perform(post("/cats").content(asJsonString(new Cat("Rilik", moment, "kent", CatColor.Brown)))
            .contentType(MediaType.APPLICATION_JSON)
            .accept(MediaType.APPLICATION_JSON))
            .andExpect(status().isForbidden());
    }

    @Test
    @WithUserDetails(value = "test")
    public void ownCatAndGetCat_ExpectCat() throws Exception {
        mvc.perform(post("/cats/own").content(asJsonString(new CatOwnerLink(1L, 2L)))
                .contentType(MediaType.APPLICATION_JSON)
                .accept(MediaType.APPLICATION_JSON))
            .andExpect(status().isOk());

        mvc.perform(get("/cats/1")
                .accept(MediaType.APPLICATION_JSON))
            .andExpect(status().isOk())
            .andExpect(content().json(asJsonString(
                new CatDto(1L, "Zyw0o", LocalDate.of(2003, 8, 1), "Shiz", ServiceCatColor.Grey, 2L, new ArrayList<Long>()))));
    }

    @Test
    @WithUserDetails(value = "test")
    public void makeFriends_ExpectFriends() throws Exception {
        mvc.perform(post("/cats/own").content(asJsonString(new CatOwnerLink(1L, 2L)))
                .contentType(MediaType.APPLICATION_JSON)
                .accept(MediaType.APPLICATION_JSON))
            .andExpect(status().isOk());

        mvc.perform(post("/cats/make-friends").content(asJsonString(new CatFriendCat(1L, 3L)))
                .contentType(MediaType.APPLICATION_JSON)
                .accept(MediaType.APPLICATION_JSON));

        mvc.perform(get("/cats/1")
                .accept(MediaType.APPLICATION_JSON))
            .andExpect(status().isOk())
            .andExpect(content().json(asJsonString(
                new CatDto(1L, "Zyw0o", LocalDate.of(2003, 8, 1), "Shiz", ServiceCatColor.Grey, 2L, new ArrayList<Long>(Arrays.asList(3L))))));

        mvc.perform(get("/cats/3")
                .accept(MediaType.APPLICATION_JSON))
            .andExpect(status().isForbidden());
    }

    @Test
    @WithUserDetails(value = "admin")
    public void deleteCatThatDoesntExist_ExpectNoException() throws Exception {
        mvc.perform(delete("/cats").content(asJsonString(new Id(418L)))
                .contentType(MediaType.APPLICATION_JSON)
                .accept(MediaType.APPLICATION_JSON))
            .andExpect(status().isOk());
    }

    @Test
    @WithUserDetails(value = "test")
    public void makeCatsFriendsThatAreFriendsAlready_ExpectNoException() throws Exception {
        mvc.perform(post("/cats/own").content(asJsonString(new CatOwnerLink(1L, 2L)))
                .contentType(MediaType.APPLICATION_JSON)
                .accept(MediaType.APPLICATION_JSON))
            .andExpect(status().isOk());

        mvc.perform(post("/cats/make-friends").content(asJsonString(new CatFriendCat(1L, 3L)))
            .contentType(MediaType.APPLICATION_JSON)
            .accept(MediaType.APPLICATION_JSON))
            .andExpect(status().isOk());

        mvc.perform(post("/cats/make-friends").content(asJsonString(new CatFriendCat(1L, 3L)))
            .contentType(MediaType.APPLICATION_JSON)
            .accept(MediaType.APPLICATION_JSON))
            .andExpect(status().isOk());

        mvc.perform(post("/cats/make-friends").content(asJsonString(new CatFriendCat(1L, 3L)))
            .contentType(MediaType.APPLICATION_JSON)
            .accept(MediaType.APPLICATION_JSON))
            .andExpect(status().isOk());

        mvc.perform(get("/cats/1")
                .accept(MediaType.APPLICATION_JSON))
            .andExpect(status().isOk())
            .andExpect(content().json(asJsonString(
                new CatDto(1L, "Zyw0o", LocalDate.of(2003, 8, 1), "Shiz", ServiceCatColor.Grey, 2L, new ArrayList<Long>(Arrays.asList(3L))))));
    }

    private static String asJsonString(final Object obj) {
        ObjectMapper objectMapper = new ObjectMapper();
        objectMapper.registerModule(new JavaTimeModule());
        try {
            return objectMapper.writeValueAsString(obj);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }
}
