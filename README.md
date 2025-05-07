Below is a complete explanation and implementation of a Spring Boot backend application that provides API endpoints for user registration and login. The application is designed to be usable directly via a browser or API tools like Postman, with no frontend included. Everything is consolidated into a single Java file for simplicity, and I’ll explain each component thoroughly. The code will be presented without breaks (as requested), and I’ll ensure it’s fully functional for browser-based API interaction.

Overview
This Spring Boot application provides two main API endpoints:
	•	/api/register: Allows users to register with a username, password, and role.
	•	/api/login: Allows users to log in by verifying their credentials.
The application uses:
	•	H2 in-memory database for storing user data (data resets on restart).
	•	Spring Security with HTTP Basic Authentication to secure endpoints.
	•	BCrypt for password encryption.
You can interact with these endpoints using a browser tool like Postman or cURL, sending JSON requests and receiving plain text responses.

Complete Backend Code: `AuthApplication.java`
Here’s the full Java code in a single file. It includes the main application, user entity, repository, service, controller, and security configuration.
package com.example.auth;import org.springframework.boot.SpringApplication;import org.springframework.boot.autoconfigure.SpringBootApplication;import org.springframework.context.annotation.Configuration;import org.springframework.data.jpa.repository.JpaRepository;import org.springframework.security.config.annotation.web.builders.HttpSecurity;import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;import org.springframework.security.core.userdetails.UserDetailsService;import org.springframework.security.core.userdetails.UsernameNotFoundException;import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;import org.springframework.security.crypto.password.PasswordEncoder;import org.springframework.security.web.SecurityFilterChain;import org.springframework.stereotype.Repository;import org.springframework.stereotype.Service;import org.springframework.web.bind.annotation.*;import jakarta.persistence.*;import java.util.Optional;@SpringBootApplication public class AuthApplication {public static void main(String[] args) {SpringApplication.run(AuthApplication.class, args);}@Entity public static class User {@Id @GeneratedValue(strategy = GenerationType.IDENTITY)private Long id;private String username;private String password;private String role;public Long getId() {return id;}public void setId(Long id) {this.id = id;}public String getUsername() {return username;}public void setUsername(String username) {this.username = username;}public String getPassword() {return password;}public void setPassword(String password) {this.password = password;}public String getRole() {return role;}public void setRole(String role) {this.role = role;}}@Repository public interface UserRepository extends JpaRepository {Optional findByUsername(String username);}@Service public static class UserServiceImpl implements UserService {private final UserRepository userRepository;private final PasswordEncoder passwordEncoder;public UserServiceImpl(UserRepository userRepository, PasswordEncoder passwordEncoder) {this.userRepository = userRepository;this.passwordEncoder = passwordEncoder;}@Override public User saveUser(User user) {user.setPassword(passwordEncoder.encode(user.getPassword()));return userRepository.save(user);}@Override public Optional findByUsername(String username) {return userRepository.findByUsername(username);}}public interface UserService {User saveUser(User user);Optional findByUsername(String username);}@RestController @RequestMapping("/api")public static class UserController {private final UserService userService;private final PasswordEncoder passwordEncoder;public UserController(UserService userService, PasswordEncoder passwordEncoder) {this.userService = userService;this.passwordEncoder = passwordEncoder;}@PostMapping("/register")public String register(@RequestBody User user) {if (userService.findByUsername(user.getUsername()).isPresent()) {return "Username already exists";}userService.saveUser(user);return "User registered successfully";}@PostMapping("/login")public String login(@RequestBody User user) {Optional existingUser = userService.findByUsername(user.getUsername());if (existingUser.isPresent() && passwordEncoder.matches(user.getPassword(), existingUser.get().getPassword())) {return "Login successful";} else {return "Invalid credentials";}}}@Configuration @EnableWebSecurity public static class SecurityConfig {private final UserService userService;public SecurityConfig(UserService userService) {this.userService = userService;}@Bean public UserDetailsService userDetailsService() {return username -> userService.findByUsername(username).map(user -> org.springframework.security.core.userdetails.User.withUsername(user.getUsername()).password(user.getPassword()).roles(user.getRole()).build()).orElseThrow(() -> new UsernameNotFoundException("User not found"));}@Bean public PasswordEncoder passwordEncoder() {return new BCryptPasswordEncoder();}@Bean public SecurityFilterChain securityFilterChain(HttpSecurity http) throws Exception {http.csrf(csrf -> csrf.disable()).authorizeHttpRequests(authorizeRequests -> authorizeRequests.requestMatchers("/api/register", "/api/login").permitAll().anyRequest().authenticated()).httpBasic(httpBasic -> {});return http.build();}}}

Dependencies
To run this code, you’ll need a pom.xml file with the following dependencies. This is a standard configuration file for Maven and isn’t part of the main code file:
    
        org.springframework.boot
        spring-boot-starter-web
    
    
        org.springframework.boot
        spring-boot-starter-data-jpa
    
    
        org.springframework.boot
        spring-boot-starter-security
    
    
        com.h2database
        h2
        runtime
    


