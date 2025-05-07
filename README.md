Below is the full code for a Spring Boot authentication application without using Lombok. This application includes user registration and login functionality with API endpoints that can be accessed via a browser or tools like Postman. All necessary getters, setters, and methods are implemented manually.

Project Structure
src/main/java/com/example/auth/
├── AuthApplication.java
├── model/
│   └── User.java
├── dto/
│   ├── UserRegistrationDto.java
│   └── UserLoginDto.java
├── repository/
│   └── UserRepository.java
├── service/
│   ├── UserService.java
│   └── UserServiceImpl.java
├── controller/
│   └── UserController.java
├── config/
│   ├── SecurityConfig.java
│   └── JpaConfig.java
├── exception/
│   └── UserAlreadyExistsException.java
└── security/
    └── CustomUserDetails.java

Code Files
1. `AuthApplication.java`
The main application class to bootstrap the Spring Boot application.
package com.example.auth;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class AuthApplication {

    public static void main(String[] args) {
        SpringApplication.run(AuthApplication.class, args);
    }
}

2. `User.java` (Entity)
The user entity representing the database table, with all methods implemented manually.
package com.example.auth.model;

import jakarta.persistence.*;
import java.util.Objects;

@Entity
@Table(name = "users")
public class User {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(unique = true, nullable = false)
    private String username;

    @Column(nullable = false)
    private String password;

    @Column(nullable = false)
    private String role;

    // Default constructor (required by JPA)
    public User() {}

    // Parameterized constructor
    public User(Long id, String username, String password, String role) {
        this.id = id;
        this.username = username;
        this.password = password;
        this.role = role;
    }

    // Getters
    public Long getId() {
        return id;
    }

    public String getUsername() {
        return username;
    }

    public String getPassword() {
        return password;
    }

    public String getRole() {
        return role;
    }

    // Setters
    public void setId(Long id) {
        this.id = id;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public void setRole(String role) {
        this.role = role;
    }

    // toString method
    @Override
    public String toString() {
        return "User{" +
                "id=" + id +
                ", username='" + username + '\'' +
                ", password='" + password + '\'' +
                ", role='" + role + '\'' +
                '}';
    }

    // equals method
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        User user = (User) o;
        return Objects.equals(id, user.id) &&
                Objects.equals(username, user.username) &&
                Objects.equals(password, user.password) &&
                Objects.equals(role, user.role);
    }

    // hashCode method
    @Override
    public int hashCode() {
        return Objects.hash(id, username, password, role);
    }
}

3. `UserRegistrationDto.java` (DTO)
Data Transfer Object for user registration.
package com.example.auth.dto;

public class UserRegistrationDto {
    private String username;
    private String password;
    private String role;

    // Getters
    public String getUsername() {
        return username;
    }

    public String getPassword() {
        return password;
    }

    public String getRole() {
        return role;
    }

    // Setters
    public void setUsername(String username) {
        this.username = username;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public void setRole(String role) {
        this.role = role;
    }
}

4. `UserLoginDto.java` (DTO)
Data Transfer Object for user login.
package com.example.auth.dto;

public class UserLoginDto {
    private String username;
    private String password;

    // Getters
    public String getUsername() {
        return username;
    }

    public String getPassword() {
        return password;
    }

    // Setters
    public void setUsername(String username) {
        this.username = username;
    }

    public void setPassword(String password) {
        this.password = password;
    }
}

5. `UserRepository.java`
JPA repository interface for database operations on the User entity.
package com.example.auth.repository;

import com.example.auth.model.User;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.Optional;

@Repository
public interface UserRepository extends JpaRepository {
    Optional findByUsername(String username);
}

6. `UserService.java` (Interface)
Service interface defining user-related operations.
package com.example.auth.service;

import com.example.auth.model.User;

import java.util.Optional;

public interface UserService {
    User registerUser(User user);
    Optional findByUsername(String username);
}

7. `UserServiceImpl.java` (Implementation)
Implementation of the UserService interface, handling user registration and lookup.
package com.example.auth.service;

import com.example.auth.exception.UserAlreadyExistsException;
import com.example.auth.model.User;
import com.example.auth.repository.UserRepository;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;

import java.util.Optional;

@Service
public class UserServiceImpl implements UserService {

    private final UserRepository userRepository;
    private final PasswordEncoder passwordEncoder;

    public UserServiceImpl(UserRepository userRepository, PasswordEncoder passwordEncoder) {
        this.userRepository = userRepository;
        this.passwordEncoder = passwordEncoder;
    }

    @Override
    public User registerUser(User user) {
        if (userRepository.findByUsername(user.getUsername()).isPresent()) {
            throw new UserAlreadyExistsException("Username " + user.getUsername() + " already exists");
        }
        user.setPassword(passwordEncoder.encode(user.getPassword()));
        return userRepository.save(user);
    }

    @Override
    public Optional findByUsername(String username) {
        return userRepository.findByUsername(username);
    }
}

8. `UserController.java`
REST controller handling registration and login endpoints.
package com.example.auth.controller;

import com.example.auth.dto.UserLoginDto;
import com.example.auth.dto.UserRegistrationDto;
import com.example.auth.exception.UserAlreadyExistsException;
import com.example.auth.model.User;
import com.example.auth.service.UserService;
import org.springframework.http.ResponseEntity;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.web.bind.annotation.*;

import java.util.Optional;

@RestController
@RequestMapping("/api")
public class UserController {

    private final UserService userService;
    private final PasswordEncoder passwordEncoder;

