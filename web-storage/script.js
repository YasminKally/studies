// Inserir itens
localStorage.setItem('name', 'Yasmin');

// Restart sem perder dados

// Resgatar itens
const name = localStorage.getItem('name');
console.log(name);

// Resgatar itens que não existem
const lastName = localStorage.getItem('lastName'); //retorna null

// Remover itens
localStorage.removeItem('key');

// limpar todos os itens
localStorage.clear();

// Salvar objetos
const person = {
    name: 'Yasmin',
    age: 19,
};

localStorage.setItem('person', JSON.stringify(person));

// Resgatar objetos
const getPerson = localStorage.getItem('person');
const personObject = JSON.parse(getPerson);

console.log(personObject.age);