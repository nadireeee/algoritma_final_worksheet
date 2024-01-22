#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>



class Employee {
public:
    int id;
    std::string name;
    int extraPerformancePoint;
    double weeklyWorkHours;
    double salary;
    float completedWork;
    int performanceRating;

    Employee(int _id, std::string _name, double _weeklyWorkHours, double _salary, float _completedWork)
        : id(_id), name(_name), weeklyWorkHours(_weeklyWorkHours), salary(_salary), completedWork(_completedWork), extraPerformancePoint(0) {
        UpdatePerformanceRating();
    }

    void UpdatePerformanceRating() {
        double performanceRatio = (completedWork / weeklyWorkHours) + (weeklyWorkHours / salary);
        performanceRating = static_cast<int>(performanceRatio * 100);
    }

    bool operator<(const Employee& other) const {
        return performanceRating > other.performanceRating;
    }

    void AddCompletedWork(float work) {
        completedWork += work;
        UpdatePerformanceRating();
    }

    void AddExtraPerformancePoint(int point) {
        extraPerformancePoint += point;
        UpdatePerformanceRating();
    }
};

class HRSystem {
private:
    std::vector<Employee> employeeList;
    int nextID = 1;

public:
    const std::vector<Employee>& GetEmployeeList() const {
        return employeeList;
    }

    void AddEmployee() {
        std::string name;
        double weeklyWorkHours, salary;
        float completedWork;

        std::cout << "Employee name: ";
        std::cin >> name;

        do {
            std::cout << "Weekly work hours (0-80): ";
            std::cin >> weeklyWorkHours;
        } while (weeklyWorkHours < 0 || weeklyWorkHours > 80);

        do {
            std::cout << "Salary (minimum 17000): ";
            std::cin >> salary;
        } while (salary < 17000);

        std::cout << "Completed work: ";
        std::cin >> completedWork;

        Employee newEmployee(nextID++, name, weeklyWorkHours, salary, completedWork);
        employeeList.push_back(newEmployee);
        std::cout << "Employee added: " << name << std::endl;
    }

    void AddCompletedWork(int id, float work) {
        auto it = std::find_if(employeeList.begin(), employeeList.end(), [id](const Employee& e) {
            return e.id == id;
        });

        if (it != employeeList.end()) {
            it->AddCompletedWork(work);
            std::cout << "Completed work added for Employee ID " << id << ": " << work << " units." << std::endl;
        } else {
            std::cout << "Employee not found." << std::endl;
        }
    }

    void RemoveEmployee(int id) {
        auto it = std::remove_if(employeeList.begin(), employeeList.end(), [id](const Employee& e) {
            return e.id == id;
        });

        if (it != employeeList.end()) {
            employeeList.erase(it, employeeList.end());
            std::cout << "Employee removed: ID " << id << std::endl;
        } else {
            std::cout << "Employee not found." << std::endl;
        }
    }

    void ListEmployees() const {
        std::cout << "Employee List:" << std::endl;
        for (const auto& employee : employeeList) {
            std::cout << "ID: " << employee.id << ", Name: " << employee.name << ", Performance Rating: " << employee.performanceRating
                      << ", Extra Performance Point: " << employee.extraPerformancePoint
                      << ", Weekly Work Hours: " << employee.weeklyWorkHours << ", Salary: " << employee.salary
                      << ", Completed Work: " << employee.completedWork << std::endl;
        }
    }

    void UpdateEmployeePerformance(int id, int newPerformanceRating) {
        auto it = std::find_if(employeeList.begin(), employeeList.end(), [id](const Employee& e) {
            return e.id == id;
        });

        if (it != employeeList.end()) {
            it->AddExtraPerformancePoint(newPerformanceRating);
            std::cout << "Extra performance point added: ID " << id << ", New Performance Rating: " << it->performanceRating << std::endl;
        } else {
            std::cout << "Employee not found." << std::endl;
        }
    }

    void UpdateEmployeeWorkAndSalary(int id, double newWeeklyWorkHours, double newSalary) {
        auto it = std::find_if(employeeList.begin(), employeeList.end(), [id](const Employee& e) {
            return e.id == id;
        });

        if (it != employeeList.end()) {
            it->weeklyWorkHours = std::min(std::max(newWeeklyWorkHours, 0.0), 80.0);
            it->salary = std::max(newSalary, 17000.0);
            it->UpdatePerformanceRating();
            std::cout << "Employee weekly work hours and salary updated: ID " << id
                      << ", New Weekly Work Hours: " << it->weeklyWorkHours << ", New Salary: " << it->salary << std::endl;
        } else {
            std::cout << "Employee not found." << std::endl;
        }
    }

