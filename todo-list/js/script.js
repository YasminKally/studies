// Seleção de Elementos
const todoForm = document.querySelector('#todoForm')
const todoInput = document.querySelector('#todoInput')
const todoList = document.querySelector('#todoList')
const editForm = document.querySelector('#editForm')
const editInput = document.querySelector('#editInput')
const cancelEdit = document.querySelector('#cancelEdit')

let oldInputValue;

// Funções
const saveTodo = (text) => {
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

    todoInput.value = ''
    todoInput.focus()
} 

const toggleForms = () => {
    editForm.classList.toggle('hide')
    todoForm.classList.toggle('hide')
    todoList.classList.toggle('hide')
}

const updateTodo = (text) => {
    const todos = document.querySelectorAll('.todo')

    todos.forEach((todo) => {
        let todoTittle = todo.querySelector('h3')
    
        if (todoTittle.innerText === oldInputValue) {
            todoTittle.innerText = text;
        }
    })
}

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
        todoTittle = parentEl.querySelector('h3').innerText;
    }

    if (targetEl.classList.contains('finishTodo')) {
        parentEl.classList.toggle('done');
    }

    if (targetEl.classList.contains('removeTodo')) {
        parentEl.remove();
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