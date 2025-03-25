#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>

class Book {
public:
    int id;
    std::string title;
    std::string author;
    bool isAvailable;

    Book(int id, std::string title, std::string author)
        : id(id), title(std::move(title)), author(std::move(author)), isAvailable(true) {}

    void display() const {
        std::cout << "ID: " << id << ", Title: " << title << ", Author: " << author
                  << ", Status: " << (isAvailable ? "Available" : "Borrowed") << std::endl;
    }
};

class Member {
public:
    int id;
    std::string name;

    Member(int id, std::string name) : id(id), name(std::move(name)) {}

    void display() const {
        std::cout << "ID: " << id << ", Name: " << name << std::endl;
    }
};

class Library {
private:
    std::vector<Book> books;
    std::vector<Member> members;
    std::string booksFile = "books.txt";
    std::string membersFile = "members.txt";

    void loadBooks() {
        std::ifstream file(booksFile);
        if (!file) {
            std::cerr << "Error: Unable to open books file." << std::endl;
            return;
        }
        books.clear();
        int id;
        std::string title, author;
        bool isAvailable;
        while (file >> id >> std::ws && std::getline(file, title) &&
               std::getline(file, author) >> isAvailable) {
            books.emplace_back(id, title, author);
            books.back().isAvailable = isAvailable;
        }
        file.close();
    }

    void saveBooks() const {
        std::ofstream file(booksFile);
        if (!file) {
            std::cerr << "Error: Unable to save books file." << std::endl;
            return;
        }
        for (const auto& book : books) {
            file << book.id << "\n" << book.title << "\n" << book.author << "\n" << book.isAvailable << "\n";
        }
        file.close();
    }

    void loadMembers() {
        std::ifstream file(membersFile);
        if (!file) {
            std::cerr << "Error: Unable to open members file." << std::endl;
            return;
        }
        members.clear();
        int id;
        std::string name;
        while (file >> id >> std::ws && std::getline(file, name)) {
            members.emplace_back(id, name);
        }
        file.close();
    }

    void saveMembers() const {
        std::ofstream file(membersFile);
        if (!file) {
            std::cerr << "Error: Unable to save members file." << std::endl;
            return;
        }
        for (const auto& member : members) {
            file << member.id << "\n" << member.name << "\n";
        }
        file.close();
    }

public:
    Library() {
        loadBooks();
        loadMembers();
    }

    ~Library() {
        saveBooks();
        saveMembers();
    }

    void addBook(const std::string& title, const std::string& author) {
        int id = books.empty() ? 1 : books.back().id + 1;
        books.emplace_back(id, title, author);
        std::cout << "Book added successfully!" << std::endl;
    }

    void deleteBook(int id) {
        auto it = std::remove_if(books.begin(), books.end(), [id](const Book& book) {
            return book.id == id;
        });
        if (it != books.end()) {
            books.erase(it, books.end());
            std::cout << "Book deleted successfully!" << std::endl;
        } else {
            std::cerr << "Error: Book not found." << std::endl;
        }
    }

    void displayBooks() const {
        if (books.empty()) {
            std::cout << "No books available." << std::endl;
            return;
        }
        for (const auto& book : books) {
            book.display();
        }
    }

    void addMember(const std::string& name) {
        int id = members.empty() ? 1 : members.back().id + 1;
        members.emplace_back(id, name);
        std::cout << "Member added successfully!" << std::endl;
    }

    void deleteMember(int id) {
        auto it = std::remove_if(members.begin(), members.end(), [id](const Member& member) {
            return member.id == id;
        });
        if (it != members.end()) {
            members.erase(it, members.end());
            std::cout << "Member deleted successfully!" << std::endl;
        } else {
            std::cerr << "Error: Member not found." << std::endl;
        }
    }

    void displayMembers() const {
        if (members.empty()) {
            std::cout << "No members registered." << std::endl;
            return;
        }
        for (const auto& member : members) {
            member.display();
        }
    }

    void borrowBook(int bookId, int memberId) {
        auto bookIt = std::find_if(books.begin(), books.end(), [bookId](const Book& book) {
            return book.id == bookId;
        });
        if (bookIt == books.end()) {
            std::cerr << "Error: Book not found." << std::endl;
            return;
        }
        if (!bookIt->isAvailable) {
            std::cerr << "Error: Book is already borrowed." << std::endl;
            return;
        }
        auto memberIt = std::find_if(members.begin(), members.end(), [memberId](const Member& member) {
            return member.id == memberId;
        });
        if (memberIt == members.end()) {
            std::cerr << "Error: Member not found." << std::endl;
            return;
        }
        bookIt->isAvailable = false;
        std::cout << "Book borrowed successfully!" << std::endl;
    }

    void returnBook(int bookId) {
        auto bookIt = std::find_if(books.begin(), books.end(), [bookId](const Book& book) {
            return book.id == bookId;
        });
        if (bookIt == books.end()) {
            std::cerr << "Error: Book not found." << std::endl;
            return;
        }
        if (bookIt->isAvailable) {
            std::cerr << "Error: Book is not borrowed." << std::endl;
            return;
        }
        bookIt->isAvailable = true;
        std::cout << "Book returned successfully!" << std::endl;
    }
};

void displayMenu() {
    std::cout << "\nLibrary Management System\n";
    std::cout << "1. Add Book\n";
    std::cout << "2. Delete Book\n";
    std::cout << "3. Display Books\n";
    std::cout << "4. Add Member\n";
    std::cout << "5. Delete Member\n";
    std::cout << "6. Display Members\n";
    std::cout << "7. Borrow Book\n";
    std::cout << "8. Return Book\n";
    std::cout << "9. Exit\n";
}

int main() {
    Library library;
    int choice;

    while (true) {
        displayMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                std::string title, author;
                std::cin.ignore();
                std::cout << "Enter book title: ";
                std::getline(std::cin, title);
                std::cout << "Enter book author: ";
                std::getline(std::cin, author);
                library.addBook(title, author);
                break;
            }
            case 2: {
                int id;
                std::cout << "Enter book ID to delete: ";
                std::cin >> id;
                library.deleteBook(id);
                break;
            }
            case 3:
                library.displayBooks();
                break;
            case 4: {
                std::string name;
                std::cin.ignore();
                std::cout << "Enter member name: ";
                std::getline(std::cin, name);
                library.addMember(name);
                break;
            }
            case 5: {
                int id;
                std::cout << "Enter member ID to delete: ";
                std::cin >> id;
                library.deleteMember(id);
                break;
            }
            case 6:
                library.displayMembers();
                break;
            case 7: {
                int bookId, memberId;
                std::cout << "Enter book ID to borrow: ";
                std::cin >> bookId;
                std::cout << "Enter member ID: ";
                std::cin >> memberId;
                library.borrowBook(bookId, memberId);
                break;
            }
            case 8: {
                int bookId;
                std::cout << "Enter book ID to return: ";
                std::cin >> bookId;
                library.returnBook(bookId);
                break;
            }
            case 9:
                std::cout << "Exiting the system. Goodbye!\n";
                return 0;
            default:
                std::cerr << "Invalid choice. Please try again.\n";
        }
    }
}