    void CompareAndReport(int id, double oldWeeklyWorkHours, double oldSalary) const {
        auto it = std::find_if(employeeList.begin(), employeeList.end(), [id](const Employee& e) {
            return e.id == id;
        });

        if (it != employeeList.end()) {
            double newRatio = (it->performanceRating + it->extraPerformancePoint) / 100.0;
            double oldRatio = (oldWeeklyWorkHours / oldSalary) + (oldSalary / oldWeeklyWorkHours);

            if (newRatio > oldRatio) {
                std::cout << "Improvement detected!" << std::endl;
            } else if (newRatio < oldRatio) {
                std::cout << "Decline detected!" << std::endl;
            } else {
                std::cout << "No change." << std::endl;
            }
        } else {
            std::cout << "Employee not found." << std::endl;
        }
    }

    void SearchEmployee(const std::string& searchTerm) const {
    // Arama terimine uyan çalışanları bul ve bilgilerini yazdır
    bool found = false;
    for (const auto& employee : employeeList) {
        if (employee.name.find(searchTerm) != std::string::npos) {
            std::cout << "ID: " << employee.id << ", Name: " << employee.name << ", Performance Rating: " << employee.performanceRating
                      << ", Extra Performance Point: " << employee.extraPerformancePoint
                      << ", Weekly Work Hours: " << employee.weeklyWorkHours << ", Salary: " << employee.salary
                      << ", Completed Work: " << employee.completedWork << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "No employee found with the name containing \"" << searchTerm << "\"." << std::endl;
    }
}

    void ListByEfficiency() {
        // Ek performans puanını da dikkate alarak sırala
        std::sort(employeeList.begin(), employeeList.end(), [](const Employee& a, const Employee& b) {
            return (a.performanceRating + a.extraPerformancePoint) > (b.performanceRating + b.extraPerformancePoint);
        });

        ListEmployees();
    }
};
void ShowMenu() {
    std::cout << "--------------------------------" << std::endl;
    std::cout << "1. Add Employee" << std::endl;
    std::cout << "2. Remove Employee" << std::endl;
    std::cout << "3. List Employees" << std::endl;
    std::cout << "4. Update Employee Performance" << std::endl;
    std::cout << "5. Update Employee Work and Salary" << std::endl;
    std::cout << "6. Compare and Report" << std::endl;
    std::cout << "7. List by Efficiency" << std::endl;
    std::cout << "8. Add Completed Work" << std::endl;
    std::cout << "9. Search Employee" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "--------------------------------" << std::endl;
}

int main() {
    int choice = -1; // choice'ı -1 ile başlatıyoruz
    HRSystem hrSystem;

    do {
        ShowMenu();

        std::cout << "Enter your choice (0-9): ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: {
                hrSystem.AddEmployee();
                break;
            }
            case 2: {
                int removeID;
                std::cout << "Enter the ID of the employee to remove: ";
                std::cin >> removeID;
                hrSystem.RemoveEmployee(removeID);
                break;
            }
            case 3: {
                hrSystem.ListEmployees();
                break;
            }
            case 4: {
                int updateID, newPerformance;
                std::cout << "Enter the ID of the employee to update: ";
                std::cin >> updateID;
                std::cout << "Enter the extra performance point to add: ";
                std::cin >> newPerformance;
                hrSystem.UpdateEmployeePerformance(updateID, newPerformance);
                break;
            }
            case 5: {
                int updateWorkSalaryID;
                double newWorkHours, newSalary;
                std::cout << "Enter the ID of the employee to update: ";
                std::cin >> updateWorkSalaryID;
                std::cout << "Enter the new weekly work hours: ";
                std::cin >> newWorkHours;
                std::cout << "Enter the new salary: ";
                std::cin >> newSalary;
                hrSystem.UpdateEmployeeWorkAndSalary(updateWorkSalaryID, newWorkHours, newSalary);
                break;
            }
            case 6: {
                int compareID;
                double oldWorkHours, oldSalary;
                std::cout << "Enter the ID of the employee to compare: ";
                std::cin >> compareID;
                std::cout << "Enter the old weekly work hours: ";
                std::cin >> oldWorkHours;
                std::cout << "Enter the old salary: ";
                std::cin >> oldSalary;
                hrSystem.CompareAndReport(compareID, oldWorkHours, oldSalary);
                break;
            }
            case 7: {
                hrSystem.ListByEfficiency();
                break;
            }
            case 8: {
                int addWorkID;
                float completedWork;
                std::cout << "Enter the ID of the employee to add completed work: ";
                std::cin >> addWorkID;
                std::cout << "Enter the completed work units: ";
                std::cin >> completedWork;
                hrSystem.AddCompletedWork(addWorkID, completedWork);
                break;
            }
            case 9: {
                std::string searchName;
                std::cout << "Enter the name (or part of the name) to search: ";
                std::cin >> searchName;
                hrSystem.SearchEmployee(searchName);
                break;
            }
            case 0: {
                std::cout << "Exiting the program." << std::endl;
                break;
            }
            default: {
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
            }
        }

    } while (choice != 0);

    return 0;
}