How to Run the Application
	1	Save the Code: Place AuthApplication.java in a directory like src/main/java/com/example/auth/.
	2	Add pom.xml: Place the pom.xml in the root directory of your project.
	3	Run the Application: Use the following command in the terminal (assuming Maven is installed): mvn spring-boot:run
	4	
	5	Access the API: The application will start on http://localhost:8080.

Explanation of Components
1. Main Application Class
	•	AuthApplication: The entry point of the Spring Boot application, annotated with @SpringBootApplication. It bootstraps the entire application when you run the main method.
2. User Entity
	•	User Class: Represents a user in the system.
	◦	Fields:
	▪	id: Auto-generated unique identifier.
	▪	username: The user’s login name.
	▪	password: The user’s password (stored encrypted).
	▪	role: The user’s role (e.g., “USER”).
	◦	Annotations:
	▪	@Entity: Marks this as a JPA entity for database storage.
	▪	@Id and @GeneratedValue: Define the primary key and its auto-increment behavior.
	◦	Getters/Setters: Allow access to the fields.
3. User Repository
	•	UserRepository Interface: Extends JpaRepository to provide database operations (e.g., save, find).
	◦	Custom Method: findByUsername retrieves a user by their username.
	◦	Annotation: @Repository marks it as a Spring Data repository.
4. User Service
	•	UserService Interface: Defines methods for user operations.
	◦	saveUser: Saves a user to the database.
	◦	findByUsername: Finds a user by username.
	•	UserServiceImpl Class: Implements the interface.
	◦	Dependencies: Uses UserRepository for database access and PasswordEncoder to encrypt passwords.
	◦	Logic: Encodes the password before saving and delegates to the repository.
	◦	Annotation: @Service marks it as a Spring service.
5. User Controller
	•	UserController Class: Handles HTTP requests to the API endpoints.
	◦	Annotations:
	▪	@RestController: Indicates this class provides RESTful endpoints.
	▪	@RequestMapping("/api"): Sets the base URL path for all endpoints.
	◦	Endpoints:
	▪	POST /api/register:
	▪	Input: JSON object with username, password, and role.
	▪	Logic: Checks if the username exists; if not, saves the user with an encrypted password.
	▪	Output: Text response (“User registered successfully” or “Username already exists”).
	▪	POST /api/login:
	▪	Input: JSON object with username and password.
	▪	Logic: Verifies the username and password; returns a success or failure message.
	▪	Output: Text response (“Login successful” or “Invalid credentials”).
6. Security Configuration
	•	SecurityConfig Class: Configures Spring Security.
	◦	Annotations:
	▪	@Configuration: Marks it as a configuration class.
	▪	@EnableWebSecurity: Enables Spring Security features.
	◦	Beans:
	▪	userDetailsService: Loads user details from the database for authentication.
	▪	passwordEncoder: Uses BCryptPasswordEncoder to encrypt and verify passwords.
	▪	securityFilterChain: Configures security rules:
	▪	Disables CSRF (not needed for stateless API).
	▪	Allows unauthenticated access to /api/register and /api/login.
	▪	Requires authentication (via HTTP Basic) for all other requests.

Testing the API Endpoints
You can test the API using tools like Postman or cURL by sending HTTP requests to http://localhost:8080. Here’s how:
1. Register a User
	•	Endpoint: POST http://localhost:8080/api/register
	•	Request Body (JSON): {
	•	  "username": "john_doe",
	•	  "password": "password123",
	•	  "role": "USER"
	•	}
	•	
	•	Response:
	◦	Success: "User registered successfully"
	◦	Failure (username taken): "Username already exists"
	•	cURL Command: curl -X POST http://localhost:8080/api/register -H "Content-Type: application/json" -d '{"username": "john_doe", "password": "password123", "role": "USER"}'
	•	
2. Login a User
	•	Endpoint: POST http://localhost:8080/api/login
	•	Request Body (JSON): {
	•	  "username": "john_doe",
	•	  "password": "password123"
	•	}
	•	
	•	Response:
	◦	Success: "Login successful"
	◦	Failure: "Invalid credentials"
	•	cURL Command: curl -X POST http://localhost:8080/api/login -H "Content-Type: application/json" -d '{"username": "john_doe", "password": "password123"}'
	•	

How It Works
	1	Registration:
	◦	Send a POST request to /api/register with user details in JSON.
	◦	The controller checks if the username is taken. If not, it encrypts the password and saves the user.
	◦	Returns a success or error message.
	2	Login:
	◦	Send a POST request to /api/login with username and password in JSON.
	◦	The controller verifies the credentials by checking the username and matching the password (using BCrypt).
	◦	Returns a success or error message.
	3	Security:
	◦	Spring Security allows public access to /api/register and /api/login.
	◦	Other endpoints (if added) would require HTTP Basic Authentication using the registered username and password.

Notes
	•	Database: Uses H2 in-memory, so data is lost when the application restarts.
	•	Password Security: Passwords are encrypted with BCrypt before storage.
	•	Browser Usability: The API returns plain text responses, making it easy to test with tools like Postman or cURL directly from a browser environment.
This setup provides a fully functional backend API for user authentication, ready to be used via browser-based tools!
