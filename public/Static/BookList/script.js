// The Book Class: Represents the book
class Book {
    constructor(title, author, id) {
        this.title = title;
        this.author = author;
        this.id = id;
    }
}

// UI class: to handle UI Tasks
class UI {
    static displayBooks() {
        const books = Store.getBooks();
        books.forEach((book) => UI.addBookToList(book));
    }
    static addBookToList(book) {
        const list = document.getElementById("book-list");
        const row = document.createElement('tr');
        row.innerHTML = `
            <td>${book.title}</td>
            <td>${book.author}</td>
            <td>${book.id}</td>
            <td><a href="#" class="btn btn-danger btn-sm  delete">X</a></td>
        `
        list.appendChild(row);
    }

    static deleteBook(target) {
        if (target.classList.contains("delete")){
            target.parentElement.parentElement.remove();
        }
    }
    
    static showAlert(message, className) {
        const div = document.createElement('div');
        div.className = `alert alert-${className}`;
        div.appendChild(document.createTextNode(message));
        const container = document.querySelector('.container');
        const form = document.querySelector('#book-form');
        container.insertBefore(div, form);  

        // vanishing after 3seconds
        setTimeout(function() {
            document.querySelector(".alert").remove()
        }, 2000);
    }


    static clearFields() {
        const list = document.querySelectorAll('input[type="text"]');
        list.forEach((a) => a.value = '');
    }

    
}

// Store Class: handles Storage
class Store  {
    static getBooks() {
        let books;
        if (localStorage.getItem('books') === null) {
            books = []
        }else {
            books = JSON.parse(localStorage.getItem('books'));
        }
        return books;
    }

    static addBook(book) {
        const books = Store.getBooks();
        books.push(book);
        localStorage.setItem('books', JSON.stringify(books));
    }
    static removeBook(id){
        const books  = Store.getBooks();
        books.forEach(function(book, index) {
            if (book.id === id) {
                books.splice(index, 1);
            }
        });
        localStorage.setItem('books', JSON.stringify(books));
    }
}

// Event: Display Books
document.addEventListener('DOMContentLoaded', UI.displayBooks());

// Event: Add a book
document.querySelector("#book-form").addEventListener('submit', (e) => {
    
    // prevent default action of the form 
    e.preventDefault();
    
    // get form values;
    const title = document.querySelector("#title").value;
    const author = document.querySelector("#author").value;
    const id = document.querySelector("#id").value;

    // validate the data

    if (title === '' || author === '' || id === '')
        UI.showAlert("Please Fill in all fields", 'danger');
    else {
        // instantiate a book
        const book = new Book(title, author, id);
    
        // add the book to UI
        UI.addBookToList(book);
        
        Store.addBook(book);
        // show success alert
        UI.showAlert("Book added succefully", 'success');
        
        // clear input fields
        UI.clearFields();
    }
});

// Event Remove a Book
document.querySelector("#book-list").addEventListener('click', (a) => {
    UI.deleteBook(a.target);
    // remove book from the store

    Store.removeBook(a.target.parentElement.previousElementSibling.textContent);
    UI.showAlert("Book Removed", 'success');
}
)