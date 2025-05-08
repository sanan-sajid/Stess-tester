Great — with three eligibility criteria (CGPA, age, income), we can update the logic to filter students accordingly.

⸻

Updated Goal:

When a new Scholarship is created (with min CGPA, max age, max income), find all students who meet all three criteria, and insert entries into scholarship_status.

⸻

1. Update Entity Classes

Student.java

@Entity
public class Student {
    @Id
    @GeneratedValue
    private Long id;

    private String name;
    private String email;
    private double gpa;
    private int age;
    private double income;

    // getters and setters
}

Scholarship.java

@Entity
public class Scholarship {
    @Id
    @GeneratedValue
    private Long id;

    private String name;
    private double minGpa;
    private int maxAge;
    private double maxIncome;

    // getters and setters
}



⸻

2. Update Repository

StudentRepository.java

public interface StudentRepository extends JpaRepository<Student, Long> {

    List<Student> findByGpaGreaterThanEqualAndAgeLessThanEqualAndIncomeLessThanEqual(
        double gpa, int age, double income
    );
}



⸻

3. Updated Service Logic

ScholarshipService.java

@Service
public class ScholarshipService {

    @Autowired
    private StudentRepository studentRepo;

    @Autowired
    private ScholarshipRepository scholarshipRepo;

    @Autowired
    private ScholarshipStatusRepository statusRepo;

    public Scholarship addScholarship(Scholarship scholarship) {
        Scholarship savedScholarship = scholarshipRepo.save(scholarship);

        List<Student> eligibleStudents = studentRepo
            .findByGpaGreaterThanEqualAndAgeLessThanEqualAndIncomeLessThanEqual(
                scholarship.getMinGpa(),
                scholarship.getMaxAge(),
                scholarship.getMaxIncome()
            );

        List<ScholarshipStatus> statuses = eligibleStudents.stream().map(student -> {
            ScholarshipStatus status = new ScholarshipStatus();
            status.setStudentEmail(student.getEmail());
            status.setScholarshipId(savedScholarship.getId());
            return status;
        }).collect(Collectors.toList());

        statusRepo.saveAll(statuses);

        return savedScholarship;
    }
}



⸻

4. Sample Scholarship POST Request

Send a POST request like:

{
  "name": "Merit + Financial Need",
  "minGpa": 3.5,
  "maxAge": 25,
  "maxIncome": 50000
}



⸻

This will:
	•	Filter students by CGPA ≥ minGpa, age ≤ maxAge, and income ≤ maxIncome.
	•	Insert entries into scholarship_status for each eligible student.

Would you like to auto-email students when they’re added to a scholarship status, or just store the info?