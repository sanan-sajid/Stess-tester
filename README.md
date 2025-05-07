I understand that the formatting of the previous code might have been difficult to follow, and you’d like it split into even more files for better organization. Below, I’ll refactor the code into a more modular structure by breaking it down further into additional files. This will improve readability, maintainability, and scalability while keeping the application functional for browser-based API testing (e.g., via Postman or cURL).

Updated Project Structure
Here’s the refactored project structure with additional files for better separation of concerns:
src/main/java/com/example/auth/
├── AuthApplication.java          // Main application class
├── model/
│   └── User.java                 // User entity
├── dto/
│   ├── UserRegistrationDto.java  // DTO for user registration
│   └── UserLoginDto.java         // DTO for user login
├── repository/
│   └── UserRepository.java       // User repository interface
├── service/
│   ├── UserService.java          // User service interface
│   └── UserServiceImpl.java      // User service implementation
├── controller/
│   └── UserController.java       // User controller for API endpoints
├── config/
│   ├── SecurityConfig.java       // Spring Security configuration
│   └── JpaConfig.java            // JPA configuration
├── exception/
│   └── UserAlreadyExistsException.java  // Custom exception for duplicate usernames
└── security/
    └── CustomUserDetails.java    // Custom UserDetails implementation

Code Files
1. `AuthApplication.java`
The main entry point of the Spring Boot application.
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
The User entity for database storage.
package com.example.auth.model;

import jakarta.persistence.*;
import lombok.Data;

@Entity
@Table(name = "users")
@Data
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
}

3. `UserRegistrationDto.java` (DTO)
Data Transfer Object for user registration.
package com.example.auth.dto;

import lombok.Data;

@Data
public class UserRegistrationDto {
    private String username;
    private String password;
    private String role;
}

4. `UserLoginDto.java` (DTO)
Data Transfer Object for user login.
package com.example.auth.dto;

import lombok.Data;

@Data
public class UserLoginDto {
    private String username;
    private String password;
}

5. `UserRepository.java`
Database operations for the User entity.
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
Service interface for user operations.
package com.example.auth.service;

import com.example.auth.model.User;

import java.util.Optional;

public interface UserService {
    User registerUser(User user);
    Optional findByUsername(String username);
}

7. `UserServiceImpl.java` (Implementation)
Implementation of the UserService interface.
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
Handles HTTP requests for registration and login.
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
Spring Security configuration.
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
JPA-specific configuration (separated from security).
package com.example.auth.config;

import org.springframework.context.annotation.Configuration;
import org.springframework.data.jpa.repository.config.EnableJpaRepositories;

@Configuration
@EnableJpaRepositories(basePackages = "com.example.auth.repository")
public class JpaConfig {
}

11. `UserAlreadyExistsException.java`
Custom exception for duplicate usernames.
package com.example.auth.exception;

public class UserAlreadyExistsException extends RuntimeException {
    public UserAlreadyExistsException(String message) {
        super(message);
    }
}

12. `CustomUserDetails.java`
Custom implementation of UserDetails for Spring Security.
package com.example.auth.security;

import com.example.auth.model.User;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.authorities.SimpleGrantedAuthority;

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
Ensure these dependencies are in your pom.xml:
    
        org.springframework.boot
        spring-boot-starter-web
    
    
        org.springframework.boot
        spring-boot-starter-data-jpa
    
    
        org.springframework.boot
        spring-boot-starter-security
    
    
        com.h2database
        h2
        runtime
    
    
        org.projectlombok
        lombok
        true
    


Application Properties (`application.properties`)
Configure the H2 database and JPA:
spring.datasource.url=jdbc:h2:mem:testdb
spring.datasource.driverClassName=org.h2.Driver
spring.datasource.username=sa
spring.datasource.password=password
spring.jpa.database-platform=org.hibernate.dialect.H2Dialect
spring.h2.console.enabled=true
spring.jpa.hibernate.ddl-auto=update
spring.jpa.show-sql=true

How to Run
	1	Organize Files: Place each file in its respective package.
	2	Build and Run: mvn clean install
	3	mvn spring-boot:run
	4	
	5	Access: The app runs on http://localhost:8080.

Testing the API
Register a User
	•	Endpoint: POST http://localhost:8080/api/register
	•	Body: {
	•	  "username": "john_doe",
	•	  "password": "password123",
	•	  "role": "USER"
	•	}
	•	
	•	Response: 200 OK with "User registered successfully" or 400 Bad Request with an error message.
Login a User
	•	Endpoint: POST http://localhost:8080/api/login
	•	Body: {
	•	  "username": "john_doe",
	•	  "password": "password123"
	•	}
	•	
	•	Response: 200 OK with "Login successful" or 401 Unauthorized with "Invalid credentials".

Improvements
	•	More Files: Added DTOs, custom exceptions, and separated security/JPA configs.
	•	Better Formatting: Each component is clearly separated and documented.
	•	Maintainability: Smaller, focused files make the code easier to manage.
Let me know if you need further adjustments!