    public UserController(UserService userService, PasswordEncoder passwordEncoder) {
        this.userService = userService;
        this.passwordEncoder = passwordEncoder;
    }

    @PostMapping("/register")
    public ResponseEntity register(@RequestBody UserRegistrationDto dto) {
        try {
            User user = new User();
            user.setUsername(dto.getUsername());
            user.setPassword(dto.getPassword());
            user.setRole(dto.getRole());
            userService.registerUser(user);
            return ResponseEntity.ok("User registered successfully");
        } catch (UserAlreadyExistsException e) {
            return ResponseEntity.badRequest().body(e.getMessage());
        }
    }

    @PostMapping("/login")
    public ResponseEntity login(@RequestBody UserLoginDto dto) {
        Optional user = userService.findByUsername(dto.getUsername());
        if (user.isPresent() && passwordEncoder.matches(dto.getPassword(), user.get().getPassword())) {
            return ResponseEntity.ok("Login successful");
        }
        return ResponseEntity.status(401).body("Invalid credentials");
    }
}

9. `SecurityConfig.java`
Spring Security configuration to secure the application and define authentication rules.
package com.example.auth.config;

import com.example.auth.security.CustomUserDetails;
import com.example.auth.service.UserService;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.security.web.SecurityFilterChain;

@Configuration
@EnableWebSecurity
public class SecurityConfig {

    private final UserService userService;

    public SecurityConfig(UserService userService) {
        this.userService = userService;
    }

    @Bean
    public UserDetailsService userDetailsService() {
        return username -> new CustomUserDetails(userService.findByUsername(username)
                .orElseThrow(() -> new RuntimeException("User not found")));
    }

    @Bean
    public PasswordEncoder passwordEncoder() {
        return new BCryptPasswordEncoder();
    }

    @Bean
    public SecurityFilterChain securityFilterChain(HttpSecurity http) throws Exception {
        http.csrf(csrf -> csrf.disable())
            .authorizeHttpRequests(authorizeRequests ->
                authorizeRequests
                    .requestMatchers("/api/register", "/api/login").permitAll()
                    .anyRequest().authenticated()
            )
            .httpBasic(httpBasic -> {});
        return http.build();
    }
}

10. `JpaConfig.java`
Configuration for JPA repositories.
package com.example.auth.config;

import org.springframework.context.annotation.Configuration;
import org.springframework.data.jpa.repository.config.EnableJpaRepositories;

@Configuration
@EnableJpaRepositories(basePackages = "com.example.auth.repository")
public class JpaConfig {
}

11. `UserAlreadyExistsException.java`
Custom exception thrown when a username is already taken.
package com.example.auth.exception;

public class UserAlreadyExistsException extends RuntimeException {
    public UserAlreadyExistsException(String message) {
        super(message);
    }
}

12. `CustomUserDetails.java`
Custom implementation of Spring Security’s UserDetails interface.
package com.example.auth.security;

import com.example.auth.model.User;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.authorities.SimpleGrantedAuthority;
import org.springframework.security.core.userdetails.UserDetails;

import java.util.Collection;
import java.util.Collections;

public class CustomUserDetails implements UserDetails {

    private final User user;

    public CustomUserDetails(User user) {
        this.user = user;
    }

    @Override
    public Collection getAuthorities() {
        return Collections.singletonList(new SimpleGrantedAuthority("ROLE_" + user.getRole()));
    }

    @Override
    public String getPassword() {
        return user.getPassword();
    }

    @Override
    public String getUsername() {
        return user.getUsername();
    }

    @Override
    public boolean isAccountNonExpired() {
        return true;
    }

    @Override
    public boolean isAccountNonLocked() {
        return true;
    }

    @Override
    public boolean isCredentialsNonExpired() {
        return true;
    }

    @Override
    public boolean isEnabled() {
        return true;
    }
}

Dependencies (`pom.xml`)
The Maven dependencies required for the project.
    
        org.springframework.boot
        spring-boot-starter-web
    
    
        org.springframework.boot
        spring-boot-starter-data-jpa
    
    
        org.springframework.boot
        spring-boot-starter-security
    
    
        com.h2database
        h2
        runtime
    


Application Properties (`application.properties`)
Configuration properties for the H2 in-memory database and JPA.
spring.datasource.url=jdbc:h2:mem:testdb
spring.datasource.driverClassName=org.h2.Driver
spring.datasource.username=sa
spring.datasource.password=password
spring.jpa.database-platform=org.hibernate.dialect.H2Dialect
spring.h2.console.enabled=true
spring.jpa.hibernate.ddl-auto=update
spring.jpa.show-sql=true

How to Run
	1	Organize the files according to the project structure provided.
	2	Build and run the application using Maven: mvn clean install
	3	mvn spring-boot:run
	4	
	5	The application will be accessible at http://localhost:8080.

Testing the API
You can test the API endpoints using a tool like Postman or curl.
	•	Register a User: Endpoint: POST /api/register Request Body (JSON): {
	•	  "username": "john_doe",
	•	  "password": "password123",
	•	  "role": "USER"
	•	}
	•	 Expected Response: "User registered successfully" (HTTP 200) or an error if the username exists.
	•	Login: Endpoint: POST /api/login Request Body (JSON): {
	•	  "username": "john_doe",
	•	  "password": "password123"
	•	}
	•	 Expected Response: "Login successful" (HTTP 200) or "Invalid credentials" (HTTP 401).

This code provides a complete Spring Boot authentication application with manual implementations, avoiding the use of Lombok. Let me know if you need further assistance!
