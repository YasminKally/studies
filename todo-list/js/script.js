// Seleção de Elementos
const todoForm = document.querySelector('#todoForm')
const todoInput = document.querySelector('#todoInput')
const todoList = document.querySelector('#todoList')
const editForm = document.querySelector('#editForm')
const editInput = document.querySelector('#editInput')
const cancelEdit = document.querySelector('#cancelEdit')
const searchInput = document.querySelector('#searchInput')
const eraseButton = document.querySelector('#eraseButton')
const selectFilter = document.querySelector('#selectFilter')

let oldInputValue;

// Funções
const saveTodo = (text, done = 0, save = 1) => {
    const todo = document.createElement('div');
    todo.classList.add('todo');

    const todoTittle = document.createElement('h3');
    todoTittle.innerText = text;
    todo.appendChild(todoTittle);

    const doneBtn = document.createElement('button');
    doneBtn.classList.add('finishTodo');
    doneBtn.innerHTML = '<i class="fa-solid fa-check"></i>';
    todo.appendChild(doneBtn);

    const editBtn = document.createElement('button');
    editBtn.classList.add('editTodo');
    editBtn.innerHTML = '<i class="fa-solid fa-pen"></i>';
    todo.appendChild(editBtn);

    const deleteBtn = document.createElement('button');
    deleteBtn.classList.add('removeTodo');
    deleteBtn.innerHTML = '<i class="fa-solid fa-trash"></i>';
    todo.appendChild(deleteBtn);

    todoList.appendChild(todo);

    if (done) {
        todo.classList.add('done');
    }

    if (save) {
        saveToLocalStorage({text, done: 0})
    }

    todoInput.value = '';
    todoInput.focus();
};

const toggleForms = () => {
    editForm.classList.toggle('hide')
    todoForm.classList.toggle('hide')
    todoList.classList.toggle('hide')
};

const updateTodo = (text) => {
    const todos = document.querySelectorAll('.todo')

    todos.forEach((todo) => {
        let todoTittle = todo.querySelector('h3')
    
        if (todoTittle.innerText === oldInputValue) {
            todoTittle.innerText = text;

            editTodoLocalStorage(oldInputValue, text)
        }
    });
};

const getSearchedTodos = (search) => {
    const todos = document.querySelectorAll('.todo');

    todos.forEach((todo) => {
        const todoTittle = todo.querySelector('h3').innerText.toLowerCase();

        todo.style.display = 'flex';

        if (!todoTittle.includes(search)) {
            todo.style.display = 'none'
        }
    }); 
};

const filterTodos = (filterValue) => {
    const todos = document.querySelectorAll('.todo')

    switch (filterValue) {
        case 'all':
            todos.forEach((todo) => (todo.style.display = 'flex'));
            break;

        case 'done':
            todos.forEach((todo) => todo.classList.contains('done') ? (todo.style.display = 'flex') : (todo.style.display = 'none'));
            break;

        case 'todo':
            todos.forEach((todo) => !todo.classList.contains('done') ? (todo.style.display = 'flex') : (todo.style.display = 'none'));
            break;

        default:
            break;
    }
};

// Eventos
todoForm.addEventListener('submit', (e) => {
    e.preventDefault();
    
    const inputValue = todoInput.value;

    if (inputValue) {
        saveTodo(inputValue);
    }
});

document.addEventListener('click', (e) => {
    const targetEl = e.target;
    const parentEl = targetEl.closest('div');
    let todoTittle;

    if (parentEl && parentEl.querySelector('h3')) {
        todoTittle = parentEl.querySelector('h3').innerText || '';
    }

    if (targetEl.classList.contains('finishTodo')) {
        parentEl.classList.toggle('done');

        updateTodoStatusLocalStorage(todoTittle);
    }

    if (targetEl.classList.contains('removeTodo')) {
        parentEl.remove();

        removeFromLocalStorage(todoTittle);
    }

    if (targetEl.classList.contains('editTodo')) {
        toggleForms();

        editInput.value = todoTittle;
        oldInputValue = todoTittle;
    }
});

cancelEdit.addEventListener('click', (e) => {
    e.preventDefault();

    toggleForms();
});

editForm.addEventListener('submit', (e) => {
    e.preventDefault();

    const editInputValue = editInput.value;

    if (editInputValue) {
        updateTodo(editInputValue);
    }

    toggleForms();
});

searchInput.addEventListener('keyup', (e) => {
    const search =  e.target.value;

    getSearchedTodos(search);
});

eraseButton.addEventListener('click', (e) => {
    e.preventDefault();

    searchInput.value = '';
    searchInput.dispatchEvent(new Event ('keyup'));
});

selectFilter.addEventListener('change', (e) => {
    const filterValue = e.target.value;
    
    filterTodos(filterValue);
});

// Local Storage
const getTodosLocalStorage = () => {
    const todos = JSON.parse(localStorage.getItem('todos')) || [];
    return todos;
};

const saveToLocalStorage = (todo) => {
    const todos = getTodosLocalStorage();

    todos.push(todo);
    localStorage.setItem('todos', JSON.stringify(todos));
};

const loadTodos = () => {
    const todos = getTodosLocalStorage();

    todos.forEach((todo) => {
        saveTodo(todo.text, todo.done, 0);
    });
};

const updateTodoStatusLocalStorage = (todoText) => {
    const todos = getTodosLocalStorage();

    todos.map((todo) => todo.text === todoText ? (todo.done = !todo.done) : null);
    localStorage.setItem('todos', JSON.stringify(todos));
};

const editTodoLocalStorage = (todoOldText, todoNewText) => {
    const todos = getTodosLocalStorage();

    todos.map((todo) => todo.text === todoOldText ? (todo.text = todoNewText) : null);
    localStorage.setItem('todos', JSON.stringify(todos))
};

const removeFromLocalStorage = (todoText) => {
    const todos = getTodosLocalStorage();
    const filteredTodos = todos.filter((todo) => todo.text != todoText);
    
    localStorage.setItem('todos', JSON.stringify(filteredTodos))
};

// Start To Do List

loadTodos();