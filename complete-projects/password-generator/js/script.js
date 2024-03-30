// Seleção de elementos
const generatePasswordButton = document.querySelector('#generate-password');
const generatePasswordElement = document.querySelector('#generated-password')

// Funções
const getLetterLowerCase = () => {
    return String.fromCharCode(Math.floor(Math.random() * 26) + 97);
};

const getLetterUpperCase = () => {
    return String.fromCharCode(Math.floor(Math.random() * 26) + 65);
};

const getNumber = () => {
    return Math.floor(Math.random() * 10).toString();
};

const getSymbols = () => {
    const symbols ="(){}[]=<>,.!@#$%&*+-";
    return symbols[Math.floor(Math.random() * symbols.length)];
};

const generatePassword = (getLetterLowerCase, getLetterUpperCase, getNumber, getSymbols) => {
    let password = "";

    const passwordLength = 10;

    const generators = [
        getLetterLowerCase,
        getLetterUpperCase,
        getNumber,
        getSymbols
    ]

    for (i = 0; i < passwordLength; i = i + 4) {
        generators.forEach(() => {
            const randomValue = generators[Math.floor(Math.random() * generators.length)]();

            password += randomValue;
        });
    }

    password = password.slice(0, password.length)

    generatePasswordElement.style.display = 'block'
    generatePasswordElement.querySelector('h4').innerText = password;
};

// Eventos
generatePasswordButton.addEventListener('click', () => {
    generatePassword(getLetterLowerCase, getLetterUpperCase, getNumber, getSymbols);
});