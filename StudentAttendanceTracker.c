#include <stdio.h>


#define MAX_STUDENTS 100
#define NAME_LEN 50
#define DATE_LEN 20

struct Student {
    int roll;
    char name[NAME_LEN];
};

// ---------------- CUSTOM STRING FUNCTIONS ----------------
int str_compare(char s1[], char s2[]) {
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i])
            return 0;
        i++;
    }
    return (s1[i] == '\0' && s2[i] == '\0');
}
void remove_newline(char s[]) {
    int i = 0;
    while (s[i] != '\0') {
        if (s[i] == '\n') {
            s[i] = '\0';
            break;
        }
        i++;
    }
}

// ---------------- STUDENT MODULE ----------------
int load_students(struct Student students[]) {
    FILE *f = fopen("students.txt", "r");
    int count = 0;
    if (!f) return 0;
    while (fscanf(f, "%d,%49[^\n]", &students[count].roll, students[count].name) == 2) {
        count++;
        if (count >= MAX_STUDENTS) break;
    }
    fclose(f);
    return count;
}
void save_student(struct Student s) {
    FILE *f = fopen("students.txt", "a");
    if (!f) { printf("Error opening students.txt\n"); return; }
    fprintf(f, "%d,%s\n", s.roll, s.name);
    fclose(f);
}
void add_student(struct Student students[], int *n) {
    if (*n >= MAX_STUDENTS) {
        printf("Limit reached!\n");
        return;
    }
    struct Student s;
    printf("Enter roll number: ");
    scanf("%d", &s.roll);
    getchar();
    printf("Enter name: ");
    fgets(s.name, NAME_LEN, stdin);
    remove_newline(s.name);

    for (int i = 0; i < *n; i++) {
        if (students[i].roll == s.roll) {
            printf("Roll number already exists!\n");
            return;
        }
    }

    students[*n] = s;
    (*n)++;
    save_student(s);
    printf("? Student added successfully!\n");
}
void list_students(struct Student students[], int n) {
    if (n == 0) {
        printf("No students found. Add students first.\n");
        return;
    }
    printf("\n--- Student List ---\nRoll\tName\n");
    for (int i = 0; i < n; i++)
        printf("%d\t%s\n", students[i].roll, students[i].name);
}

// ---------------- ATTENDANCE MODULE ----------------
void mark_attendance(struct Student students[], int n) {
    if (n == 0) {
        printf("No students to mark. Add students first.\n");
        return;
    }

    FILE *f = fopen("attendance.txt", "a");
    if (!f) { printf("Error opening attendance.txt\n"); return; }

    char date[DATE_LEN];
    printf("Enter date (DD-MM-YYYY): ");
    scanf("%s", date);

    printf("\nMark attendance (P = Present, A = Absent):\n");
    for (int i = 0; i < n; i++) {
        char status;
        printf("%d - %s (P/A): ", students[i].roll, students[i].name);
        scanf(" %c", &status);
        if (status == 'P' || status == 'p')
            fprintf(f, "%s,%d,P\n", date, students[i].roll);
        else
            fprintf(f, "%s,%d,A\n", date, students[i].roll);
    }

    fclose(f);
    printf("\n? Attendance saved for %s!\n", date);
}

// ---------------- REPORT MODULE ----------------
void student_report(struct Student students[], int n) {
    if (n == 0) { printf("No students in list.\n"); return; }

    int roll, total = 0, present = 0;
    char date[DATE_LEN], status;
    printf("Enter roll number: ");
    scanf("%d", &roll);

    FILE *f = fopen("attendance.txt", "r");
    if (!f) { printf("No attendance records found.\n"); return; }

    char line[200];
    while (fgets(line, sizeof(line), f)) {
        int r;
        sscanf(line, "%[^,],%d,%c", date, &r, &status);
        if (r == roll) {
            total++;
            if (status == 'P') present++;
        }
    }
    fclose(f);

    if (total == 0) { printf("No records found for this roll number.\n"); return; }

    double percent = (present * 100.0) / total;
    printf("\nReport for Roll No %d\n", roll);
    printf("Total Classes: %d\nPresent: %d\nAttendance: %.2f%%\n", total, present, percent);
}
void date_summary() {
    FILE *f = fopen("attendance.txt", "r");
    if (!f) { printf("No attendance data available.\n"); return; }

    char search_date[DATE_LEN], date[DATE_LEN], status, line[200];
    int roll, present = 0, absent = 0;

    printf("Enter date (DD-MM-YYYY): ");
    scanf("%s", search_date);

    while (fgets(line, sizeof(line), f)) {
        sscanf(line, "%[^,],%d,%c", date, &roll, &status);
        if (str_compare(date, search_date)) {
            if (status == 'P') present++;
            else absent++;
        }
    }
    fclose(f);

    printf("\nSummary for %s\nPresent: %d\nAbsent: %d\nTotal: %d\n",
           search_date, present, absent, present + absent);
}


// ---------------- MAIN PROGRAM ----------------
int main() {
    struct Student students[MAX_STUDENTS];
    int n = load_students(students);
    int choice;

    printf("====== MINI ATTENDANCE TRACKER - BSC IT B SECTION ======\n");

    while (1) {
        printf("\n1. Add Student\n2. List Students\n3. Mark Attendance\n4. Student Report\n5. Date Summary\n6. Exit\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_student(students, &n); break;
            case 2: list_students(students, n); break;
            case 3: mark_attendance(students, n); break;
            case 4: student_report(students, n); break;
            case 5: date_summary(); break;
            case 6: printf("\nExiting program... Goodbye!\n"); return 0;
            default: printf("Invalid choice! Try again.\n");
        }
    }
}